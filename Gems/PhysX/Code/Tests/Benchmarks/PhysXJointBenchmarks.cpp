/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#ifdef HAVE_BENCHMARK
#include <benchmark/benchmark.h>

#include <AzTest/AzTest.h>
#include <AzCore/Math/Random.h>
#include <AzFramework/Physics/Common/PhysicsSimulatedBody.h>
#include <AzFramework/Physics/Configuration/RigidBodyConfiguration.h>
#include <AzFramework/Physics/Configuration/SimulatedBodyConfiguration.h>
#include <AzFramework/Physics/Configuration/StaticRigidBodyConfiguration.h>
#include <AzFramework/Physics/SimulatedBodies/StaticRigidBody.h>
#include <Benchmarks/PhysXBenchmarksUtilities.h>
#include <Benchmarks/PhysXBenchmarksCommon.h>

#include <PhysXTestCommon.h>
#include <PhysX/Joint/Configuration/PhysXJointConfiguration.h>

namespace PhysX::Benchmarks
{
    namespace JointConstants
    {
        //! Controls the simulation length of the test. 30secs at 60fps
        static const int GameFramesToSimulate = 1800;

        //! Decide if CCD should be on/off for the following tests
        static const bool CCDEnabled = false;

        //! used in BM_Joints_Swinging, applys an impluse to the rigid body of the joint every {x} frames
        static const AZ::u32 ImpulseFreq = GameFramesToSimulate / 6;
        static const float ImpulseForceMax = 2.5f;
        static const float ImpulseForceMin = 1.0f;

        //! used in BM_Joints_Snake
        static const float SnakeSegmentLength = 2.5f;

        //! Constant seed to use with random number generation
        static const long long RandGenSeed = 74111105110116; //(Number generated by concatenating 'Joint' ascii character codes (74 111 105 110 116)

        //! Settings used to setup each benchmark
        namespace BenchmarkSettings
        {
            //! Values passed to benchmark to select the number of rigid bodies to spawn during each test
            //! Current values will run tests between StartRange to EndRange (inclusive), multiplying by RangeMultiplier each step.
            static const int StartRange = 8;
            static const int EndRange = 8192;
            static const int RangeMultipler = 2;

            //! Number of iterations for each test
            static const int NumIterations = 3;
        } // namespace BenchmarkSettings

        //! Default values used if no Generator functions are passed to Utils::CreateJoints
        namespace CreateJointDefaults
        {
            //default spawn XY positions used for both parent and child if no position generator has been provided
            static const float DefaultXPosition = 3.0f;
            static const float DefaultYPosition = 1.0f;

            //Range of motion for the joint
            static const float UpperLimit = 90.0f;
            static const float LowerLimit = -90.0f;

            static const float ColliderRadius = 0.5f;
        } // namespace SpawnDefaults

        namespace JointSettings
        {
            static const float ParentXPosSpacing = 3.0f;
            static const float ChildStartYOffsetPos = 2.0f;

            //Range of motion for the swinging joint test
            static const float SwingingJointUpperLimit = 90.0f;
            static const float SwingingJointLowerLimit = -180.0f;

        } // namespace JointSettings
    } // namespace JointConstants

    namespace Utils
    {
        //! Structure to hold references to the Joint and the 2 attached bodies
        //! used with Utils::CreateJoints
        struct JointGroup
        {
            AzPhysics::RigidBody* m_parent;
            AzPhysics::SimulatedBody* m_child;
            AzPhysics::JointHandle m_jointHandle;
        };
        //! Structure to hold the upper and lower twist limits
        //! used with Utils::CreateJoints GenerateTwistLimitsFuncPtr
        struct TwistLimits
        {
            TwistLimits(float upper, float lower)
                : m_upperLimit(upper)
                , m_lowerLimit(lower)
            {
            }
            float m_upperLimit;
            float m_lowerLimit;
        };

        //!Function pointers to allow customization of Utils::CreateJoints for specific tests
        using GenerateTwistLimitsFuncPtr = AZStd::function<TwistLimits(int)>;

        //! Helper function to add the required number of joints to the provided world
        //! @param numJoints, the requested number of joints to spawn
        //! @param scene, the physics scene to spawn the joints and their world bodies into
        //! @param parentPositionGenerator, [optional] function pointer to allow caller to pick the spawn position of the parent body
        //! @param childPositionGenerator, [optional] function pointer to allow caller to pick the spawn position of the child body
        //! @param GenerateTwistLimitsFuncPtr, [optional] function pointer to allow caller to pick the twist limits of the joint
        AZStd::vector<JointGroup> CreateJoints(int numJoints,
            AzPhysics::Scene* scene,
            GenerateSpawnPositionFuncPtr* parentPositionGenerator = nullptr,
            GenerateSpawnPositionFuncPtr* childPositionGenerator = nullptr,
            GenerateTwistLimitsFuncPtr* twistLimitsGenerator = nullptr)
        {
            AZStd::vector<JointGroup> joints;

            auto shapeConfiguration = AZStd::make_shared<Physics::SphereShapeConfiguration>(JointConstants::CreateJointDefaults::ColliderRadius);
            AzPhysics::ShapeColliderPair shapeColliderConfig(AZStd::make_shared<Physics::ColliderConfiguration>(), shapeConfiguration);
            for (int i = 0; i < numJoints; i++)
            {
                JointGroup newJoint;

                AzPhysics::RigidBodyConfiguration rigidBodyConfig;
                if (parentPositionGenerator)
                {
                    rigidBodyConfig.m_position = (*parentPositionGenerator)(i);
                }
                else
                {
                    rigidBodyConfig.m_position = AZ::Vector3(JointConstants::CreateJointDefaults::DefaultXPosition * i, 0.0f, 0.0f);
                }
                rigidBodyConfig.m_colliderAndShapeData = shapeColliderConfig;
                AzPhysics::SimulatedBodyHandle rigidBodyHandle = scene->AddSimulatedBody(&rigidBodyConfig);
                newJoint.m_parent = azdynamic_cast<AzPhysics::RigidBody*>(scene->GetSimulatedBodyFromHandle(rigidBodyHandle));

                //fix the child
                AzPhysics::StaticRigidBodyConfiguration staticRigidBodyConfig;
                if (childPositionGenerator)
                {
                    staticRigidBodyConfig.m_position = (*childPositionGenerator)(i);
                }
                else
                {
                    staticRigidBodyConfig.m_position = AZ::Vector3(
                        JointConstants::CreateJointDefaults::DefaultXPosition * i,
                        JointConstants::CreateJointDefaults::DefaultYPosition,
                        0.0f);
                }
                staticRigidBodyConfig.m_colliderAndShapeData = shapeColliderConfig;

                AzPhysics::SimulatedBodyHandle staticRigidBodyHandle = scene->AddSimulatedBody(&staticRigidBodyConfig);
                newJoint.m_child = scene->GetSimulatedBodyFromHandle(staticRigidBodyHandle);

                PhysX::D6JointLimitConfiguration config;

                TwistLimits limits(JointConstants::CreateJointDefaults::UpperLimit, JointConstants::CreateJointDefaults::LowerLimit);
                if (twistLimitsGenerator)
                {
                    limits = (*twistLimitsGenerator)(i);
                }
                config.m_twistLimitUpper = limits.m_upperLimit;
                config.m_twistLimitLower = limits.m_lowerLimit;
                config.m_swingLimitY = 1.0f;
                config.m_swingLimitZ = 1.0f;
                newJoint.m_jointHandle = scene->AddJoint(&config, newJoint.m_parent->m_bodyHandle, newJoint.m_child->m_bodyHandle);
                joints.emplace_back(AZStd::move(newJoint));
            }

            return AZStd::move(joints);
        }
    } // namespace Utils

    //! Joint performance fixture
    //! Will create the world for the benchmark to run in
    class PhysXJointBenchmarkFixture
        : public PhysXBaseBenchmarkFixture
    {
        void internalSetUp()
        {
            PhysXBaseBenchmarkFixture::SetUpInternal();
        }

        void internalTearDown()
        {
            PhysXBaseBenchmarkFixture::TearDownInternal();
        }

    public:
        void SetUp(const benchmark::State&) override
        {
            internalSetUp();
        }
        void SetUp(benchmark::State&) override
        {
            internalSetUp();
        }

        void TearDown(const benchmark::State&) override
        {
            internalTearDown();
        }
        void TearDown(benchmark::State&) override
        {
            internalTearDown();
        }

        // PhysXBaseBenchmarkFixture Interface ---------
        AzPhysics::SceneConfiguration GetDefaultSceneConfiguration() override
        {
            AzPhysics::SceneConfiguration sceneConfig = AzPhysics::SceneConfiguration::CreateDefault();
            return sceneConfig;
        }
        // PhysXBaseBenchmarkFixture Interface ---------
    };

    //! BM_Joints_AtRest - This test will spawn the requested number of joints
    //! Each joint will be between a static rigid body and a rigid body
    //! The rigid body will spawn next to the static rigid body and swing down to a stop below the static rigid body and come to a rest
    BENCHMARK_DEFINE_F(PhysXJointBenchmarkFixture, BM_Joints_AtRest)(benchmark::State& state)
    {
        //just use the default setting for the joint configuration
        Utils::GenerateSpawnPositionFuncPtr parentPosGenerator = [](int idx) -> AZ::Vector3 {
            return AZ::Vector3(JointConstants::JointSettings::ParentXPosSpacing * idx, 0.0f, 0.0f);
        };
        Utils::GenerateSpawnPositionFuncPtr childPosGenerator = [parentPosGenerator](int idx) -> AZ::Vector3 {
            AZ::Vector3 position = parentPosGenerator(idx);
            position.SetY(position.GetY() + JointConstants::JointSettings::ChildStartYOffsetPos);
            return position;
        };

        AZStd::vector<Utils::JointGroup> joinGroups = Utils::CreateJoints(aznumeric_cast<int>(state.range(0)), m_defaultScene,
            &parentPosGenerator, &childPosGenerator);

        //setup the sub tick tracker
        Utils::PrePostSimulationEventHandler subTickTracker;
        subTickTracker.Start(m_defaultScene);

        //setup the frame timer tracker
        Types::TimeList tickTimes;
        for ([[maybe_unused]] auto _ : state)
        {
            for (AZ::u32 i = 0; i < JointConstants::GameFramesToSimulate; i++)
            {
                auto start = AZStd::chrono::steady_clock::now();
                StepScene1Tick(DefaultTimeStep);

                //time each physics tick and store it to analyze
                auto tickElapsedMilliseconds = Types::double_milliseconds(AZStd::chrono::steady_clock::now() - start);
                tickTimes.emplace_back(tickElapsedMilliseconds.count());
            }
        }
        subTickTracker.Stop();

        for (const auto& jointGroup : joinGroups)
        {
            m_defaultScene->RemoveJoint(jointGroup.m_jointHandle);
        }

        //sort the frame times and get the P50, P90, P99 percentiles
        Utils::ReportFramePercentileCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
        Utils::ReportFrameStandardDeviationAndMeanCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
    }

    //! BM_Joints_Swinging - This test will spawn the requested number of joints
    //! Each joint will be between a static rigid body and a rigid body
    //! The rigid body will spawn next to the static rigid body and swing down
    //! Every few frames an impulse will be applied to push the swinging rigid body in its direction of motion
    BENCHMARK_DEFINE_F(PhysXJointBenchmarkFixture, BM_Joints_Swinging)(benchmark::State& state)
    {
        AZ::SimpleLcgRandom rand;
        rand.SetSeed(JointConstants::RandGenSeed);

        //just use the default setting for the joint configuration
        Utils::GenerateSpawnPositionFuncPtr parentPosGenerator = [](int idx) -> AZ::Vector3 {
            return AZ::Vector3(JointConstants::JointSettings::ParentXPosSpacing * idx, 0.0f, 0.0f);
        };
        Utils::GenerateSpawnPositionFuncPtr childPosGenerator = [parentPosGenerator](int idx) -> AZ::Vector3 {
            AZ::Vector3 position = parentPosGenerator(idx);
            position.SetY(position.GetY() + JointConstants::JointSettings::ChildStartYOffsetPos);
            return position;
        };
        Utils::GenerateTwistLimitsFuncPtr twistGenerator = []([[maybe_unused]] int idx) -> Utils::TwistLimits {
            return Utils::TwistLimits(JointConstants::JointSettings::SwingingJointUpperLimit, JointConstants::JointSettings::SwingingJointLowerLimit);
        };

        AZStd::vector<Utils::JointGroup> joinGroups = Utils::CreateJoints(aznumeric_cast<int>(state.range(0)), m_defaultScene,
            &parentPosGenerator, &childPosGenerator, &twistGenerator);

        //setup the sub tick tracker
        Utils::PrePostSimulationEventHandler subTickTracker;
        subTickTracker.Start(m_defaultScene);

        //setup the frame timer tracker
        Types::TimeList tickTimes;
        for ([[maybe_unused]] auto _ : state)
        {
            for (AZ::u32 i = 0; i < JointConstants::GameFramesToSimulate; i++)
            {
                //every few frames apply an impluse to each joint
                if ((i % JointConstants::ImpulseFreq) == 0)
                {
                    for (auto& jointGroup : joinGroups)
                    {
                        //apply a randomized impulse to each joint
                        const float impulse = JointConstants::ImpulseForceMin + rand.GetRandomFloat() * JointConstants::ImpulseForceMax;
                        const AZ::Vector3 movementDir = jointGroup.m_parent->GetLinearVelocity().GetNormalizedSafe();
                        const AZ::Vector3 impulseDir = AZ::Vector3::CreateAxisY(impulse) * movementDir;
                        if (!impulseDir.IsZero())
                        {
                            jointGroup.m_parent->ApplyLinearImpulse(impulseDir);
                        }
                    }
                }

                auto start = AZStd::chrono::steady_clock::now();
                StepScene1Tick(DefaultTimeStep);

                //time each physics tick and store it to analyze
                auto tickElapsedMilliseconds = Types::double_milliseconds(AZStd::chrono::steady_clock::now() - start);
                tickTimes.emplace_back(tickElapsedMilliseconds.count());
            }
        }
        subTickTracker.Stop();

        for (const auto& jointGroup : joinGroups)
        {
            m_defaultScene->RemoveJoint(jointGroup.m_jointHandle);
        }

        //sort the frame times and get the P50, P90, P99 percentiles
        Utils::ReportFramePercentileCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
        Utils::ReportFrameStandardDeviationAndMeanCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
    }

    //! BM_Joints_Snake - This test will spawn the requested number of joints and link them together like a snake
    //! The first joint will be between a static rigid body and a rigid body
    //! All other joints are between two rigid bodies
    BENCHMARK_DEFINE_F(PhysXJointBenchmarkFixture, BM_Joints_Snake)(benchmark::State& state)
    {
        const int numSegments = aznumeric_cast<int>(state.range(0));
        const int bodyType = aznumeric_cast<int>(state.range(1));

        //create the collider shape config to use on the whole snake
        auto snakePartShapeConfiguration = AZStd::make_shared<Physics::SphereShapeConfiguration>(JointConstants::CreateJointDefaults::ColliderRadius);

        //create the had of the snake this is the only static part.
        AzPhysics::StaticRigidBodyConfiguration snakeHeadBodyConfig;
        snakeHeadBodyConfig.m_position = AZ::Vector3::CreateZero();
        snakeHeadBodyConfig.m_colliderAndShapeData = AzPhysics::ShapeColliderPair(
            AZStd::make_shared<Physics::ColliderConfiguration>(), snakePartShapeConfiguration);

        AzPhysics::SimulatedBody* snakeHead = nullptr;
        if (auto* sceneInterface = AZ::Interface<AzPhysics::SceneInterface>::Get())
        {
            AzPhysics::SimulatedBodyHandle snakeHeadHandle = sceneInterface->AddSimulatedBody(m_testSceneHandle, &snakeHeadBodyConfig);
            snakeHead = sceneInterface->GetSimulatedBodyFromHandle(m_testSceneHandle, snakeHeadHandle);
        }

        //create the body
        Utils::GenerateColliderFuncPtr colliderGenerator = [&snakePartShapeConfiguration]([[maybe_unused]] int idx) -> auto
        {
            return snakePartShapeConfiguration;
        };
        Utils::GenerateSpawnPositionFuncPtr posGenerator = [](int idx) -> auto
        {
            return AZ::Vector3(0.0f, JointConstants::SnakeSegmentLength + JointConstants::SnakeSegmentLength * idx, 0.0f);
        };

        Utils::BenchmarkRigidBodies snakeRigidBodyHandles = Utils::CreateRigidBodies(
            numSegments, GetDefaultSceneHandle(), JointConstants::CCDEnabled, bodyType, &colliderGenerator, &posGenerator);
        AZStd::vector<AzPhysics::RigidBody*> snakeRigidBodies;
        snakeRigidBodies.reserve(numSegments);
        snakeRigidBodies = Utils::GetRigidBodiesFromHandles(m_defaultScene, snakeRigidBodyHandles);

        //build the snake
        AZStd::vector<AzPhysics::JointHandle> jointHandles;
        PhysX::D6JointLimitConfiguration config;
        config.m_twistLimitUpper = JointConstants::CreateJointDefaults::UpperLimit;
        config.m_twistLimitLower = JointConstants::CreateJointDefaults::LowerLimit;
        config.m_swingLimitY = 1.0f;
        config.m_swingLimitZ = 1.0f;
        //build the head
        jointHandles.emplace_back(m_defaultScene->AddJoint(&config, snakeRigidBodies[0]->m_bodyHandle, snakeHead->m_bodyHandle));
        for (size_t i = 0; (i+1) < snakeRigidBodies.size(); i++)
        {
            jointHandles.emplace_back(
                m_defaultScene->AddJoint(&config, snakeRigidBodies[i + 1]->m_bodyHandle, snakeRigidBodies[i]->m_bodyHandle));
        }

        //setup the sub tick tracker
        Utils::PrePostSimulationEventHandler subTickTracker;
        subTickTracker.Start(m_defaultScene);

        //setup the frame timer tracker
        Types::TimeList tickTimes;
        for ([[maybe_unused]] auto _ : state)
        {
            for (AZ::u32 i = 0; i < JointConstants::GameFramesToSimulate; i++)
            {
                auto start = AZStd::chrono::steady_clock::now();
                StepScene1Tick(DefaultTimeStep);

                //time each physics tick and store it to analyze
                auto tickElapsedMilliseconds = Types::double_milliseconds(AZStd::chrono::steady_clock::now() - start);
                tickTimes.emplace_back(tickElapsedMilliseconds.count());
            }
        }
        subTickTracker.Stop();

        if (auto handlesList = AZStd::get_if<AzPhysics::SimulatedBodyHandleList>(&snakeRigidBodyHandles))
        {
            m_defaultScene->RemoveSimulatedBodies(*handlesList);
        }

        for (const auto& jointHandle : jointHandles)
        {
            m_defaultScene->RemoveJoint(jointHandle);
        }

        AZStd::visit(
            [](auto& rigidBodies)
            {
                rigidBodies.clear();
            },
            snakeRigidBodyHandles);

        //sort the frame times and get the P50, P90, P99 percentiles
        Utils::ReportFramePercentileCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
        Utils::ReportFrameStandardDeviationAndMeanCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
    }

    BENCHMARK_REGISTER_F(PhysXJointBenchmarkFixture, BM_Joints_AtRest)
        ->RangeMultiplier(JointConstants::BenchmarkSettings::RangeMultipler)
        ->Range(JointConstants::BenchmarkSettings::StartRange, JointConstants::BenchmarkSettings::EndRange)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(JointConstants::BenchmarkSettings::NumIterations)
        ;

    BENCHMARK_REGISTER_F(PhysXJointBenchmarkFixture, BM_Joints_Swinging)
         ->RangeMultiplier(JointConstants::BenchmarkSettings::RangeMultipler)
         ->Range(JointConstants::BenchmarkSettings::StartRange, JointConstants::BenchmarkSettings::EndRange)
         ->Unit(benchmark::kMillisecond)
         ->Iterations(JointConstants::BenchmarkSettings::NumIterations)
         ;

       BENCHMARK_REGISTER_F(PhysXJointBenchmarkFixture, BM_Joints_Snake)
         ->RangeMultiplier(JointConstants::BenchmarkSettings::RangeMultipler)
         ->Ranges({ { JointConstants::BenchmarkSettings::StartRange, JointConstants::BenchmarkSettings::EndRange },
                   { RigidBodyApiObject, RigidBodyEntity } })
         ->Unit(benchmark::kMillisecond)
         ->Iterations(JointConstants::BenchmarkSettings::NumIterations)
         ;

} // namespace PhysX::Benchmarks
#endif
