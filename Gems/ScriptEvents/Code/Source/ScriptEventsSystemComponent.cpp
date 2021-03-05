/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include "precompiled.h"
#include "ScriptEventsSystemComponent.h"

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Math/Color.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/sort.h>

#include <AzFramework/Asset/GenericAssetHandler.h>

#include <ScriptEvents/ScriptEventsAssetRef.h>
#include <ScriptEvents/ScriptEventDefinition.h>
#include <ScriptEvents/ScriptEventFundamentalTypes.h>

namespace ScriptEvents
{
    void ScriptEventsSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        using namespace ScriptEvents;

        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<ScriptEventsSystemComponent, AZ::Component>()
                ->Version(1)
                // ScriptEvents avoids a use dependency on the AssetBuilderSDK. Therefore the Crc is used directly to register this component with the Gem builder
                ->Attribute(AZ::Edit::Attributes::SystemComponentTags, AZStd::vector<AZ::Crc32>({ AZ_CRC("AssetBuilder", 0xc739c7d7) }));
                ;
        }

        ScriptEventData::VersionedProperty::Reflect(context);
        Parameter::Reflect(context);
        Method::Reflect(context);
        ScriptEvent::Reflect(context);

        ScriptEvents::ScriptEventsAsset::Reflect(context);
        ScriptEvents::ScriptEventsAssetRef::Reflect(context);
        ScriptEvents::ScriptEventsAssetPtr::Reflect(context);
    }

    void ScriptEventsSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("ScriptEventsService", 0x6897c23b));
    }

    void ScriptEventsSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("ScriptEventsService", 0x6897c23b));
    }

    void ScriptEventsSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC("AssetDatabaseService", 0x3abf5601));
    }

    void ScriptEventsSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        AZ_UNUSED(dependent);
    }

    void ScriptEventsSystemComponent::Init()
    {
    }

    void ScriptEventsSystemComponent::Activate()
    {
        ScriptEventsSystemComponentImpl* moduleConfiguration = nullptr;
        ScriptEventModuleConfigurationRequestBus::BroadcastResult(moduleConfiguration, &ScriptEventModuleConfigurationRequests::GetSystemComponentImpl);
        if (moduleConfiguration)
        {
            moduleConfiguration->RegisterAssetHandler();
        }
    }

    void ScriptEventsSystemComponent::Deactivate()
    {
        for (auto& asset : m_scriptEvents)
        {
            asset.second.reset();
        }

        m_scriptEvents.clear();

        ScriptEventsSystemComponentImpl* moduleConfiguration = nullptr;
        ScriptEventModuleConfigurationRequestBus::BroadcastResult(moduleConfiguration, &ScriptEventModuleConfigurationRequests::GetSystemComponentImpl);
        if (moduleConfiguration)
        {
            moduleConfiguration->UnregisterAssetHandler();
        }

    }

    void ScriptEventsSystemComponentRuntimeImpl::RegisterAssetHandler()
    {
        AZ::Data::AssetType assetType(azrtti_typeid<ScriptEvents::ScriptEventsAsset>());
        if (AZ::Data::AssetManager::Instance().GetHandler(assetType))
        {
            return; // Asset Type already handled
        }

        m_assetHandler = AZStd::make_unique<ScriptEventAssetRuntimeHandler>(ScriptEvents::ScriptEventsAsset::GetDisplayName(),
            ScriptEvents::ScriptEventsAsset::GetGroup(), ScriptEvents::ScriptEventsAsset::GetFileExtension(),
            AZ::AzTypeInfo<ScriptEvents::ScriptEventsSystemComponent>::Uuid());

        AZ::Data::AssetManager::Instance().RegisterHandler(m_assetHandler.get(), assetType);

        // Use AssetCatalog service to register ScriptCanvas asset type and extension
        AZ::Data::AssetCatalogRequestBus::Broadcast(&AZ::Data::AssetCatalogRequests::AddAssetType, assetType);
        AZ::Data::AssetCatalogRequestBus::Broadcast(&AZ::Data::AssetCatalogRequests::EnableCatalogForAsset, assetType);
        AZ::Data::AssetCatalogRequestBus::Broadcast(&AZ::Data::AssetCatalogRequests::AddExtension,
            ScriptEvents::ScriptEventsAsset::GetFileExtension());
    }

    void ScriptEventsSystemComponentRuntimeImpl::UnregisterAssetHandler()
    {
        AZ::Data::AssetManager::Instance().UnregisterHandler(m_assetHandler.get());
        m_assetHandler.reset();
    }

}
