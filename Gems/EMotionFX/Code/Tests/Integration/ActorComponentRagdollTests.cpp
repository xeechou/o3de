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

#include <AzCore/Component/Entity.h>
#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Physics/World.h>
#include <Integration/Components/ActorComponent.h>
#include <EMotionFX/Source/Allocators.h>
#include <EMotionFX/Source/Node.h>
#include <Tests/Integration/EntityComponentFixture.h>
#include <Tests/TestAssetCode/JackActor.h>
#include <Tests/TestAssetCode/TestActorAssets.h>
#include <Tests/TestAssetCode/ActorFactory.h>
#include <Tests/Mocks/PhysicsRagdoll.h>

namespace EMotionFX
{
    class TestRagdollPhysicsRequestHandler
        : public AzFramework::RagdollPhysicsRequestBus::Handler
    {
    public:
        TestRagdollPhysicsRequestHandler(Physics::Ragdoll* ragdoll, const AZ::EntityId& entityId)
        {
            m_ragdoll = ragdoll;
            AzFramework::RagdollPhysicsRequestBus::Handler::BusConnect(entityId);
        }

        ~TestRagdollPhysicsRequestHandler()
        {
            AzFramework::RagdollPhysicsRequestBus::Handler::BusDisconnect();
        }

        void EnableSimulation([[maybe_unused]] const Physics::RagdollState& initialState) override {}
        void EnableSimulationQueued([[maybe_unused]]const Physics::RagdollState& initialState) override {}
        void DisableSimulation() override {}
        void DisableSimulationQueued() override {}

        Physics::Ragdoll* GetRagdoll() override { return m_ragdoll; }

        void GetState([[maybe_unused]] Physics::RagdollState& ragdollState) const override {}
        void SetState([[maybe_unused]] const Physics::RagdollState& ragdollState) override {}
        void SetStateQueued([[maybe_unused]] const Physics::RagdollState& ragdollState) override {}

        void GetNodeState([[maybe_unused]] size_t nodeIndex, [[maybe_unused]] Physics::RagdollNodeState& nodeState) const override {}
        void SetNodeState([[maybe_unused]] size_t nodeIndex, [[maybe_unused]] const Physics::RagdollNodeState& nodeState) override {}

        Physics::RagdollNode* GetNode([[maybe_unused]] size_t nodeIndex) const override { return nullptr; }

        void EnterRagdoll() override {}
        void ExitRagdoll() override {}

    private:
        Physics::Ragdoll* m_ragdoll = nullptr;
    };

    TEST_F(EntityComponentFixture, ActorComponent_ActivateRagdoll)
    {
        AZ::EntityId entityId(740216387);
        AZ::Crc32 worldId(174592);

        TestRagdoll testRagdoll;
        TestRagdollPhysicsRequestHandler ragdollPhysicsRequestHandler(&testRagdoll, entityId);

        EXPECT_CALL(testRagdoll, GetState(::testing::_)).Times(::testing::AnyNumber());
        EXPECT_CALL(testRagdoll, GetNumNodes()).WillRepeatedly(::testing::Return(1));
        EXPECT_CALL(testRagdoll, IsSimulated()).WillRepeatedly(::testing::Return(true));
        EXPECT_CALL(testRagdoll, GetWorldId()).WillRepeatedly(::testing::Return(worldId));
        EXPECT_CALL(testRagdoll, GetEntityId()).WillRepeatedly(::testing::Return(entityId));
        EXPECT_CALL(testRagdoll, GetPosition()).WillRepeatedly(::testing::Return(AZ::Vector3::CreateZero()));
        EXPECT_CALL(testRagdoll, GetOrientation()).WillRepeatedly(::testing::Return(AZ::Quaternion::CreateIdentity()));

        auto gameEntity = AZStd::make_unique<AZ::Entity>();
        gameEntity->SetId(entityId);

        auto transformComponent = gameEntity->CreateComponent<AzFramework::TransformComponent>();
        auto actorComponent = gameEntity->CreateComponent<Integration::ActorComponent>();

        gameEntity->Init();
        gameEntity->Activate();

        AZ::Data::AssetId actorAssetId("{5060227D-B6F4-422E-BF82-41AAC5F228A5}");
        AZStd::unique_ptr<Actor> actor = ActorFactory::CreateAndInit<JackNoMeshesActor>();
        AZ::Data::Asset<Integration::ActorAsset> actorAsset = TestActorAssets::GetAssetFromActor(actorAssetId, AZStd::move(actor));

        actorComponent->OnAssetReady(actorAsset);
        EXPECT_FALSE(actorComponent->IsWorldNotificationBusConnected(worldId))
            << "World notification bus should not be connected directly after creating the actor instance.";

        actorComponent->OnRagdollActivated();
        EXPECT_TRUE(actorComponent->IsWorldNotificationBusConnected(worldId))
            << "World notification bus should be connected after activating the ragdoll.";

        actorComponent->OnRagdollDeactivated();
        EXPECT_FALSE(actorComponent->IsWorldNotificationBusConnected(worldId))
            << "World notification bus should not be connected after deactivating the ragdoll.";

        actorComponent->OnRagdollActivated();
        EXPECT_TRUE(actorComponent->IsWorldNotificationBusConnected(worldId))
            << "World notification bus should be connected after activating the ragdoll.";

        gameEntity->Deactivate();
        EXPECT_FALSE(actorComponent->IsWorldNotificationBusConnected(worldId))
            << "World notification bus should not be connected anymore after deactivating the entire entity.";
    }
}
