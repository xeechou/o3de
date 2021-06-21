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

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/optional.h>
#include <AzCore/EBus/Event.h>

#include <Atom/RPI.Public/AssetInitBus.h>
#include <Atom/RPI.Reflect/Asset/AssetHandler.h>
#include <Atom/RPI.Reflect/Shader/ShaderOptionGroupLayout.h>
#include <Atom/RPI.Reflect/Shader/ShaderVariantAsset.h>
#include <Atom/RPI.Reflect/Shader/ShaderVariantTreeAsset.h>
#include <Atom/RPI.Reflect/Shader/ShaderInputContract.h>
#include <Atom/RPI.Reflect/Shader/ShaderOutputContract.h>
#include <Atom/RPI.Reflect/Shader/ShaderCommonTypes.h>
#include <Atom/RPI.Reflect/Shader/ShaderVariantKey.h>
#include <Atom/RPI.Reflect/Shader/IShaderVariantFinder.h>

#include <Atom/RHI/PipelineStateDescriptor.h>

#include <Atom/RHI.Reflect/ShaderStages.h>
#include <Atom/RHI.Reflect/PipelineLayoutDescriptor.h>

#include <limits>

namespace AZ
{
    namespace RPI
    {
        using ShaderResourceGroupLayoutList = AZStd::fixed_vector<RHI::Ptr<RHI::ShaderResourceGroupLayout>, RHI::Limits::Pipeline::ShaderResourceGroupCountMax>;

        enum class ShaderAssetSubId : uint32_t
        {
            ShaderAsset = 0, //!< for .azshader file, One per .shader.
            RootShaderVariantAsset, //!< for .azshadervariant, one per supervariant and referenced inside the .azshader.
            FlatAzsl, //!< .azslin, this file contains the result of preprocessing an azsl file with MCPP, along with prepending the per-RHI azsli header.
            IaJson, //!< .ia.json, Input Assembly reflection data.
            OmJson, //!< .om.json, Output Merger reflection data. 
            SrgJson, //!< .srg.json, Shader Resource Group reflection data.
            OptionsJson, //!< .options.json, Shader Options reflection data.
            BindingdepJson, //!<.bindingdep.json, Binding dependencies.
            GeneratedHlslSource, //!<.hlsl code generated with AZSLc.
            FirstByProduct, //!< This must be last because we use this as a base for adding all the debug byProducts generated
                            //!< with dxc, or spirv-cross, etc.
        };

        class ShaderAsset final
            : public Data::AssetData
            , public ShaderVariantFinderNotificationBus::Handler
            , public Data::AssetBus::Handler
            , public AssetInitBus::Handler
        {
            friend class ShaderAssetCreator;
            friend class ShaderAssetHandler;
            friend class ShaderAssetTester;
        public:
            AZ_RTTI(ShaderAsset, "{823395A3-D570-49F4-99A9-D820CD1DEF98}", Data::AssetData);
            static void Reflect(ReflectContext* context);

            static constexpr char DisplayName[] = "Shader";
            static constexpr char Extension[] = "azshader";
            static constexpr char Group[] = "Shader";

            //! The default shader variant (i.e. the one without any options set).
            static const ShaderVariantStableId RootShaderVariantStableId;

            // @subProductType is one of ShaderAssetSubId, or ShaderAssetSubId::FirstByProduct+
            static uint32_t MakeProductAssetSubId(uint32_t rhiApiUniqueIndex, uint32_t supervariantIndex, uint32_t subProductType);
            static SupervariantIndex GetSupervariantIndexFromProductAssetSubId(uint32_t assetProducSubId);
            static SupervariantIndex GetSupervariantIndexFromAssetId(const Data::AssetId& assetId);


            ShaderAsset() = default;
            ~ShaderAsset();

            AZ_DISABLE_COPY_MOVE(ShaderAsset);


            //! Returns the name of the shader.
            const Name& GetName() const;

            //! Returns the pipeline state type generated by variants of this shader.
            RHI::PipelineStateType GetPipelineStateType() const;

            //! Returns the draw list tag name.
            //! To get the corresponding DrawListTag use DrawListTagRegistry's FindTag() or AcquireTag() (see
            //! RHISystemInterface::GetDrawListTagRegistry()). The DrawListTag is also available in the Shader that corresponds to this
            //! ShaderAsset.
            const Name& GetDrawListName() const;

            //! Return the timestamp when the shader asset was built.
            //! This is used to synchronize versions of the ShaderAsset and ShaderVariantTreeAsset, especially during hot-reload.
            AZStd::sys_time_t GetShaderAssetBuildTimestamp() const;

            //! Returns the shader option group layout.
            const ShaderOptionGroupLayout* GetShaderOptionGroupLayout() const;

            //! Returns the supervariant index from the specified name.
            //! Note that this will append the system supervariant name from RPI::ShaderSystem when searching.
            SupervariantIndex GetSupervariantIndex(const AZ::Name& supervariantName) const;

            //! This function should be your one stop shop to get a ShaderVariantAsset.
            //! Finds and returns the best matching ShaderVariantAsset given a ShaderVariantId.
            //! If the ShaderVariantAsset is not fully loaded and ready at the moment, this function
            //! will QueueLoad the ShaderVariantTreeAsset and subsequently will QueueLoad the ShaderVariantAsset.
            //! The called will be notified via the ShaderVariantFinderNotificationBus when the
            //! ShaderVariantAsset is loaded and ready.
            //! In the mean time, if the required variant is not available this function
            //! returns the Root Variant.
            Data::Asset<ShaderVariantAsset> GetVariant(
                const ShaderVariantId& shaderVariantId, SupervariantIndex supervariantIndex);
            Data::Asset<ShaderVariantAsset> GetVariant(const ShaderVariantId& shaderVariantId) { return GetVariant(shaderVariantId, DefaultSupervariantIndex); }

            //! Finds the best matching shader variant and returns its StableId.
            //! This function first loads and caches the ShaderVariantTreeAsset (if not done before).
            //! If the ShaderVariantTreeAsset is not found (either the AssetProcessor has not generated it yet, or it simply doesn't exist), then
            //! it returns a search result that identifies the root variant.
            //! This function is thread safe.
            ShaderVariantSearchResult FindVariantStableId(const ShaderVariantId& shaderVariantId);

            //! Returns the variant asset associated with the provided StableId.
            //! The user should call FindVariantStableId() first to get a ShaderVariantStableId from a ShaderVariantId,
            //! Or better yet, call GetVariant(ShaderVariantId) for maximum convenience.
            //! If the requested variant is not found, the root variant will be returned AND the requested variant will be queued for loading.
            //! Next time around if the variant has been loaded this function will return it. Alternatively
            //! the caller can register with the ShaderVariantFinderNotificationBus to get the asset as soon as is available.
            //! This function is thread safe.
            Data::Asset<ShaderVariantAsset> GetVariant(
                ShaderVariantStableId shaderVariantStableId, SupervariantIndex supervariantIndex) const;
            Data::Asset<ShaderVariantAsset> GetVariant(ShaderVariantStableId shaderVariantStableId) const { return GetVariant(shaderVariantStableId, DefaultSupervariantIndex); }

            Data::Asset<ShaderVariantAsset> GetRootVariant(SupervariantIndex supervariantIndex) const;
            Data::Asset<ShaderVariantAsset> GetRootVariant() const { return GetRootVariant(DefaultSupervariantIndex); }


            //! Finds and returns the shader resource group asset with the requested name. Returns an empty handle if no matching group was found.
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& FindShaderResourceGroupLayout(
                const Name& shaderResourceGroupName, SupervariantIndex supervariantIndex) const;
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& FindShaderResourceGroupLayout(const Name& shaderResourceGroupName) const;

            //! Finds and returns the shader resource group layout associated with the requested binding slot. Returns an empty handle if no matching srg was found.
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& FindShaderResourceGroupLayout(
                uint32_t bindingSlot, SupervariantIndex supervariantIndex) const;
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& FindShaderResourceGroupLayout(uint32_t bindingSlot) const;

            //! Finds and returns the shader resource group layout designated as a ShaderVariantKey fallback.
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& FindFallbackShaderResourceGroupLayout( SupervariantIndex supervariantIndex) const;
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& FindFallbackShaderResourceGroupLayout() const
            {
                return FindFallbackShaderResourceGroupLayout(DefaultSupervariantIndex);
            }


            //! Returns the set of shader resource group layouts owned by a given supervariant.
            AZStd::array_view<RHI::Ptr<RHI::ShaderResourceGroupLayout>> GetShaderResourceGroupLayouts( SupervariantIndex supervariantIndex) const;
            AZStd::array_view<RHI::Ptr<RHI::ShaderResourceGroupLayout>> GetShaderResourceGroupLayouts() const
            {
                return GetShaderResourceGroupLayouts(DefaultSupervariantIndex);
            }

            //! Returns the pipeline layout descriptor shared by all variants in the asset.
            const RHI::PipelineLayoutDescriptor* GetPipelineLayoutDescriptor(SupervariantIndex supervariantIndex) const;
            const RHI::PipelineLayoutDescriptor* GetPipelineLayoutDescriptor() const
            {
                return GetPipelineLayoutDescriptor(DefaultSupervariantIndex);
            }

            //! Returns the shader resource group asset that has per-draw frequency, which is added to every draw packet.
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& GetDrawSrgLayout(SupervariantIndex supervariantIndex) const;
            const RHI::Ptr<RHI::ShaderResourceGroupLayout>& GetDrawSrgLayout() const
            {
                return GetDrawSrgLayout(DefaultSupervariantIndex);
            }


            //! Returns the ShaderInputContract which describes which inputs the shader requires
            const ShaderInputContract& GetInputContract(SupervariantIndex supervariantIndex) const;
            const ShaderInputContract& GetInputContract() const
            {
                return GetInputContract(DefaultSupervariantIndex);
            }


            //! Returns the ShaderOuputContract which describes which outputs the shader requires
            const ShaderOutputContract& GetOutputContract(SupervariantIndex supervariantIndex) const;
            const ShaderOutputContract& GetOutputContract() const
            {
                return GetOutputContract(DefaultSupervariantIndex);
            }


            //! Returns the render states for the draw pipeline. Only used for draw pipelines.
            const RHI::RenderStates& GetRenderStates(SupervariantIndex supervariantIndex) const;
            const RHI::RenderStates& GetRenderStates() const
            {
                return GetRenderStates(DefaultSupervariantIndex);
            }


            //! Returns a list of arguments for the specified attribute, or nullopt_t if the attribute is not found. The list can be empty which is still valid.
            AZStd::optional<RHI::ShaderStageAttributeArguments> GetAttribute(
                const RHI::ShaderStage& shaderStage, const Name& attributeName, SupervariantIndex supervariantIndex) const;
            AZStd::optional<RHI::ShaderStageAttributeArguments> GetAttribute(
                const RHI::ShaderStage& shaderStage, const Name& attributeName) const
            {
                return GetAttribute(shaderStage, attributeName, DefaultSupervariantIndex);
            }


        private:
            ///////////////////////////////////////////////////////////////////
            /// AssetBus overrides
            void OnAssetReloaded(Data::Asset<Data::AssetData> asset) override;
            void OnAssetReady(Data::Asset<Data::AssetData> asset) override;
            ///////////////////////////////////////////////////////////////////

            void ReinitializeRootShaderVariant(Data::Asset<Data::AssetData> asset);

            ///////////////////////////////////////////////////////////////////
            /// ShaderVariantFinderNotificationBus overrides
            void OnShaderVariantTreeAssetReady(Data::Asset<ShaderVariantTreeAsset> shaderVariantTreeAsset, bool isError) override;
            void OnShaderVariantAssetReady(Data::Asset<ShaderVariantAsset> /*shaderVariantAsset*/, bool /*isError*/) override {};
            ///////////////////////////////////////////////////////////////////

            //! A Supervariant represents a set of static shader compilation parameters.
            //! Those parameters can be predefined c-preprocessor macros or specific arguments
            //! for AZSLc.
            //! For each Supervariant there's a unique Root ShaderVariantAsset, and possibly an N amount
            //! of ShaderVariantAssets. The 'N' amount is the same across all Supervariants because all Supervariants
            //! share the same ShaderVariantTreeAsset.
            struct Supervariant
            {
                AZ_TYPE_INFO(Supervariant, "{850826EF-B267-4752-92F6-A85E4175CAB8}");
                static void Reflect(AZ::ReflectContext* context);

                AZ::Name m_name;
                ShaderResourceGroupLayoutList m_srgLayoutList;
                RHI::Ptr<RHI::PipelineLayoutDescriptor> m_pipelineLayoutDescriptor;
                ShaderInputContract m_inputContract;
                ShaderOutputContract m_outputContract;
                RHI::RenderStates m_renderStates;
                RHI::ShaderStageAttributeMapList m_attributeMaps;
                Data::Asset<ShaderVariantAsset> m_rootShaderVariantAsset;
            };

            //! Container of shader data that is specific to an RHI API.
            //! A ShaderAsset can contain shader data for multiple RHI APIs if
            //! the platform support multiple RHIs.
            struct ShaderApiDataContainer
            {
                AZ_TYPE_INFO(ShaderApiDataContainer, "{C636722C-60B9-421C-ACAD-9750BF634A27}");
                static void Reflect(AZ::ReflectContext* context);

                //! RHI API Type for this shader data.
                RHI::APIType m_APIType;
                // Index 0, will always be the default Supervariant. (see DefaultSupervariantIndex)
                AZStd::vector<Supervariant> m_supervariants;
            };

            bool PostLoadInit() override;
            void SetReady();

            //! SelectShaderApiData() must be called before most other ShaderAsset functions.
            bool SelectShaderApiData();

            //! Returns the active ShaderApiDataContainer which was selected in SelectShaderApiData().
            ShaderApiDataContainer& GetCurrentShaderApiData();
            const ShaderApiDataContainer& GetCurrentShaderApiData() const;

            //! Returning pointers instead of references to allow for error checking
            //! and not having to assert. 
            Supervariant* GetSupervariant(SupervariantIndex supervariantIndex);
            const Supervariant* GetSupervariant(SupervariantIndex supervariantIndex) const;

            //! Search for a supervariant index by name.
            SupervariantIndex GetSupervariantIndexInternal(AZ::Name supervariantName) const;

            //! The name is the stem of the source <name>.shader file.
            Name m_name;

            //! Dictates the type of pipeline state generated by this asset (Draw / Dispatch / etc.).
            //! All shader variants, across all supervariants, in the asset adhere to this type.
            RHI::PipelineStateType m_pipelineStateType = RHI::PipelineStateType::Count;

            //! Defines the layout of the shader options in the asset.
            Ptr<ShaderOptionGroupLayout> m_shaderOptionGroupLayout;

            //! List with shader data per RHI backend.
            AZStd::vector<ShaderApiDataContainer> m_perAPIShaderData;

            Name m_drawListName;

            //! Use to synchronize versions of the ShaderAsset and ShaderVariantTreeAsset, especially during hot-reload.
            AZStd::sys_time_t m_shaderAssetBuildTimestamp = 0; 


            ///////////////////////////////////////////////////////////////////
            //! Do Not Serialize!

            static constexpr size_t InvalidAPITypeIndex = std::numeric_limits<size_t>::max();

            //! Index that indicates which ShaderDataContainer to use.
            //! At runtime, the asset checks the current active RHI Backend
            //! and based on the results this variable gets set on asset load.
            //! The vector @m_perAPIShaderData will be indexed with this variable.
            size_t m_currentAPITypeIndex = InvalidAPITypeIndex;

            //! We can not know the ShaderVariantTreeAsset by the time this asset is being created.
            //! This is a value that is discovered at run time. It becomes valid when FindVariantStableId is called at least once.
            Data::Asset<ShaderVariantTreeAsset> m_shaderVariantTree;

            //! Used for thread safety for FindVariantStableId().
            mutable AZStd::shared_mutex m_variantTreeMutex;

            bool m_shaderVariantTreeLoadWasRequested = false;
        };

        class ShaderAssetHandler final
            : public AssetHandler<ShaderAsset>
        {
            using Base = AssetHandler<ShaderAsset>;
        public:
            ShaderAssetHandler() = default;

        private:
            Data::AssetHandler::LoadResult LoadAssetData(
                const Data::Asset<Data::AssetData>& asset,
                AZStd::shared_ptr<Data::AssetDataStream> stream,
                const Data::AssetFilterCB& assetLoadFilterCB) override;
        };

        //////////////////////////////////////////////////////////////////////////
    } // namespace RPI

} // namespace AZ
