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

#include <Atom/RPI.Public/RPISystem.h>

#include <Atom/RPI.Reflect/Asset/AssetReference.h>
#include <Atom/RPI.Reflect/Asset/AssetHandler.h>
#include <Atom/RPI.Reflect/Material/MaterialAsset.h>
#include <Atom/RPI.Reflect/ResourcePoolAsset.h>
#include <Atom/RPI.Reflect/Shader/ShaderAsset.h>
#include <Atom/RPI.Reflect/System/AnyAsset.h>
#include <Atom/RPI.Reflect/System/AssetAliases.h>
#include <Atom/RPI.Reflect/System/PipelineRenderSettings.h>
#include <Atom/RPI.Reflect/System/RenderPipelineDescriptor.h>
#include <Atom/RPI.Reflect/Asset/AssetUtils.h>

#include <Atom/RPI.Public/FeatureProcessor.h>
#include <Atom/RPI.Public/GpuQuery/GpuQueryTypes.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/RPI.Public/View.h>
#include <Atom/RPI.Public/Pass/PassFactory.h>

#include <Atom/RHI/Factory.h>
#include <Atom/RHI/Device.h>
#include <Atom/RHI.Reflect/PlatformLimitsDescriptor.h>

#include <AzCore/Debug/EventTrace.h>
#include <AzCore/Interface/Interface.h>

#include <AzFramework/Asset/AssetSystemBus.h>

// This will cause the RPI System to print out global state (like the current pass hierarchy) when an assert is hit
// This is useful for rendering engineers debugging a crash in the RPI/RHI layers
#define AZ_RPI_PRINT_GLOBAL_STATE_ON_ASSERT 0

namespace AZ
{
    namespace RPI
    {
        RPISystemInterface* RPISystemInterface::Get()
        {
            return Interface<RPISystemInterface>::Get();
        }

        void RPISystem::Reflect(ReflectContext* context)
        {
            AssetReference::Reflect(context);

            BufferSystem::Reflect(context);
            ImageSystem::Reflect(context);
            MaterialSystem::Reflect(context);
            ModelSystem::Reflect(context);
            ShaderSystem::Reflect(context);
            PassSystem::Reflect(context);

            ResourcePoolAsset::Reflect(context);

            SceneDescriptor::Reflect(context);
            PipelineRenderSettings::Reflect(context);
            RenderPipelineDescriptor::Reflect(context);
            AssetAliases::Reflect(context);

            RPISystemDescriptor::Reflect(context);
            GpuQuerySystemDescriptor::Reflect(context);
            ShaderMetricsSystem::Reflect(context);

            PipelineStatisticsResult::Reflect(context);
        }

        void RPISystem::Initialize(const RPISystemDescriptor& rpiSystemDescriptor)
        {
            m_rhiSystem.InitDevice();

            // Gather asset handlers from sub-systems.
            ImageSystem::GetAssetHandlers(m_assetHandlers);
            BufferSystem::GetAssetHandlers(m_assetHandlers);
            MaterialSystem::GetAssetHandlers(m_assetHandlers);
            ModelSystem::GetAssetHandlers(m_assetHandlers);
            PassSystem::GetAssetHandlers(m_assetHandlers);
            ShaderSystem::GetAssetHandlers(m_assetHandlers);
            m_assetHandlers.emplace_back(MakeAssetHandler<ResourcePoolAssetHandler>());
            m_assetHandlers.emplace_back(MakeAssetHandler<AnyAssetHandler>());

            m_materialSystem.Init();
            m_modelSystem.Init();
            m_shaderSystem.Init();
            m_shaderMetricsSystem.Init();
            m_passSystem.Init();
            m_featureProcessorFactory.Init();
            m_querySystem.Init(m_descriptor.m_gpuQuerySystemDescriptor);

            Interface<RPISystemInterface>::Register(this);

            SystemTickBus::Handler::BusConnect();

#if AZ_RPI_PRINT_GLOBAL_STATE_ON_ASSERT
            Debug::TraceMessageBus::Handler::BusConnect();
#endif
            m_descriptor = rpiSystemDescriptor;
        }

        void RPISystem::Shutdown()
        {
            m_viewportContextManager.Shutdown();
            m_viewSrgAsset.Reset();
            m_sceneSrgAsset.Reset();

#if AZ_RPI_PRINT_GLOBAL_STATE_ON_ASSERT
            Debug::TraceMessageBus::Handler::BusDisconnect();
#endif
            SystemTickBus::Handler::BusDisconnect();

            Interface<RPISystemInterface>::Unregister(this);

            m_featureProcessorFactory.Shutdown();
            m_passSystem.Shutdown();
            m_dynamicDraw.Shutdown();
            m_bufferSystem.Shutdown();
            m_materialSystem.Shutdown();
            m_modelSystem.Shutdown();
            m_shaderSystem.Shutdown();
            m_shaderMetricsSystem.Shutdown();
            m_imageSystem.Shutdown();
            m_querySystem.Shutdown();
            m_rhiSystem.Shutdown();

            /**
             * [LY-86745] We need to pump the asset manager queue here, because
             * it uses AZStd::function<> with vtable pointers embedded from this DLL.
             * If we allow the DLL to shutdown with queued events, they will be pumped
             * later by the asset manager component, which will then reference garbage
             * vtable pointers.
             *
             * Note that it's necessary to pump before *and* after we clear the handlers,
             * since the handler clear could result in more events dispatched.
             */
            Data::AssetManager::Instance().DispatchEvents();
            m_assetHandlers.clear();
            Data::AssetManager::Instance().DispatchEvents();
        }

        void RPISystem::RegisterScene(ScenePtr scene)
        {
            for (auto& sceneItem : m_scenes)
            {
                if (sceneItem == scene)
                {
                    AZ_Assert(false, "Scene was already registered");
                    return;
                }
            }

            m_scenes.push_back(scene);
        }

        void RPISystem::UnregisterScene(ScenePtr scene)
        {
            for (auto itr = m_scenes.begin(); itr != m_scenes.end(); itr++)
            {
                if (*itr == scene)
                {
                    m_scenes.erase(itr);
                    return;
                }
            }
            AZ_Assert(false, "Can't unregister scene which wasn't registered");
        }

        ScenePtr RPISystem::GetScene(const SceneId& sceneId) const
        {
            for (const auto& scene : m_scenes)
            {
                if (scene->GetId() == sceneId)
                {
                    return scene;
                }
            }
            return nullptr;
        }

        ScenePtr RPISystem::GetDefaultScene() const
        {
            if (m_scenes.size() > 0)
            {
                return m_scenes[0];
            }
            return nullptr;
        }


        RenderPipelinePtr RPISystem::GetRenderPipelineForWindow(AzFramework::NativeWindowHandle windowHandle)
        {
            RenderPipelinePtr renderPipeline;
            for (auto& scene : m_scenes)
            {
                renderPipeline = scene->FindRenderPipelineForWindow(windowHandle);
                if (renderPipeline)
                {
                    return renderPipeline;
                }
            }
            return nullptr;
        }

        Data::Asset<ShaderResourceGroupAsset> RPISystem::GetSceneSrgAsset() const
        {
            AZ_Assert(m_systemAssetsInitialized, "InitializeSystemAssets() should be called once when asset catalog loaded'");
            return m_sceneSrgAsset;
        }

        Data::Asset<ShaderResourceGroupAsset> RPISystem::GetViewSrgAsset() const
        {
            AZ_Assert(m_systemAssetsInitialized, "InitializeSystemAssets() should be called once when asset catalog loaded'");
            return m_viewSrgAsset;
        }

        void RPISystem::OnSystemTick()
        {
            AZ_ATOM_PROFILE_FUNCTION("RPI", "RPISystem: OnSystemTick");

            // Image system update is using system tick but not game tick so it can stream images in background even game is pausing
            m_imageSystem.Update();
        }

        void RPISystem::SimulationTick()
        {
            if (!m_systemAssetsInitialized)
            {
                return;
            }
            AZ_ATOM_PROFILE_FUNCTION("RPI", "RPISystem: SimulationTick");

            // Update tick time info
            FillTickTimeInfo();

            for (auto& scene : m_scenes)
            {
                scene->Simulate(m_tickTime, m_simulationJobPolicy);
            }
        }

        void RPISystem::FillTickTimeInfo()
        {
            AZ::TickRequestBus::BroadcastResult(m_tickTime.m_gameDeltaTime, &AZ::TickRequestBus::Events::GetTickDeltaTime);
            ScriptTimePoint currentTime;
            AZ::TickRequestBus::BroadcastResult(currentTime, &AZ::TickRequestBus::Events::GetTimeAtCurrentTick);
            m_tickTime.m_currentGameTime = static_cast<float>(currentTime.GetMilliseconds());
        }

        void RPISystem::RenderTick()
        {
            if (!m_systemAssetsInitialized)
            {
                return;
            }

            AZ_PROFILE_FUNCTION(AZ::Debug::ProfileCategory::AzRender);
            AZ_ATOM_PROFILE_FUNCTION("RPI", "RPISystem: RenderTick");

            // Query system update is to increment the frame count
            m_querySystem.Update();

            // Collect draw packets for each scene and prepare RPI system SRGs
            // [GFX TODO] We may parallel scenes' prepare render.
            for (auto& scenePtr : m_scenes)
            {
                scenePtr->PrepareRender(m_tickTime, m_prepareRenderJobPolicy);
            }

            m_rhiSystem.FrameUpdate(
                [this](RHI::FrameGraphBuilder& frameGraphBuilder)
            {
                // Pass system's frame update, which includes the logic of adding scope producers, has to be added here since the scope producers only can be added to the frame
                // when frame started which cleans up previous scope producers. 
                m_passSystem.FrameUpdate(frameGraphBuilder);

                // Update View Srgs
                for (auto& scenePtr : m_scenes)
                {
                    scenePtr->UpdateSrgs();
                }
            });
           
            m_dynamicDraw.FrameEnd();
            m_passSystem.FrameEnd();

            for (auto& scenePtr : m_scenes)
            {
                scenePtr->OnFrameEnd();
            }

            m_renderTick++;
        }

        void RPISystem::SetSimulationJobPolicy(RHI::JobPolicy jobPolicy)
        {
            m_simulationJobPolicy = jobPolicy;
        }

        RHI::JobPolicy RPISystem::GetSimulationJobPolicy() const
        {
            return m_simulationJobPolicy;
        }

        void RPISystem::SetRenderPrepareJobPolicy(RHI::JobPolicy jobPolicy)
        {
            m_prepareRenderJobPolicy = jobPolicy;
        }

        RHI::JobPolicy RPISystem::GetRenderPrepareJobPolicy() const
        {
            return m_prepareRenderJobPolicy;
        }

        const RPISystemDescriptor& RPISystem::GetDescriptor() const
        {
            return m_descriptor;
        }

        Name RPISystem::GetRenderApiName() const
        {
            return RHI::Factory::Get().GetName();
        }

        void RPISystem::InitializeSystemAssets()
        {
            AzFramework::AssetSystem::AssetStatus status = AzFramework::AssetSystem::AssetStatus_Unknown;
            if (m_systemAssetsInitialized)
            {
                AZ_Warning("RPISystem", false , "InitializeSystemAssets should only be called once'");
                return;
            }

            //[GFX TODO][ATOM-5867] - Move file loading code within RHI to reduce coupling with RPI
            AZStd::string platformLimitsFilePath = AZStd::string::format("config/platform/%s/%s/platformlimits.azasset", AZ_TRAIT_OS_PLATFORM_NAME, GetRenderApiName().GetCStr());
            AZStd::to_lower(platformLimitsFilePath.begin(), platformLimitsFilePath.end());
            // Wait for the platformlimits asset to be compiled (if it exists)
            AzFramework::AssetSystemRequestBus::BroadcastResult(
                status, &AzFramework::AssetSystemRequestBus::Events::CompileAssetSync, platformLimitsFilePath);
            Data::Asset<RPI::AnyAsset> platformLimitsAsset = RPI::AssetUtils::LoadAssetByProductPath<RPI::AnyAsset>(platformLimitsFilePath.c_str(), RPI::AssetUtils::TraceLevel::Error); 
            m_descriptor.m_rhiSystemDescriptor.m_platformLimits = RPI::GetDataFromAnyAsset<RHI::PlatformLimits>(platformLimitsAsset);

            m_rhiSystem.Init(m_descriptor.m_rhiSystemDescriptor);
            m_imageSystem.Init(m_descriptor.m_imageSystemDescriptor);
            m_bufferSystem.Init();
            m_dynamicDraw.Init(m_descriptor.m_dynamicDrawSystemDescriptor);

            // Wait for the assets be compiled
            AzFramework::AssetSystemRequestBus::BroadcastResult(
                status, &AzFramework::AssetSystemRequestBus::Events::CompileAssetSync, m_descriptor.m_viewSrgAssetPath);
            AZ_Error("RPISystem", status == AzFramework::AssetSystem::AssetStatus_Compiled, "Could not compile view SRG at '%s'", m_descriptor.m_viewSrgAssetPath.c_str());
            AzFramework::AssetSystemRequestBus::BroadcastResult(
                status, &AzFramework::AssetSystemRequestBus::Events::CompileAssetSync, m_descriptor.m_sceneSrgAssetPath);
            AZ_Error("RPISystem", status == AzFramework::AssetSystem::AssetStatus_Compiled, "Could not compile scene SRG at '%s'", m_descriptor.m_sceneSrgAssetPath.c_str());
            AzFramework::AssetSystemRequestBus::BroadcastResult(
                status, &AzFramework::AssetSystemRequestBus::Events::CompileAssetSync, m_descriptor.m_passTemplatesMappingPath);
            AZ_Error("RPISystem", status == AzFramework::AssetSystem::AssetStatus_Compiled, "Could not compile pass template mapping at '%s'", m_descriptor.m_passTemplatesMappingPath.c_str());

            m_viewSrgAsset = AssetUtils::LoadAssetByProductPath<ShaderResourceGroupAsset>(m_descriptor.m_viewSrgAssetPath.c_str(), AssetUtils::TraceLevel::Error);
            m_sceneSrgAsset = AssetUtils::LoadAssetByProductPath<ShaderResourceGroupAsset>(m_descriptor.m_sceneSrgAssetPath.c_str(), AssetUtils::TraceLevel::Error);

            // Have pass system load default pass template mapping
            m_passSystem.LoadPassTemplateMappings(m_descriptor.m_passTemplatesMappingPath);
            m_systemAssetsInitialized = true;
        }

        void RPISystem::InitializeSystemAssetsForTests()
        {
            if (m_systemAssetsInitialized)
            {
                AZ_Warning("RPISystem", false, "InitializeSystemAssets should only be called once'");
                return;
            }

            //Init rhi/image/buffer systems to match InitializeSystemAssets
            m_rhiSystem.Init(m_descriptor.m_rhiSystemDescriptor);
            m_imageSystem.Init(m_descriptor.m_imageSystemDescriptor);
            m_bufferSystem.Init();

            // Assets aren't actually available or needed for tests, but the m_systemAssetsInitialized flag still needs to be flipped.
            m_systemAssetsInitialized = true;
            return;
        }

        bool RPISystem::OnPreAssert([[maybe_unused]] const char* fileName, [[maybe_unused]] int line, [[maybe_unused]] const char* func, [[maybe_unused]] const char* message)
        {
#if AZ_RPI_PRINT_GLOBAL_STATE_ON_ASSERT
            AZ_Printf("RPI System", "\n--- Assert hit! Dumping RPI state ---\n\n");
            m_passSystem.DebugPrintPassHierarchy();
#endif
            return false;
        }
                
        uint64_t RPISystem::GetCurrentTick() const
        {
            return m_renderTick;
        }

    } //namespace RPI
} //namespace AZ
