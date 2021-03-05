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

#pragma once

#include <Atom/Feature/Mesh/MeshFeatureProcessorInterface.h>
#include <Atom/RPI.Public/Culling.h>
#include <Atom/RPI.Public/MeshDrawPacket.h>
#include <Atom/RPI.Public/Shader/ShaderSystemInterface.h>
#include <Atom/Feature/Material/MaterialAssignment.h>
#include <Atom/Feature/TransformService/TransformServiceFeatureProcessor.h>
#include <RayTracing/RayTracingFeatureProcessor.h>
#include <AzCore/Asset/AssetCommon.h>
#include <AtomCore/std/parallel/concurrency_checker.h>

namespace AZ
{
    namespace Render
    {
        class TransformServiceFeatureProcessor;
        class RayTracingFeatureProcessor;

        class MeshDataInstance
        {
            friend class MeshFeatureProcessor;
            friend class MeshLoader;

        public:
            const Data::Instance<RPI::Model>& GetModel() { return m_model; }
            const RPI::Cullable& GetCullable() { return m_cullable; }

        private:

            class MeshLoader
                : private Data::AssetBus::Handler
            {
            public:
                using ModelChangedEvent = MeshFeatureProcessorInterface::ModelChangedEvent;

                MeshLoader(const Data::Asset<RPI::ModelAsset>& modelAsset, MeshDataInstance* parent);
                ~MeshLoader();

                ModelChangedEvent& GetModelChangedEvent();

            private:
                // AssetBus::Handler overrides...
                void OnAssetReady(Data::Asset<Data::AssetData> asset) override;
                void OnAssetError(Data::Asset<Data::AssetData> asset) override;

                MeshFeatureProcessorInterface::ModelChangedEvent m_modelChangedEvent;
                Data::Asset<RPI::ModelAsset> m_modelAsset;
                MeshDataInstance* m_parent = nullptr;
            };

            void DeInit();
            void Init(Data::Instance<RPI::Model> model);
            void BuildDrawPacketList(size_t modelLodIndex);
            void SetRayTracingData();
            void SetSortKey(RHI::DrawItemSortKey sortKey);
            RHI::DrawItemSortKey GetSortKey();
            void SetLodOverride(RPI::Cullable::LodOverride lodOverride);
            RPI::Cullable::LodOverride GetLodOverride();
            void UpdateDrawPackets(bool forceUpdate = false);
            void BuildCullable();
            void UpdateCullBounds(const TransformServiceFeatureProcessor* transformService);
            void SelectMotionVectorShader(Data::Instance<RPI::Material> material);

            using DrawPacketList = AZStd::vector<RPI::MeshDrawPacket>;

            AZStd::fixed_vector<DrawPacketList, RPI::ModelLodAsset::LodCountMax> m_drawPacketListsByLod;
            RPI::Cullable m_cullable;
            MaterialAssignmentMap m_materialAssignments;

            Data::Instance<RPI::Model> m_model;
            Data::Instance<RPI::ShaderResourceGroup> m_shaderResourceGroup;
            AZStd::unique_ptr<MeshLoader> m_meshLoader;
            RPI::Scene* m_scene = nullptr;
            RHI::DrawItemSortKey m_sortKey;

            TransformServiceFeatureProcessorInterface::ObjectId m_objectId;

            bool m_cullBoundsNeedsUpdate = false;
            bool m_cullableNeedsRebuild = false;
            bool m_excludeFromReflectionCubeMaps = false;
            bool m_skinnedMeshWithMotion = false;
            bool m_rayTracingEnabled = true;
            bool m_visible = true;
        };

        //! This feature processor handles static and dynamic non-skinned meshes.
        class MeshFeatureProcessor final
            : public MeshFeatureProcessorInterface
        {
        public:

            AZ_RTTI(AZ::Render::MeshFeatureProcessor, "{6E3DFA1D-22C7-4738-A3AE-1E10AB88B29B}", MeshFeatureProcessorInterface);

            static void Reflect(AZ::ReflectContext* context);

            MeshFeatureProcessor() = default;
            virtual ~MeshFeatureProcessor() = default;

            // FeatureProcessor overrides ...
            //! Creates pools, buffers, and buffer views
            void Activate() override;
            //! Releases GPU resources.
            void Deactivate() override;
            //! Updates GPU buffers with latest data from render proxies
            void Simulate(const FeatureProcessor::SimulatePacket& packet) override;

            // RPI::SceneNotificationBus overrides ...
            void OnBeginPrepareRender() override;
            void OnEndPrepareRender() override;

            MeshHandle AcquireMesh(
                const Data::Asset<RPI::ModelAsset>& modelAsset,
                const MaterialAssignmentMap& materials = {},
                bool skinnedMeshWithMotion = false,
                bool rayTracingEnabled = true) override;
            MeshHandle AcquireMesh(
                const Data::Asset<RPI::ModelAsset> &modelAsset,
                const Data::Instance<RPI::Material>& material,
                bool skinnedMeshWithMotion = false,
                bool rayTracingEnabled = true) override;
            bool ReleaseMesh(MeshHandle& meshHandle) override;
            MeshHandle CloneMesh(const MeshHandle& meshHandle) override;

            Data::Instance<RPI::Model> GetModel(const MeshHandle& meshHandle) const override;
            void SetMaterialAssignmentMap(const MeshHandle& meshHandle, const MaterialAssignmentMap& materials) override;
            const MaterialAssignmentMap& GetMaterialAssignmentMap(const MeshHandle& meshHandle) const override;
            void ConnectModelChangeEventHandler(const MeshHandle& meshHandle, ModelChangedEvent::Handler& handler) override;

            void SetTransform(const MeshHandle& meshHandle, const AZ::Transform& transform) override;
            Transform GetTransform(const MeshHandle& meshHandle) override;

            void SetSortKey(const MeshHandle& meshHandle, RHI::DrawItemSortKey sortKey) override;
            RHI::DrawItemSortKey GetSortKey(const MeshHandle& meshHandle) override;

            void SetLodOverride(const MeshHandle& meshHandle, RPI::Cullable::LodOverride lodOverride) override;
            RPI::Cullable::LodOverride GetLodOverride(const MeshHandle& meshHandle) override;

            void SetExcludeFromReflectionCubeMaps(const MeshHandle& meshHandle, bool excludeFromReflectionCubeMaps) override;
            void SetRayTracingEnabled(const MeshHandle& meshHandle, bool rayTracingEnabled) override;
            void SetVisible(const MeshHandle& meshHandle, bool visible) override;

        private:
            MeshFeatureProcessor(const MeshFeatureProcessor&) = delete;

            // RPI::SceneNotificationBus::Handler overrides...
            void OnRenderPipelineAdded(RPI::RenderPipelinePtr pipeline) override;
            void OnRenderPipelineRemoved(RPI::RenderPipeline* pipeline) override;
                        
            AZStd::concurrency_checker m_meshDataChecker;
            StableDynamicArray<MeshDataInstance> m_meshData;
            TransformServiceFeatureProcessor* m_transformService;
            RayTracingFeatureProcessor* m_rayTracingFeatureProcessor = nullptr;
            AZ::RPI::ShaderSystemInterface::GlobalShaderOptionUpdatedEvent::Handler m_handleGlobalShaderOptionUpdate;
            bool m_forceRebuildDrawPackets = false;
        };
    } // namespace Render
} // namespace AZ
