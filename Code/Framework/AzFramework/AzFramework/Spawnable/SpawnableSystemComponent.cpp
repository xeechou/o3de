/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Component/ComponentApplicationLifecycle.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Spawnable/SpawnableAssetRef.h>
#include <AzFramework/Spawnable/SpawnableMetaData.h>
#include <AzFramework/Spawnable/SpawnableNotificationsHandler.h>
#include <AzFramework/Spawnable/SpawnableSystemComponent.h>

namespace AzFramework
{
    void SpawnableSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        Spawnable::Reflect(context);
        SpawnableMetaData::Reflect(context);
        EntitySpawnTicket::Reflect(context);
        SpawnableAssetRef::Reflect(context);

        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context); serializeContext != nullptr)
        {
            serializeContext->Class<SpawnableSystemComponent, AZ::Component>();
            serializeContext->RegisterGenericType<AZ::Data::Asset<Spawnable>>();
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<SpawnableRequestsBus>("SpawnableRequestsBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Prefab/Spawning")
                ->Attribute(AZ::Script::Attributes::Module, "prefabs")
                ->Event("CreateSpawnTicket", &SpawnableRequests::CreateSpawnTicket)
                ->Event("Spawn", &SpawnableRequests::Spawn)
                ->Event("Despawn", &SpawnableRequests::Despawn);

            behaviorContext->EBus<SpawnableNotificationsBus>("SpawnableNotificationsBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Category, "Prefab/Spawning")
                ->Attribute(AZ::Script::Attributes::Module, "prefabs")
                ->Handler<SpawnableNotificationsHandler>();
        }
    }

    void SpawnableSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("SpawnableSystemService"));
    }

    void SpawnableSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("SpawnableSystemService"));
    }

    void SpawnableSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& services)
    {
        services.push_back(AZ_CRC_CE("AssetDatabaseService"));
        services.push_back(AZ_CRC_CE("AssetCatalogService"));
    }

    void SpawnableSystemComponent::OnTick(float /*deltaTime*/, AZ::ScriptTimePoint /*time*/)
    {
        ProcessSpawnableQueue();
        RootSpawnableNotificationBus::ExecuteQueuedEvents();

        ProcessDespawnedResults();
        ProcessSpawnedResults();
    }

    int SpawnableSystemComponent::GetTickOrder()
    {
        return AZ::ComponentTickBus::TICK_GAME;
    }

    void SpawnableSystemComponent::OnSystemTick()
    {
        // Handle only high priority spawning events such as those created from network. These need to happen even if the client
        // doesn't have focus to avoid time-out issues for instance.
        m_entitiesManager.ProcessQueue(SpawnableEntitiesManager::CommandQueuePriority::High);
    }

    uint64_t SpawnableSystemComponent::AssignRootSpawnable(AZ::Data::Asset<Spawnable> rootSpawnable)
    {
        uint32_t generation = 0;

        if (m_rootSpawnableId == rootSpawnable.GetId())
        {
            AZ_TracePrintf("Spawnables", "Root spawnable wasn't updated because it's already assigned to the requested asset.");
            return m_rootSpawnableContainer.GetCurrentGeneration();
        }

        if (rootSpawnable.QueueLoad())
        {
            m_rootSpawnableId = rootSpawnable.GetId();

            // Suspend and resume processing in the container that completion calls aren't received until
            // everything has been setup to accept callbacks from the call.
            m_rootSpawnableContainer.Reset(rootSpawnable);
            generation = m_rootSpawnableContainer.GetCurrentGeneration();

            // Don't send out the alert that the root spawnable has been assigned until the spawnable itself is ready. The common
            // use case is for handlers to do something with the information in the spawnable before the entities get spawned.
            m_rootSpawnableContainer.Alert(
                [rootSpawnable](uint32_t generation)
                {
                    RootSpawnableNotificationBus::Broadcast(
                        &RootSpawnableNotificationBus::Events::OnRootSpawnableAssigned, AZStd::move(rootSpawnable), generation);
                }, SpawnableEntitiesContainer::CheckIfSpawnableIsLoaded::Yes);
            m_rootSpawnableContainer.SpawnAllEntities();
            m_rootSpawnableContainer.Alert(
                [newSpawnable = AZStd::move(rootSpawnable)](uint32_t generation)
                {
                    RootSpawnableNotificationBus::QueueBroadcast(
                        &RootSpawnableNotificationBus::Events::OnRootSpawnableReady, AZStd::move(newSpawnable), generation);
                });

            AZ_TracePrintf("Spawnables", "Root spawnable set to '%s' at generation %zu.\n", rootSpawnable.GetHint().c_str(), generation);     
        }
        else
        {
            AZ_Error("Spawnables", false, "Unable to queue root spawnable '%s' for loading.", rootSpawnable.GetHint().c_str());
        }

        return generation;
    }

    void SpawnableSystemComponent::ReleaseRootSpawnable()
    {
        if (m_rootSpawnableContainer.IsSet())
        {
            m_rootSpawnableContainer.Alert(
                [](uint32_t generation)
                {
                    RootSpawnableNotificationBus::QueueBroadcast(&RootSpawnableNotificationBus::Events::OnRootSpawnableReleased, generation);
                });
            m_rootSpawnableContainer.Clear();
        }
        m_rootSpawnableId = AZ::Data::AssetId();
    }

    void SpawnableSystemComponent::ProcessSpawnableQueue()
    {
        m_entitiesManager.ProcessQueue(
            SpawnableEntitiesManager::CommandQueuePriority::High | SpawnableEntitiesManager::CommandQueuePriority::Regular);
    }

    void SpawnableSystemComponent::OnRootSpawnableAssigned([[maybe_unused]] AZ::Data::Asset<Spawnable> rootSpawnable,
        [[maybe_unused]] uint32_t generation)
    {
        AZ_TracePrintf("Spawnables", "New root spawnable '%s' assigned (generation: %i).\n", rootSpawnable.GetHint().c_str(), generation);
    }

    void SpawnableSystemComponent::OnRootSpawnableReady(
        [[maybe_unused]] AZ::Data::Asset<Spawnable> rootSpawnable, [[maybe_unused]] uint32_t generation)
    {
        AZ_TracePrintf("Spawnables", "Entities from new root spawnable '%s' are ready (generation: %i).\n", rootSpawnable.GetHint().c_str(), generation);
    }

    void SpawnableSystemComponent::OnRootSpawnableReleased([[maybe_unused]] uint32_t generation)
    {
        AZ_TracePrintf("Spawnables", "Generation %i of the root spawnable has been released.\n", generation);
    }

    EntitySpawnTicket SpawnableSystemComponent::CreateSpawnTicket(const SpawnableAssetRef& spawnableAsset)
    {
        return EntitySpawnTicket(spawnableAsset.m_asset);
    }

    bool SpawnableSystemComponent::Spawn(
        EntitySpawnTicket spawnTicket,
        AZ::EntityId parentId,
        AZ::Vector3 translation,
        AZ::Vector3 rotation,
        float scale)
    {
        if (!spawnTicket.IsValid())
        {
            AZ_Error("Spawnables", false, "EntitySpawnTicket used for spawning is invalid.");
            return false;
        }

        auto preSpawnCB = [this, parentId, translation, rotation, scale](
                              [[maybe_unused]] EntitySpawnTicket::Id ticketId,
            SpawnableEntityContainerView view)
        {
            AZStd::lock_guard lock(m_mutex);

            AZ::Entity* rootEntity = *view.begin();
            TransformComponent* entityTransform = rootEntity->FindComponent<TransformComponent>();

            if (entityTransform)
            {
                AZ::Vector3 rotationCopy = rotation;
                AZ::Quaternion rotationQuat = AZ::Quaternion::CreateFromEulerAnglesDegrees(rotationCopy);

                TransformComponentConfiguration transformConfig;
                transformConfig.m_parentId = parentId;
                transformConfig.m_localTransform = AZ::Transform(translation, rotationQuat, scale);
                entityTransform->SetConfiguration(transformConfig);
            }
        };

        auto spawnCompleteCB =
            [this,
             spawnTicket]([[maybe_unused]] EntitySpawnTicket::Id ticketId, SpawnableConstEntityContainerView view)
        {
            AZStd::lock_guard lock(m_mutex);

            SpawnableResult spawnableResult;
            // SpawnTicket instance is cached instead of SpawnTicketId to simplify managing its lifecycle on Script Canvas
            // and to provide easier access to it in OnSpawnCompleted callback
            spawnableResult.m_spawnTicket = spawnTicket;
            spawnableResult.m_entityList.reserve(view.size());
            for (const AZ::Entity* entity : view)
            {
                spawnableResult.m_entityList.emplace_back(entity->GetId());
            }
            m_spawnedResults.push_back(spawnableResult);
        };

        SpawnAllEntitiesOptionalArgs optionalArgs;
        optionalArgs.m_preInsertionCallback = AZStd::move(preSpawnCB);
        optionalArgs.m_completionCallback = AZStd::move(spawnCompleteCB);
        SpawnableEntitiesInterface::Get()->SpawnAllEntities(spawnTicket, AZStd::move(optionalArgs));
        return true;
    }

    bool SpawnableSystemComponent::Despawn(EntitySpawnTicket spawnTicket)
    {
        if (!spawnTicket.IsValid())
        {
            AZ_Error("Spawnables", false, "EntitySpawnTicket used for despawning is invalid.");
            return false;
        }

        auto despawnCompleteCB = [this, spawnTicket]([[maybe_unused]] EntitySpawnTicket::Id ticketId)
        {
            AZStd::lock_guard lock(m_mutex);
            // SpawnTicket instance is cached instead of SpawnTicketId to simplify managing its lifecycle on Script Canvas
            // and to provide easier access to it in OnDespawn callback
            m_despawnedResults.push_back(spawnTicket);
        };

        DespawnAllEntitiesOptionalArgs optionalArgs;
        optionalArgs.m_completionCallback = AZStd::move(despawnCompleteCB);
        SpawnableEntitiesInterface::Get()->DespawnAllEntities(spawnTicket, AZStd::move(optionalArgs));
        return true;
    }

    void SpawnableSystemComponent::Activate()
    {
        // Register with AssetDatabase
        AZ_Assert(AZ::Data::AssetManager::IsReady(), "Spawnables can't be registered because the Asset Manager is not ready yet.");
        AZ::Data::AssetManager::Instance().RegisterHandler(&m_assetHandler, AZ::AzTypeInfo<Spawnable>::Uuid());

        // Register with AssetCatalog
        AZ::Data::AssetCatalogRequestBus::Broadcast(
            &AZ::Data::AssetCatalogRequestBus::Events::EnableCatalogForAsset, AZ::AzTypeInfo<Spawnable>::Uuid());
        AZ::Data::AssetCatalogRequestBus::Broadcast(
            &AZ::Data::AssetCatalogRequestBus::Events::AddExtension, Spawnable::FileExtension);

        // Register for the CriticalAssetsCompiled lifecycle event to trigger the loading of the root spawnable
        auto settingsRegistry = AZ::SettingsRegistry::Get();
        AZ_Assert(settingsRegistry, "Unable to change root spawnable callback because Settings Registry is not available.");

        auto LifecycleCallback = [this](AZStd::string_view, AZ::SettingsRegistryInterface::Type)
        {
            LoadRootSpawnableFromSettingsRegistry();
        };
        AZ::ComponentApplicationLifecycle::RegisterHandler(*settingsRegistry, m_criticalAssetsHandler,
            AZStd::move(LifecycleCallback), "CriticalAssetsCompiled");


        RootSpawnableNotificationBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
        SpawnableRequestsBus::Handler::BusConnect();

        m_registryChangeHandler = settingsRegistry->RegisterNotifier([this](AZStd::string_view path, AZ::SettingsRegistryInterface::Type /*type*/)
            {
                if (path.starts_with(RootSpawnableRegistryKey))
                {
                    LoadRootSpawnableFromSettingsRegistry();
                }
            });
    }

    void SpawnableSystemComponent::Deactivate()
    {
        m_spawnedResults.clear(); // clears any cached SpawnTickets that may remain so everything despawns
        m_spawnedResults.shrink_to_fit();

        m_despawnedResults.clear(); // clears any cached SpawnTickets that may remain so everything despawns
        m_despawnedResults.shrink_to_fit();

        ProcessSpawnableQueue();

        m_registryChangeHandler.Disconnect();

        AZ::TickBus::Handler::BusDisconnect();
        RootSpawnableNotificationBus::Handler::BusDisconnect();
        SpawnableRequestsBus::Handler::BusDisconnect();

        // Unregister Lifecycle event handler
        m_criticalAssetsHandler = {};

        if (m_rootSpawnableId.IsValid())
        {
            ReleaseRootSpawnable();

            // The SpawnableSystemComponent needs to guarantee there's no more processing left to do by the
            // entity manager before it can safely destroy it on shutdown, but also to make sure that are no
            // more calls to the callback registered to the root spawnable as that accesses this component.
            m_rootSpawnableContainer.Clear();
            SpawnableEntitiesManager::CommandQueueStatus queueStatus;
            do
            {
                queueStatus = m_entitiesManager.ProcessQueue(
                    SpawnableEntitiesManager::CommandQueuePriority::High | SpawnableEntitiesManager::CommandQueuePriority::Regular);
            } while (queueStatus == SpawnableEntitiesManager::CommandQueueStatus::HasCommandsLeft);
        }

        AZ::Data::AssetManager::Instance().UnregisterHandler(&m_assetHandler);
    }

    void SpawnableSystemComponent::LoadRootSpawnableFromSettingsRegistry()
    {
        auto registry = AZ::SettingsRegistry::Get();
        AZ_Assert(registry, "Unable to check for root spawnable because the Settings Registry is not available.");

        AZ::SettingsRegistryInterface::Type rootSpawnableKeyType = registry->GetType(RootSpawnableRegistryKey);
        if (rootSpawnableKeyType == AZ::SettingsRegistryInterface::Type::Object)
        {
            AZ::Data::Asset<Spawnable> rootSpawnable;
            if (registry->GetObject(rootSpawnable, RootSpawnableRegistryKey) && rootSpawnable.GetId().IsValid())
            {
                AssignRootSpawnable(AZStd::move(rootSpawnable));
            }
            else
            {
                AZ_Warning("Spawnables", false, "Root spawnable couldn't be queued for loading");
                ReleaseRootSpawnable();
            }
        }
        else if (rootSpawnableKeyType == AZ::SettingsRegistryInterface::Type::String)
        {
            AZStd::string rootSpawnableName;
            if (registry->Get(rootSpawnableName, RootSpawnableRegistryKey))
            {
                AZ::Data::AssetId rootSpawnableId;
                AZ::Data::AssetCatalogRequestBus::BroadcastResult(
                    rootSpawnableId, &AZ::Data::AssetCatalogRequestBus::Events::GetAssetIdByPath, rootSpawnableName.c_str(),
                    azrtti_typeid<Spawnable>(), false);
                if (rootSpawnableId.IsValid())
                {
                    AZ::Data::Asset<Spawnable> rootSpawnable = AZ::Data::Asset<Spawnable>(rootSpawnableId, azrtti_typeid<Spawnable>());
                    if (rootSpawnable.GetId().IsValid())
                    {
                        AssignRootSpawnable(AZStd::move(rootSpawnable));
                    }
                    else
                    {
                        AZ_Warning(
                            "Spawnables", false, "Root spawnable at '%s' couldn't be queued for loading.", rootSpawnableName.c_str());
                        ReleaseRootSpawnable();
                    }
                }
                else
                {
                    AZ_Warning(
                        "Spawnables", false, "Root spawnable with name '%s' wasn't found in the asset catalog.", rootSpawnableName.c_str());
                    ReleaseRootSpawnable();
                }
            }
        }
        else if (rootSpawnableKeyType == AZ::SettingsRegistryInterface::Type::NoType)
        {
            // [LYN-4146] - temporarily disabled
            /*AZ_Warning(
                "Spawnables", false,
                "No root spawnable assigned. The root spawnable can be assigned in the Settings Registry under the key '%s'.\n",
                RootSpawnableRegistryKey);*/
            ReleaseRootSpawnable();
        }
    }

    void SpawnableSystemComponent::ProcessSpawnedResults()
    {
        AZStd::vector<SpawnableResult> swappedSpawnedResults;
        {
            AZStd::lock_guard lock(m_mutex);
            swappedSpawnedResults.swap(m_spawnedResults);
        }

        for (const auto& spawnResult : swappedSpawnedResults)
        {
            if (spawnResult.m_entityList.empty())
            {
                continue;
            }
            SpawnableNotificationsBus::Event(
                spawnResult.m_spawnTicket.GetId(),
                &SpawnableNotifications::OnSpawn,
                spawnResult.m_spawnTicket,
                move(spawnResult.m_entityList));
        }
    }

    void SpawnableSystemComponent::ProcessDespawnedResults()
    {
        AZStd::vector<EntitySpawnTicket> swappedDespawnedResults;
        {
            AZStd::lock_guard lock(m_mutex);
            swappedDespawnedResults.swap(m_despawnedResults);
        }

        for (const auto& spawnTicket : swappedDespawnedResults)
        {
            SpawnableNotificationsBus::Event(spawnTicket.GetId(), &SpawnableNotifications::OnDespawn, spawnTicket);
        }
    }
} // namespace AzFramework
