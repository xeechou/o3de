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

#include <AchievementsSystemComponent.h>
#include <Achievements/AchievementNotificationBus.h>
#include <AzCore/Component/TickBus.h>

namespace Achievements
{
    class AchievementNotificationBusBehaviorHandler
        : public AchievementNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        AZ_EBUS_BEHAVIOR_BINDER(AchievementNotificationBusBehaviorHandler, "{33DFB6A3-434B-4341-B603-5F387D1CACFE}", AZ::SystemAllocator
            , OnAchievementUnlocked
            , OnAchievementDetailsQueried
            , OnAchievementUnlockRequested
        );

        ////////////////////////////////////////////////////////////////////////////////////////////
        void OnAchievementUnlocked(const AZStd::string& achievementId, const AzFramework::LocalUserId& localUserId) override
        {
            Call(FN_OnAchievementUnlocked, achievementId, localUserId);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        void OnAchievementUnlockRequested(const AZStd::string& achievementId, const AzFramework::LocalUserId& localUserId) override
        {
            Call(FN_OnAchievementUnlockRequested, achievementId, localUserId);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        void OnAchievementDetailsQueried(const AzFramework::LocalUserId& localUserId, const AchievementDetails& achievementDetails) override
        {
            Call(FN_OnAchievementDetailsQueried, localUserId, achievementDetails);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementDetails::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<AchievementDetails>()
                ->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<AchievementDetails>("AchievementDetails", "Struct to hold platform agnostic achievement details for query results")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            
            behaviorContext->Class<AchievementDetails>()
                ->Constructor<AchievementDetails&>()
                ->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::Preview)
                ->Attribute(AZ::Script::Attributes::Storage, AZ::Script::Attributes::StorageType::Value)
                ->Property("id", BehaviorValueProperty(&AchievementDetails::id))
                ->Property("name", BehaviorValueProperty(&AchievementDetails::name))
                ->Property("desc", BehaviorValueProperty(&AchievementDetails::desc))
                ->Property("rewardValue", BehaviorValueProperty(&AchievementDetails::rewardValue))
                ->Property("secret", BehaviorValueProperty(&AchievementDetails::secret))
                ->Property("currentProgress", BehaviorValueProperty(&AchievementDetails::currentProgress))
                ->Property("unlocked", BehaviorValueProperty(&AchievementDetails::unlocked));
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    AchievementDetails::AchievementDetails() :
        id(0),
        name(""),
        desc(""),
        rewardValue(0),
        currentProgress(0),
        unlocked(false),
        secret(false)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<AchievementsSystemComponent, AZ::Component>()
                ->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<AchievementsSystemComponent>("Achievements", "Platform agnostic interface for retrieving achievement details and unlocking achievements")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<AchievementNotificationBus>("AchievementNotificationBus")
                ->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::All)
                ->Handler<AchievementNotificationBusBehaviorHandler>();

            using UnlockAchievementParams = AchievementRequests::UnlockAchievementParams;
            behaviorContext->Class<UnlockAchievementParams>()
                ->Attribute(AZ::Script::Attributes::Storage, AZ::Script::Attributes::StorageType::Value)
                ->Property("achievementId", BehaviorValueProperty(&UnlockAchievementParams::achievementId))
                ->Property("localUserId", BehaviorValueProperty(&UnlockAchievementParams::localUserId))
                ->Property("percentage", BehaviorValueProperty(&UnlockAchievementParams::percentage));

            using QueryAchievementParams = AchievementRequests::QueryAchievementParams;
            behaviorContext->Class<QueryAchievementParams>()
                ->Attribute(AZ::Script::Attributes::Storage, AZ::Script::Attributes::StorageType::Value)
                ->Property("achievementId", BehaviorValueProperty(&QueryAchievementParams::achievementId))
                ->Property("localUserId", BehaviorValueProperty(&QueryAchievementParams::localUserId));

            behaviorContext->EBus<AchievementRequestBus>("AchievementRequestBus")
                ->Attribute(AZ::Script::Attributes::ExcludeFrom, AZ::Script::Attributes::ExcludeFlags::All)
                ->Attribute(AZ::Script::Attributes::Category, "Achievements")
                ->Event("UnlockAchievement", &AchievementRequestBus::Events::UnlockAchievement)
                ->Event("QueryAchievementDetails", &AchievementRequestBus::Events::QueryAchievementDetails);
        }

        AchievementDetails::Reflect(context);
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("AchievementsService"));
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("AchievementsService"));
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::Activate()
    {
        m_pimpl.reset(Implementation::Create(*this));
        AchievementRequestBus::Handler::BusConnect();
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::Deactivate()
    {
        AchievementRequestBus::Handler::BusDisconnect();
        m_pimpl.reset();
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::UnlockAchievement(const UnlockAchievementParams& params)
    {
        AchievementNotificationBus::Broadcast(&AchievementNotifications::OnAchievementUnlockRequested, params.achievementId, params.localUserId);
        AZ_Printf("Achievements", "Unlock Achievement request for localuserId %s, achievement ID %s", AzFramework::LocalUserIdToString(params.localUserId).c_str(), params.achievementId.c_str());
        if (m_pimpl)
        {
            m_pimpl->UnlockAchievement(params);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::QueryAchievementDetails(const QueryAchievementParams& params)
    {
        AZ_Printf("Achievements", "Query Achievement request for localuserId %s, achievement ID %s", AzFramework::LocalUserIdToString(params.localUserId).c_str(), params.achievementId.c_str());
        if (m_pimpl)
        {
            m_pimpl->QueryAchievementDetails(params);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    AchievementsSystemComponent::Implementation::Implementation(AchievementsSystemComponent& achievementSystemComponent)
        : m_achievementsSystemComponent(achievementSystemComponent)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    AchievementsSystemComponent::Implementation::~Implementation()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::Implementation::OnUnlockAchievementComplete(const UnlockAchievementParams& params)
    {
        AZ::TickBus::QueueFunction([params]()
        {
            if (params.OnAchievementUnlockCallback)
            {
                params.OnAchievementUnlockCallback(params.localUserId, params.achievementId);
            }
            AchievementNotificationBus::Broadcast(&AchievementNotifications::OnAchievementUnlocked, params.achievementId, params.localUserId);
        });
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    void AchievementsSystemComponent::Implementation::OnQueryAchievementDetailsComplete(const QueryAchievementParams& params, const AchievementDetails& details)
    {
        AZ::TickBus::QueueFunction([params, details]()
        {
            if (params.OnAchievementDetailsQueriedCallback)
            {
                params.OnAchievementDetailsQueriedCallback(params.localUserId, details);
            }
            AchievementNotificationBus::Broadcast(&AchievementNotifications::OnAchievementDetailsQueried, params.localUserId, details);
        });
    }
}
