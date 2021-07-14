/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.
 * 
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#ifdef HAVE_BENCHMARK
#include <benchmark/benchmark.h>

#include <AzTest/AzTest.h>
#include <AzTest/Utils.h>

#include <AzCore/Math/MathUtils.h>

#include <Benchmarks/PhysXBenchmarksCommon.h>
#include <Benchmarks/PhysXBenchmarksUtilities.h>
#include <Benchmarks/PhysXBenchmarkWashingMachine.h>

#include <AzFramework/Physics/SystemBus.h>

#include <RagdollTestData.h>
#include <AzFramework/Physics/RagdollPhysicsBus.h>
#include <PhysXCharacters/API/Ragdoll.h>
#include <PhysXCharacters/API/CharacterUtils.h>

#include <PhysXTestCommon.h>

namespace PhysX::Benchmarks
{
    namespace RagdollConstants
    {
        //! Controls the simulation length of the test. 30secs at 60fps
        static const int GameFramesToSimulate = 1800;

        //! The size of the test terrain
        static const float TerrainSize = 1000.0f;

        //! Decide if CCD should be on/off for the following tests
        static const bool CCDEnabled = true;

        //! Constant seed to use with random number generation
        static const long long RandGenSeed = 909; //(Number generated by adding 'Character' ascii character codes together (67+104+97+114+97+99+116+101+114).

        //! Settings used to setup each benchmark
        namespace BenchmarkSettings
        {
            //! Values passed to benchmark to select the number of rigid bodies to spawn during each test
            //! Current values will run tests between StartRange to EndRange (inclusive), multiplying by RangeMultiplier each step.
            static const int StartRange = 1;
            static const int EndRange = 64;
            static const int RangeMultipler = 2;

            //! Number of iterations for each test
            static const int NumIterations = 3;
        } // namespace BenchmarkSettings

        //! Constants for setting up the washing machine
        namespace WashingMachine
        {
            static const float CylinderRadius = 5.0f;
            static const float CylinderHeight = 10.0f;
            static const float BladeRPM = 7.5f;
        } // namespace WashingMachine
    } // namespace RagdollConstants

    //! Ragdoll performance fixture.
    //! Will create a world, and terrain used within the test.
    class PhysXCharactersRagdollBenchmarkFixture
        : public PhysX::Benchmarks::PhysXBaseBenchmarkFixture
    {
    public:
        virtual void SetUp([[maybe_unused]] const ::benchmark::State& state) override
        {
            PhysX::Benchmarks::PhysXBaseBenchmarkFixture::SetUpInternal();
            //need to get the Physics::System to be able to spawn the rigid bodies
            m_system = AZ::Interface<Physics::System>::Get();

            m_terrainEntity = PhysX::TestUtils::CreateFlatTestTerrain(m_testSceneHandle, RagdollConstants::TerrainSize, RagdollConstants::TerrainSize);
        }

        virtual void TearDown([[maybe_unused]] const ::benchmark::State& state) override
        {
            m_terrainEntity = nullptr;
            PhysX::Benchmarks::PhysXBaseBenchmarkFixture::TearDownInternal();
        }

    protected:
        // PhysXBaseBenchmarkFixture Overrides ...
        AzPhysics::SceneConfiguration GetDefaultSceneConfiguration() override
        {
            AzPhysics::SceneConfiguration sceneConfig = AzPhysics::SceneConfiguration::CreateDefault();
            sceneConfig.m_enableCcd = RagdollConstants::CCDEnabled;
            return sceneConfig;
        }

        Physics::System* m_system;
        PhysX::EntityPtr m_terrainEntity;
    };

    Physics::RagdollState GetTPose(const AZ::Vector3& position, Physics::SimulationType simulationType = Physics::SimulationType::Dynamic)
    {
        Physics::RagdollState ragdollState;
        for (int nodeIndex = 0; nodeIndex < RagdollTestData::NumNodes; nodeIndex++)
        {
            Physics::RagdollNodeState nodeState;
            nodeState.m_position = RagdollTestData::NodePositions[nodeIndex] + position;
            nodeState.m_orientation = RagdollTestData::NodeOrientations[nodeIndex];
            nodeState.m_simulationType = simulationType;
            ragdollState.push_back(nodeState);
        }

        return ragdollState;
    }

    Physics::RagdollState GetTPose(Physics::SimulationType simulationType = Physics::SimulationType::Dynamic)
    {
        return GetTPose(AZ::Vector3::CreateZero(), simulationType);
    }

    PhysX::Ragdoll* CreateRagdoll(AzPhysics::SceneHandle sceneHandle)
    {
        Physics::RagdollConfiguration* configuration =
            AZ::Utils::LoadObjectFromFile<Physics::RagdollConfiguration>(AZ::Test::GetEngineRootPath() + "/Gems/PhysX/Code/Tests/RagdollConfiguration.xml");

        configuration->m_initialState = GetTPose();
        configuration->m_parentIndices.reserve(configuration->m_nodes.size());
        for (int i = 0; i < configuration->m_nodes.size(); i++)
        {
            configuration->m_parentIndices.push_back(RagdollTestData::ParentIndices[i]);
        }

        if (auto* sceneInterface = AZ::Interface<AzPhysics::SceneInterface>::Get())
        {
            AzPhysics::SimulatedBodyHandle bodyHandle = sceneInterface->AddSimulatedBody(sceneHandle, configuration);
            return azdynamic_cast<Ragdoll*>(sceneInterface->GetSimulatedBodyFromHandle(sceneHandle, bodyHandle));
        }
        return nullptr;
    }

    //! BM_Ragdoll_AtRest - This test just spawns the requested number of ragdolls and places them near the terrain
    //! The test will run the simulation for ~1800 game frames at 60fps.
    BENCHMARK_DEFINE_F(PhysXCharactersRagdollBenchmarkFixture, BM_Ragdoll_AtRest)(benchmark::State& state)
    {
        //setup some pieces for the test
        const int numRagdolls = static_cast<const int>(state.range(0));

        //create ragdolls
        AZStd::vector<PhysX::Ragdoll*> ragdolls;
        ragdolls.reserve(numRagdolls);
        for (int i = 0; i < numRagdolls; i++)
        {
            ragdolls.emplace_back(CreateRagdoll(m_testSceneHandle));
        }

        //enable and position the ragdolls
        int spawnColIdx = 0;
        int spawnRowIdx = 0;
        const float spacing = 5.0f;
        const int ragdollsPerCol = static_cast<const int>(RagdollConstants::TerrainSize / spacing) - 1;
        for (auto& ragdoll : ragdolls)
        {
            spawnColIdx++;
            if (spawnColIdx >= ragdollsPerCol)
            {
                spawnColIdx = 0;
                spawnRowIdx++;
            }
            const AZ::Vector3 rootSpawnPosition(spacing + (spacing * spawnColIdx), spacing + (spacing * spawnRowIdx), 0.0f);
            auto kinematicTPose = GetTPose(rootSpawnPosition, static_cast<Physics::SimulationType>(state.range(1)));
            ragdoll->EnableSimulation(kinematicTPose);
            ragdoll->SetState(kinematicTPose);
        }

        //setup the sub tick tracker
        PhysX::Benchmarks::Utils::PrePostSimulationEventHandler subTickTracker;
        subTickTracker.Start(m_defaultScene);

        //setup the frame timer tracker
        AZStd::vector<double> tickTimes;
        tickTimes.reserve(RagdollConstants::GameFramesToSimulate);
        for (auto _ : state)
        {
            for (AZ::u32 i = 0; i < RagdollConstants::GameFramesToSimulate; i++)
            {
                auto start = AZStd::chrono::system_clock::now();
                StepScene1Tick(DefaultTimeStep);

                //time each physics tick and store it to analyze
                auto tickElapsedMilliseconds = PhysX::Benchmarks::Types::double_milliseconds(AZStd::chrono::system_clock::now() - start);
                tickTimes.emplace_back(tickElapsedMilliseconds.count());
            }
        }
        subTickTracker.Stop();

        //get the P50, P90, P99 percentiles
        PhysX::Benchmarks::Utils::ReportFramePercentileCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
        PhysX::Benchmarks::Utils::ReportFrameStandardDeviationAndMeanCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
    }

    //! BM_Ragdoll_MovingAndColliding - This test will create the physics washing machine, a cylinder with a spinning blade where
    //! it will spawn the requested number of ragdolls and places them above the machine to fall into the spinning blade.
    //! The test will run the simulation for ~1800 game frames at 60fps.
    BENCHMARK_DEFINE_F(PhysXCharactersRagdollBenchmarkFixture, BM_Ragdoll_MovingAndColliding)(benchmark::State& state)
    {
        //setup some pieces for the test
        AZ::SimpleLcgRandom rand;
        rand.SetSeed(RagdollConstants::RandGenSeed);
        const int numRagdolls = static_cast<const int>(state.range(0));

        //Create a washing machine of physx objects. This is a cylinder with a spinning blade that rigid bodies are placed inside
        const AZ::Vector3 washingMachineCentre(500.0f, 500.0f, 0.0f);
        PhysX::Benchmarks::WashingMachine washingMachine;
        washingMachine.SetupWashingMachine(
            m_testSceneHandle, RagdollConstants::WashingMachine::CylinderRadius, RagdollConstants::WashingMachine::CylinderHeight,
            washingMachineCentre, RagdollConstants::WashingMachine::BladeRPM);

        //create ragdolls
        AZStd::vector<PhysX::Ragdoll*> ragdolls;
        ragdolls.reserve(numRagdolls);
        for (int i = 0; i < numRagdolls; i++)
        {
            ragdolls.emplace_back(CreateRagdoll(m_testSceneHandle));
        }

        //enable and position the ragdolls
        const int ragdollsPerCol = static_cast<const int>(RagdollConstants::TerrainSize / 10.0f) - 1;
        int idx = 0;
        for (auto& ragdoll : ragdolls)
        {
            const float u = std::sqrt(rand.GetRandomFloat());
            const float theta = AZ::Constants::TwoPi * rand.GetRandomFloat();
            const float x = washingMachineCentre.GetX() + RagdollConstants::WashingMachine::CylinderRadius * u * std::sin(theta);
            const float y = washingMachineCentre.GetY() + RagdollConstants::WashingMachine::CylinderRadius * u * std::cos(theta);
            const float z = washingMachineCentre.GetZ() + 1.0f + (0.3f * idx);
            auto kinematicTPose = GetTPose(AZ::Vector3(x, y, z), Physics::SimulationType::Dynamic);
            ragdoll->EnableSimulation(kinematicTPose);
            ragdoll->SetState(kinematicTPose);
            idx++;
        }

        //setup the sub tick tracker
        PhysX::Benchmarks::Utils::PrePostSimulationEventHandler subTickTracker;
        subTickTracker.Start(m_defaultScene);

        //setup the frame timer tracker
        AZStd::vector<double> tickTimes;
        tickTimes.reserve(RagdollConstants::GameFramesToSimulate);
        for (auto _ : state)
        {
            for (AZ::u32 i = 0; i < RagdollConstants::GameFramesToSimulate; i++)
            {
                auto start = AZStd::chrono::system_clock::now();
                StepScene1Tick(DefaultTimeStep);

                //time each physics tick and store it to analyze
                auto tickElapsedMilliseconds = PhysX::Benchmarks::Types::double_milliseconds(AZStd::chrono::system_clock::now() - start);
                tickTimes.emplace_back(tickElapsedMilliseconds.count());
            }
        }
        subTickTracker.Stop();

        //get the P50, P90, P99 percentiles
        PhysX::Benchmarks::Utils::ReportFramePercentileCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
        PhysX::Benchmarks::Utils::ReportFrameStandardDeviationAndMeanCounters(state, tickTimes, subTickTracker.GetSubTickTimes());
    }

    BENCHMARK_REGISTER_F(PhysXCharactersRagdollBenchmarkFixture, BM_Ragdoll_AtRest)
        ->RangeMultiplier(RagdollConstants::BenchmarkSettings::RangeMultipler)
        ->Ranges({
            {RagdollConstants::BenchmarkSettings::StartRange, RagdollConstants::BenchmarkSettings::EndRange},
            {static_cast<int>(Physics::SimulationType::Kinematic), static_cast<int>(Physics::SimulationType::Dynamic)}
            })
        ->Unit(benchmark::kMillisecond)
        ->Iterations(RagdollConstants::BenchmarkSettings::NumIterations)
        ;

    BENCHMARK_REGISTER_F(PhysXCharactersRagdollBenchmarkFixture, BM_Ragdoll_MovingAndColliding)
        ->RangeMultiplier(RagdollConstants::BenchmarkSettings::RangeMultipler)
        ->Range(RagdollConstants::BenchmarkSettings::StartRange, RagdollConstants::BenchmarkSettings::EndRange)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(RagdollConstants::BenchmarkSettings::NumIterations)
        ;
} // namespace PhysX::Benchmarks

#endif // #ifdef HAVE_BENCHMARK
