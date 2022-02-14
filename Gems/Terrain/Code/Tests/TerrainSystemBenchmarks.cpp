/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#ifdef HAVE_BENCHMARK

#include <AzCore/Component/ComponentApplication.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Math/Random.h>
#include <AzCore/Memory/PoolAllocator.h>
#include <AzCore/Jobs/JobManagerComponent.h>
#include <AzCore/std/parallel/semaphore.h>
#include <AzCore/UnitTest/TestTypes.h>
#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Terrain/TerrainDataRequestBus.h>
#include <AzTest/AzTest.h>

#include <GradientSignal/Components/RandomGradientComponent.h>
#include <GradientSignal/Components/GradientTransformComponent.h>
#include <GradientSignal/Ebuses/GradientRequestBus.h>

#include <SurfaceData/SurfaceDataTypes.h>

#include <LmbrCentral/Shape/ShapeComponentBus.h>
#include <MockAxisAlignedBoxShapeComponent.h>

#include <TerrainSystem/TerrainSystem.h>
#include <Components/TerrainLayerSpawnerComponent.h>
#include <Components/TerrainHeightGradientListComponent.h>
#include <Components/TerrainSurfaceGradientListComponent.h>

#include <benchmark/benchmark.h>

namespace UnitTest
{
    using ::testing::NiceMock;
    using ::testing::Return;

    class TerrainSystemBenchmarkFixture
        : public UnitTest::AllocatorsBenchmarkFixture
        , public UnitTest::TraceBusRedirector
    {
    public:
        void SetUp(const benchmark::State& state) override
        {
            InternalSetUp(state);
        }
        void SetUp(benchmark::State& state) override
        {
            InternalSetUp(state);
        }

        void TearDown(const benchmark::State& state) override
        {
            InternalTearDown(state);
        }
        void TearDown(benchmark::State& state) override
        {
            InternalTearDown(state);
        }

        void InternalSetUp(const benchmark::State& state)
        {
            AZ::Debug::TraceMessageBus::Handler::BusConnect();
            UnitTest::AllocatorsBenchmarkFixture::SetUp(state);

            m_app = AZStd::make_unique<AZ::ComponentApplication>();
            ASSERT_TRUE(m_app != nullptr);

            AZ::ComponentApplication::Descriptor componentAppDesc;

            AZ::Entity* systemEntity = m_app->Create(componentAppDesc);
            ASSERT_TRUE(systemEntity != nullptr);
            m_app->AddEntity(systemEntity);

            AZ::AllocatorInstance<AZ::ThreadPoolAllocator>::Create();
        }

        void InternalTearDown(const benchmark::State& state)
        {
            AZ::AllocatorInstance<AZ::ThreadPoolAllocator>::Destroy();

            m_app->Destroy();
            m_app.reset();

            UnitTest::AllocatorsBenchmarkFixture::TearDown(state);
            AZ::Debug::TraceMessageBus::Handler::BusDisconnect();
        }

        AZStd::unique_ptr<AZ::Entity> CreateEntity()
        {
            return AZStd::make_unique<AZ::Entity>();
        }

        void ActivateEntity(AZ::Entity* entity)
        {
            entity->Init();
            entity->Activate();
        }

        template<typename Component, typename Configuration>
        Component* CreateComponent(AZ::Entity* entity, const Configuration& config)
        {
            m_app->RegisterComponentDescriptor(Component::CreateDescriptor());
            return entity->CreateComponent<Component>(config);
        }

        template<typename Component>
        Component* CreateComponent(AZ::Entity* entity)
        {
            m_app->RegisterComponentDescriptor(Component::CreateDescriptor());
            return entity->CreateComponent<Component>();
        }

        // Create a terrain system with reasonable defaults for testing, but with the ability to override the defaults
        // on a test-by-test basis.
        AZStd::unique_ptr<Terrain::TerrainSystem> CreateAndActivateTerrainSystem(
            float queryResolution = 1.0f,
            AZ::Aabb worldBounds = AZ::Aabb::CreateFromMinMax(AZ::Vector3(-128.0f), AZ::Vector3(128.0f)))
        {
            // Create the terrain system and give it one tick to fully initialize itself.
            auto terrainSystem = AZStd::make_unique<Terrain::TerrainSystem>();
            terrainSystem->SetTerrainAabb(worldBounds);
            terrainSystem->SetTerrainHeightQueryResolution(queryResolution);
            terrainSystem->Activate();
            AZ::TickBus::Broadcast(&AZ::TickBus::Events::OnTick, 0.f, AZ::ScriptTimePoint{});
            return terrainSystem;
        }

        // Create a mock shape bus listener that will listen to the given EntityId for shape requests and returns the following:
        //  - GetEncompassingAabb - returns the given Aabb
        //  - GetTransformAndLocalBounds - returns the center of the Aabb as the transform, and the size of the Aabb as the local bounds
        //  - IsPointInside - true if the point is in the Aabb, false if not
        AZStd::unique_ptr<NiceMock<UnitTest::MockShapeComponentRequests>> CreateMockShape(
            const AZ::Aabb& spawnerBox, const AZ::EntityId& shapeEntityId)
        {
            AZStd::unique_ptr<NiceMock<UnitTest::MockShapeComponentRequests>> mockShape =
                AZStd::make_unique<NiceMock<UnitTest::MockShapeComponentRequests>>(shapeEntityId);

            ON_CALL(*mockShape, GetEncompassingAabb).WillByDefault(Return(spawnerBox));
            ON_CALL(*mockShape, GetTransformAndLocalBounds)
                .WillByDefault(
                    [spawnerBox](AZ::Transform& transform, AZ::Aabb& bounds)
                    {
                        transform = AZ::Transform::CreateTranslation(spawnerBox.GetCenter());
                        bounds = spawnerBox.GetTranslated(-spawnerBox.GetCenter());
                    });
            ON_CALL(*mockShape, IsPointInside)
                .WillByDefault(
                    [spawnerBox](const AZ::Vector3& point) -> bool
                    {
                        return spawnerBox.Contains(point);
                    });

            return mockShape;
        }

        // Create an entity with a Random Gradient on it that can be used for gradient queries.
        AZStd::unique_ptr<AZ::Entity> CreateTestRandomGradientEntity(const AZ::Aabb& spawnerBox, uint32_t randomSeed)
        {
            // Create the base entity
            AZStd::unique_ptr<AZ::Entity> testGradientEntity = CreateEntity();

            // Add a mock AABB Shape so that the shape requirement is fulfilled.
            CreateComponent<UnitTest::MockAxisAlignedBoxShapeComponent>(testGradientEntity.get());

            // Create the Random Gradient Component with some default parameters.
            GradientSignal::RandomGradientConfig config;
            config.m_randomSeed = randomSeed;
            CreateComponent<GradientSignal::RandomGradientComponent>(testGradientEntity.get(), config);

            // Create the Gradient Transform Component with some default parameters.
            GradientSignal::GradientTransformConfig gradientTransformConfig;
            gradientTransformConfig.m_wrappingType = GradientSignal::WrappingType::None;
            CreateComponent<GradientSignal::GradientTransformComponent>(testGradientEntity.get(), gradientTransformConfig);

            // Set the transform to match the given spawnerBox
            auto transform = CreateComponent<AzFramework::TransformComponent>(testGradientEntity.get());
            transform->SetLocalTM(AZ::Transform::CreateTranslation(spawnerBox.GetCenter()));
            transform->SetWorldTM(AZ::Transform::CreateTranslation(spawnerBox.GetCenter()));

            return testGradientEntity;
        }

        AZStd::unique_ptr<AZ::Entity> CreateTestLayerSpawnerEntity(
            const AZ::Aabb& spawnerBox, const AZ::EntityId& heightGradientEntityId,
            const Terrain::TerrainSurfaceGradientListConfig& surfaceConfig)
        {
            // Create the base entity
            AZStd::unique_ptr<AZ::Entity> testLayerSpawnerEntity = CreateEntity();

            // Add a mock AABB Shape so that the shape requirement is fulfilled.
            CreateComponent<UnitTest::MockAxisAlignedBoxShapeComponent>(testLayerSpawnerEntity.get());

            // Add a Terrain Layer Spawner
            CreateComponent<Terrain::TerrainLayerSpawnerComponent>(testLayerSpawnerEntity.get());

            // Add a Terrain Height Gradient List with one entry pointing to the given gradient entity
            Terrain::TerrainHeightGradientListConfig heightConfig;
            heightConfig.m_gradientEntities.emplace_back(heightGradientEntityId);
            CreateComponent<Terrain::TerrainHeightGradientListComponent>(testLayerSpawnerEntity.get(), heightConfig);

            // Add a Terrain Surface Gradient List with however many entries we were given
            CreateComponent<Terrain::TerrainSurfaceGradientListComponent>(testLayerSpawnerEntity.get(), surfaceConfig);

            // Set the transform to match the given spawnerBox
            auto transform = CreateComponent<AzFramework::TransformComponent>(testLayerSpawnerEntity.get());
            transform->SetLocalTM(AZ::Transform::CreateTranslation(spawnerBox.GetCenter()));
            transform->SetWorldTM(AZ::Transform::CreateTranslation(spawnerBox.GetCenter()));

            return testLayerSpawnerEntity;
        }

        void RunTerrainApiBenchmark(
            benchmark::State& state,
            AZStd::function<void(
                float queryResolution,
                const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)> ApiCaller)
        {
            // Get the ranges for querying from our benchmark parameters
            float boundsRange = aznumeric_cast<float>(state.range(0));
            uint32_t numSurfaces = aznumeric_cast<uint32_t>(state.range(1));
            AzFramework::Terrain::TerrainDataRequests::Sampler sampler =
                static_cast<AzFramework::Terrain::TerrainDataRequests::Sampler>(state.range(2));

            // Set up our world bounds and query resolution
            AZ::Aabb worldBounds = AZ::Aabb::CreateFromMinMax(AZ::Vector3(-boundsRange / 2.0f), AZ::Vector3(boundsRange / 2.0f));
            float queryResolution = 1.0f;

            // Create a Random Gradient to use as our height provider
            const uint32_t heightRandomSeed = 12345;
            auto heightGradientEntity = CreateTestRandomGradientEntity(worldBounds, heightRandomSeed);
            auto heightGradientShapeRequests = CreateMockShape(worldBounds, heightGradientEntity->GetId());
            ActivateEntity(heightGradientEntity.get());


            // Create a set of Random Gradients to use as our surface providers
            Terrain::TerrainSurfaceGradientListConfig surfaceConfig;
            AZStd::vector<AZStd::unique_ptr<AZ::Entity>> surfaceGradientEntities;
            AZStd::vector<AZStd::unique_ptr<NiceMock<UnitTest::MockShapeComponentRequests>>> surfaceGradientShapeRequests;
            for (uint32_t surfaces = 0; surfaces < numSurfaces; surfaces++)
            {
                const uint32_t surfaceRandomSeed = 23456 + surfaces;
                auto surfaceGradientEntity = CreateTestRandomGradientEntity(worldBounds, surfaceRandomSeed);
                auto shapeRequests = CreateMockShape(worldBounds, surfaceGradientEntity->GetId());
                ActivateEntity(surfaceGradientEntity.get());

                // Give each gradient a new surface tag
                surfaceConfig.m_gradientSurfaceMappings.emplace_back(
                    surfaceGradientEntity->GetId(), SurfaceData::SurfaceTag(AZStd::string::format("test%u", surfaces)));

                surfaceGradientEntities.emplace_back(AZStd::move(surfaceGradientEntity));
                surfaceGradientShapeRequests.emplace_back(AZStd::move(shapeRequests));
            }

            // Create a single Terrain Layer Spawner that covers the entire terrain world bounds
            // (Do this *after* creating and activating the height and surface gradients)
            auto testLayerSpawnerEntity = CreateTestLayerSpawnerEntity(worldBounds, heightGradientEntity->GetId(), surfaceConfig);
            auto spawnerShapeRequests = CreateMockShape(worldBounds, testLayerSpawnerEntity->GetId());
            ActivateEntity(testLayerSpawnerEntity.get());

            // Create the global job manager.
            auto serializeContext = AZStd::make_unique<AZ::SerializeContext>();
            auto jobManagerComponentDescriptor = AZ::JobManagerComponent::CreateDescriptor();
            jobManagerComponentDescriptor->Reflect(serializeContext.get());
            auto jobManagerEntity = AZStd::make_unique<AZ::Entity>();
            jobManagerEntity->CreateComponent<AZ::JobManagerComponent>();
            jobManagerEntity->Init();
            jobManagerEntity->Activate();

            // Create the terrain system (do this after creating the terrain layer entity to ensure that we don't need any data refreshes)
            // Also ensure to do this after creating the global JobManager.
            auto terrainSystem = CreateAndActivateTerrainSystem(queryResolution, worldBounds);

            // Call the terrain API we're testing for every height and width in our ranges.
            for (auto stateIterator : state)
            {
                ApiCaller(queryResolution, worldBounds, sampler);
            }

            testLayerSpawnerEntity.reset();
            spawnerShapeRequests.reset();

            heightGradientEntity.reset();
            heightGradientShapeRequests.reset();

            surfaceGradientEntities.clear();
            surfaceGradientShapeRequests.clear();
        }

        void GenerateInputPositionsList(float queryResolution, const AZ::Aabb& worldBounds, AZStd::vector<AZ::Vector3>& positions)
        {
            const size_t numSamplesX = aznumeric_cast<size_t>(ceil(worldBounds.GetExtents().GetX() / queryResolution));
            const size_t numSamplesY = aznumeric_cast<size_t>(ceil(worldBounds.GetExtents().GetY() / queryResolution));

            for (size_t y = 0; y < numSamplesY; y++)
            {
                float fy = aznumeric_cast<float>(worldBounds.GetMin().GetY() + (y * queryResolution));
                for (size_t x = 0; x < numSamplesX; x++)
                {
                    float fx = aznumeric_cast<float>(worldBounds.GetMin().GetX() + (x * queryResolution));
                    positions.emplace_back(fx, fy, 0.0f);
                }
            }
        }

    protected:
        AZStd::unique_ptr<AZ::ComponentApplication> m_app;
    };


    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_GetHeight)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                float worldMinZ = worldBounds.GetMin().GetZ();

                for (float y = worldBounds.GetMin().GetY(); y < worldBounds.GetMax().GetY(); y += 1.0f)
                {
                    for (float x = worldBounds.GetMin().GetX(); x < worldBounds.GetMax().GetX(); x += 1.0f)
                    {
                        float terrainHeight = worldMinZ;
                        bool terrainExists = false;
                        AzFramework::Terrain::TerrainDataRequestBus::BroadcastResult(
                            terrainHeight, &AzFramework::Terrain::TerrainDataRequests::GetHeightFromFloats, x, y, sampler, &terrainExists);
                        benchmark::DoNotOptimize(terrainHeight);
                    }
                }
            });
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_GetHeight)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsRegion)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_position.GetZ());
                };

                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessHeightsFromRegion, worldBounds, stepSize, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsRegion)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsRegionAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_position.GetZ());
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;

                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessHeightsFromRegionAsync, worldBounds, stepSize, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsRegionAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsList)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_position.GetZ());
                };
                
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessHeightsFromList, inPositions, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsList)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsListAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_position.GetZ());
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessHeightsFromListAsync, inPositions, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessHeightsListAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_GetNormal)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
               AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                for (float y = worldBounds.GetMin().GetY(); y < worldBounds.GetMax().GetY(); y += 1.0f)
                {
                    for (float x = worldBounds.GetMin().GetX(); x < worldBounds.GetMax().GetX(); x += 1.0f)
                    {
                        AZ::Vector3 terrainNormal;
                        bool terrainExists = false;
                        AzFramework::Terrain::TerrainDataRequestBus::BroadcastResult(
                            terrainNormal, &AzFramework::Terrain::TerrainDataRequests::GetNormalFromFloats, x, y, sampler, &terrainExists);
                        benchmark::DoNotOptimize(terrainNormal);
                    }
                }
            });
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_GetNormal)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsRegion)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_normal);
                };
                
                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessNormalsFromRegion, worldBounds, stepSize, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsRegion)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsRegionAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_normal);
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;

                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessNormalsFromRegionAsync, worldBounds, stepSize, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsRegionAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsList)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_normal);
                };
                
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessNormalsFromList, inPositions, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsList)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsListAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_normal);
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessNormalsFromListAsync, inPositions, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessNormalsListAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_GetSurfaceWeights)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
               AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AzFramework::SurfaceData::SurfaceTagWeightList surfaceWeights;
                for (float y = worldBounds.GetMin().GetY(); y < worldBounds.GetMax().GetY(); y += 1.0f)
                {
                    for (float x = worldBounds.GetMin().GetX(); x < worldBounds.GetMax().GetX(); x += 1.0f)
                    {
                        bool terrainExists = false;
                        AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                            &AzFramework::Terrain::TerrainDataRequests::GetSurfaceWeightsFromFloats, x, y, surfaceWeights, sampler,
                            &terrainExists);
                        benchmark::DoNotOptimize(surfaceWeights);
                    }
                }
            });
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_GetSurfaceWeights)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsRegion)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_surfaceTags);
                };
                
                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfaceWeightsFromRegion, worldBounds, stepSize, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsRegion)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsRegionAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_surfaceTags);
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;

                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfaceWeightsFromRegionAsync, worldBounds, stepSize, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsRegionAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsList)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_surfaceTags);
                };
                
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfaceWeightsFromList, inPositions, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsList)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsListAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint.m_surfaceTags);
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfaceWeightsFromListAsync, inPositions, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfaceWeightsListAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 2, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 4, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_GetSurfacePoints)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
               AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AzFramework::SurfaceData::SurfacePoint surfacePoint;
                for (float y = worldBounds.GetMin().GetY(); y < worldBounds.GetMax().GetY(); y += 1.0f)
                {
                    for (float x = worldBounds.GetMin().GetX(); x < worldBounds.GetMax().GetX(); x += 1.0f)
                    {
                        bool terrainExists = false;
                        AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                            &AzFramework::Terrain::TerrainDataRequests::GetSurfacePointFromFloats, x, y, surfacePoint, sampler,
                            &terrainExists);
                        benchmark::DoNotOptimize(surfacePoint);
                    }
                }
            });
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_GetSurfacePoints)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsRegion)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint);
                };
                
                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfacePointsFromRegion, worldBounds, stepSize, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsRegion)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsRegionAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            []([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                auto perPositionCallback = []([[maybe_unused]] size_t xIndex, [[maybe_unused]] size_t yIndex, 
                    const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint);
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;

                AZ::Vector2 stepSize = AZ::Vector2(queryResolution);
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfacePointsFromRegionAsync, worldBounds, stepSize, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsRegionAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsList)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint);
                };
                
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfacePointsFromList, inPositions, perPositionCallback, sampler);
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsList)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsListAsync)(benchmark::State& state)
    {
        // Run the benchmark
        RunTerrainApiBenchmark(
            state,
            [this]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                AZStd::vector<AZ::Vector3> inPositions;
                GenerateInputPositionsList(queryResolution, worldBounds, inPositions);

                auto perPositionCallback = [](const AzFramework::SurfaceData::SurfacePoint& surfacePoint, [[maybe_unused]] bool terrainExists)
                {
                    benchmark::DoNotOptimize(surfacePoint);
                };

                AZStd::semaphore completionEvent;
                auto completionCallback = [&completionEvent](AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::TerrainJobContext>)
                {
                    completionEvent.release();
                };

                AZStd::shared_ptr<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams> asyncParams
                    = AZStd::make_shared<AzFramework::Terrain::TerrainDataRequests::ProcessAsyncParams>();
                asyncParams->m_completionCallback = completionCallback;
                AzFramework::Terrain::TerrainDataRequestBus::Broadcast(
                    &AzFramework::Terrain::TerrainDataRequests::ProcessSurfacePointsFromListAsync, inPositions, perPositionCallback, sampler, asyncParams);

                completionEvent.acquire();
            }
        );
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_ProcessSurfacePointsListAsync)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::BILINEAR) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::CLAMP) })
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_GetClosestIntersectionRandom)(benchmark::State& state)
    {
        // Run the benchmark
        const uint32_t numRays = aznumeric_cast<uint32_t>(state.range(1));
        RunTerrainApiBenchmark(
            state,
            [numRays]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                [[maybe_unused]] AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                // Cast rays starting at random positions above the terrain,
                // and ending at a random positions below the terrain.
                AZ::SimpleLcgRandom random;
                AzFramework::RenderGeometry::RayRequest ray;
                AzFramework::RenderGeometry::RayResult result;
                for (uint32_t i = 0; i < numRays; ++i)
                {
                    ray.m_startWorldPosition.SetX(worldBounds.GetMin().GetX() + (random.GetRandomFloat() * worldBounds.GetXExtent()));
                    ray.m_startWorldPosition.SetY(worldBounds.GetMin().GetY() + (random.GetRandomFloat() * worldBounds.GetYExtent()));
                    ray.m_startWorldPosition.SetZ(worldBounds.GetMax().GetZ());
                    ray.m_endWorldPosition.SetX(worldBounds.GetMin().GetX() + (random.GetRandomFloat() * worldBounds.GetXExtent()));
                    ray.m_endWorldPosition.SetY(worldBounds.GetMin().GetY() + (random.GetRandomFloat() * worldBounds.GetYExtent()));
                    ray.m_endWorldPosition.SetZ(worldBounds.GetMin().GetZ());
                    AzFramework::Terrain::TerrainDataRequestBus::BroadcastResult(
                        result, &AzFramework::Terrain::TerrainDataRequests::GetClosestIntersection, ray);
                }
            });
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_GetClosestIntersectionRandom)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 10, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 10, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 10, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 100, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 100, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 100, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 1000, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1000, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1000, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);

    BENCHMARK_DEFINE_F(TerrainSystemBenchmarkFixture, BM_GetClosestIntersectionWorstCase)(benchmark::State& state)
    {
        // Run the benchmark
        const uint32_t numRays = aznumeric_cast<uint32_t>(state.range(1));
        RunTerrainApiBenchmark(
            state,
            [numRays]([[maybe_unused]] float queryResolution, const AZ::Aabb& worldBounds,
                [[maybe_unused]] AzFramework::Terrain::TerrainDataRequests::Sampler sampler)
            {
                // Cast rays starting at an upper corner of the terrain world,
                // and ending at the opposite top corner of the terrain world,
                // traversing the entire grid without finding an intersection.
                AzFramework::RenderGeometry::RayRequest ray;
                AzFramework::RenderGeometry::RayResult result;
                ray.m_startWorldPosition = worldBounds.GetMax();
                ray.m_endWorldPosition = worldBounds.GetMin();
                ray.m_endWorldPosition.SetZ(ray.m_startWorldPosition.GetZ());
                for (uint32_t i = 0; i < numRays; ++i)
                {
                    AzFramework::Terrain::TerrainDataRequestBus::BroadcastResult(
                        result, &AzFramework::Terrain::TerrainDataRequests::GetClosestIntersection, ray);
                }
            });
    }

    BENCHMARK_REGISTER_F(TerrainSystemBenchmarkFixture, BM_GetClosestIntersectionWorstCase)
        ->Args({ 1024, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 10, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 10, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 10, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 100, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 100, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 100, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 1024, 1000, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 2048, 1000, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Args({ 4096, 1000, static_cast<int>(AzFramework::Terrain::TerrainDataRequests::Sampler::EXACT) })
        ->Unit(::benchmark::kMillisecond);
#endif

}
