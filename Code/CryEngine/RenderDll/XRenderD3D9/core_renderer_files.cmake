#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#
# For complete copyright and license terms please see the LICENSE at the root of this
# distribution (the "License"). All use of this software is governed by the License,
# or, if provided, by the license below or the license accompanying this file. Do not
# remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#

set(FILES
    D3DAmbientOcclusion.cpp
    D3DColorGradingController.cpp
    D3DDebug.cpp
    D3DDeferredPasses.cpp
    D3DDeferredRender.cpp
    D3DDeferredShading.cpp
    D3DFont.cpp
    D3DFXPipeline.cpp
    D3DHDRRender.cpp
    D3DHMDRenderer.cpp
    D3DHWShader.cpp
    D3DHWShaderCompiling.cpp
    D3DOcclQuery.cpp
    D3D_SVO.cpp
    D3DRECloud.cpp
    D3DREImposter.cpp
    D3DRenderAuxGeom.cpp
    D3DRenderRE.cpp
    D3DRenderThread.cpp
    D3DRendPipeline.cpp
    D3DRendPipeline_Jobs.cpp
    D3DShaders.cpp
    D3DShadows.cpp
    D3DStereo.cpp
    D3DSystem.cpp
    D3DTexture.cpp
    D3DTexturesStreaming.cpp
    D3DTextureStreamPool.cpp
    D3DTiledShading.cpp
    GPUTimer.cpp
    GPUTimerFactory.cpp
    PipelineProfiler.cpp
    MultiLayerAlphaBlendPass.cpp
    D3DPostProcess.cpp
    PostProcessFilters.cpp
    PostProcessGame.cpp
    PostProcessMisc.cpp
    PostProcessMotionBlur.cpp
    PostProcessRain.cpp
    PostProcessSunShafts.cpp
    PostProcessWater.cpp
    PostProcessSnow.cpp
    RTT/RTTContext.cpp
    RTT/RTTContextManager.cpp
    RTT/RTTSwappableRenderTarget.cpp
    D3D_SVO.h
    D3DTiledShading.h
    ../Common/Textures/ITextureStreamer.h
    ../Common/Shaders/ShaderAllocator.h
    ../Common/CryNameR.h
    ../Common/FencedIB.h
    ../Common/FencedVB.h
    D3DColorGradingController.h
    D3DDebug.h
    D3DDeferredShading.h
    D3DHMDRenderer.h
    D3DHWShader.h
    D3DPostProcess.h
    D3DRenderAuxGeom.h
    D3DStereo.h
    ../Common/RendererDefs.h
    ../Common/Defs.h
    DeviceInfo.h
    GPUTimer.h
    GPUTimerFactory.h
    NullD3D11Device.h
    PipelineProfiler.h
    resource.h
    ../RenderDll_precompiled.h
    DeviceInfo.inl
    MultiLayerAlphaBlendPass.h
    RTT/RTTContext.h
    RTT/RTTContextManager.h
    RTT/RTTSwappableRenderTarget.h
    RTT/RTTSwappableCVar.h
    ../Common/Renderer_Jobs.cpp
    ../Common/CommonRender.cpp
    ../Common/DeferredRenderUtils.cpp
    ../Common/RenderAuxGeom.cpp
    ../Common/Renderer.cpp
    ../Common/RenderPipeline.cpp
    ../Common/RenderThread.cpp
    ../Common/ResFile.cpp
    ../Common/ResFileLookupDataMan.cpp
    ../Common/ShadowUtils.cpp
    ../Common/TextMessages.cpp
    ../Common/WaterUtils.cpp
    ../Common/ResFileLookupDataMan.h
    ../Common/TextMessages.h
    ../Common/PerInstanceConstantBufferPool.cpp
    ../Common/RenderView.cpp
    ../Common/CommonRender.h
    ../Common/DeferredRenderUtils.h
    ../Common/FrameProfiler.h
    ../Common/IColorGradingControllerInt.h
    ../Common/OcclQuery.h
    ../Common/RenderAuxGeom.h
    ../Common/Renderer.h
    ../Common/RenderCapabilities.h
    ../Common/RenderPipeline.h
    ../Common/RenderThread.h
    ../Common/ResFile.h
    ../Common/ResFile_info.h
    ../Common/Shadow_Renderer.h
    ../Common/ShadowUtils.h
    ../Common/WaterUtils.h
    ../Common/ReverseDepth.h
    ../Common/PerInstanceConstantBufferPool.h
    ../Common/RenderView.h
    ../Common/Include_HLSL_CPP_Shared.h
    ../Common/TypedConstantBuffer.h
    ../Common/RenderMesh_Render.cpp
    ../Common/RenderMesh_SetMesh.cpp
    ../Common/RenderMesh.cpp
    ../Common/RenderMesh.h
    ../Common/Shaders/Parser.cpp
    ../Common/Shaders/ParserBin.cpp
    ../Common/Shaders/PoundPoundParser.cpp
    ../Common/Shaders/RemoteCompiler.cpp
    ../Common/Shaders/ShaderCache.cpp
    ../Common/Shaders/ShaderComponents.cpp
    ../Common/Shaders/ShaderCore.cpp
    ../Common/Shaders/ShaderFXParseBin.cpp
    ../Common/Shaders/ShaderParse.cpp
    ../Common/Shaders/ShaderScript.cpp
    ../Common/Shaders/ShaderSerialize.cpp
    ../Common/Shaders/ShaderTemplate.cpp
    ../Common/Shaders/ShaderResources.cpp
    ../Common/Shaders/CShader.h
    ../Common/Shaders/CShaderBin.h
    ../Common/Shaders/Parser.h
    ../Common/Shaders/ParserBin.h
    ../Common/Shaders/PoundPoundParser.h
    ../Common/Shaders/RemoteCompiler.h
    ../Common/Shaders/Shader.h
    ../Common/Shaders/ShaderCache.h
    ../Common/Shaders/ShaderComponents.h
    ../Common/Shaders/ShaderSerialize.h
    ../Common/Shaders/ShaderResources.h
    ../Common/Shaders/Vertex.h
    ../Common/Shaders/ShadersResourcesGroups/PerFrame.h
    ../Common/PostProcess/PostEffects.cpp
    ../Common/PostProcess/PostProcess.cpp
    ../Common/PostProcess/PostProcessUtils.cpp
    ../Common/PostProcess/PostEffects.h
    ../Common/PostProcess/PostProcess.h
    ../Common/PostProcess/PostProcessUtils.h
    ../Common/Memory/VRAMDriller.cpp
    ../Common/Memory/VRAMDriller.h
    ../Common/Memory/VRAMDrillerBus.h
    DevBuffer.cpp
    DeviceManager/Base.h
    DeviceManager/Enums.h
    DeviceManager/PartitionTable.h
    DeviceManager/ConstantBufferCache.h
    DeviceManager/ConstantBufferCache.cpp
    DeviceManager/DeviceManager.cpp
    DeviceManager/DeviceManagerInline.h
    ../Common/DevBuffer.h
    DeviceManager/TempDynBuffer.h
    DeviceManager/DeviceManager.h
    DeviceManager/DeviceManager_D3D11.inl
    DeviceManager/DeviceManager_D3D11_NVAPI.h
    DeviceManager/DeviceManager_D3D11_NVAPI.inl
    DeviceManager/DeviceManager_D3D12.inl
    DeviceManager/DeviceWrapper12.h
    DeviceManager/DeviceWrapper12.cpp
    DeviceManager/DeviceWrapper12_D3D11.cpp
    DeviceManager/DeviceWrapper12_D3D12.cpp
    CRELensOpticsD3D.cpp
    ../Common/RendElements/CREBeam.cpp
    ../Common/RendElements/CREClientPoly.cpp
    ../Common/RendElements/CRECloud.cpp
    ../Common/RendElements/CREDeferredShading.cpp
    ../Common/RendElements/CREFogVolume.cpp
    ../Common/RendElements/CREGameEffect.cpp
    ../Common/RendElements/CREHDRProcess.cpp
    ../Common/RendElements/CREImposter.cpp
    ../Common/RendElements/CREMesh.cpp
    ../Common/RendElements/CREOclusionQuery.cpp
    ../Common/RendElements/CREGeomCache.cpp
    ../Common/RendElements/CREPostProcess.cpp
    ../Common/RendElements/CREPrismObject.cpp
    ../Common/RendElements/CRESky.cpp
    ../Common/RendElements/CREVolumeObject.cpp
    ../Common/RendElements/CREWaterOcean.cpp
    ../Common/RendElements/CREWaterVolume.cpp
    ../Common/RendElements/RendElement.cpp
    ../Common/RendElements/CRELensOptics.h
    ../Common/RendElements/CREBeam.h
    ../Common/RendElements/CREClientPoly.h
    ../Common/RendElements/CRECloud.h
    ../Common/RendElements/CREDeferredShading.h
    ../Common/RendElements/CREHDRProcess.h
    ../Common/RendElements/CREMeshImpl.h
    ../Common/RendElements/Stars.h
    ../Common/RendElements/Utils/PolygonMath2D.cpp
    ../Common/RendElements/Utils/PolygonMath2D.h
    ../Common/RendElements/Utils/SpatialHashGrid.h
    ../Common/RendElements/CameraOrbs.cpp
    ../Common/RendElements/AbstractMeshElement.cpp
    ../Common/RendElements/ChromaticRing.cpp
    ../Common/RendElements/Ghost.cpp
    ../Common/RendElements/Glow.cpp
    ../Common/RendElements/IrisShafts.cpp
    ../Common/RendElements/MeshUtil.cpp
    ../Common/RendElements/OpticsElement.cpp
    ../Common/RendElements/OpticsFactory.cpp
    ../Common/RendElements/RootOpticsElement.cpp
    ../Common/RendElements/Streaks.cpp
    ../Common/RendElements/FlareSoftOcclusionQuery.cpp
    ../Common/RendElements/ImageSpaceShafts.cpp
    ../Common/RendElements/AbstractMeshElement.h
    ../Common/RendElements/MeshUtil.h
    ../Common/RendElements/OpticsFactory.h
    ../Common/RendElements/Streaks.h
    ../Common/RendElements/Timeline.h
    ../Common/RendElements/CameraOrbs.h
    ../Common/RendElements/ChromaticRing.h
    ../Common/RendElements/Ghost.h
    ../Common/RendElements/Glow.h
    ../Common/RendElements/Interpolator.h
    ../Common/RendElements/IrisShafts.h
    ../Common/RendElements/OpticsElement.h
    ../Common/RendElements/OpticsPredef.hpp
    ../Common/RendElements/RootOpticsElement.h
    ../Common/RendElements/FlareSoftOcclusionQuery.h
    ../Common/RendElements/ImageSpaceShafts.h
    ../Common/RendElements/OpticsGroup.h
    ../Common/RendElements/OpticsGroup.cpp
    ../Common/RendElements/OpticsProxy.h
    ../Common/RendElements/OpticsProxy.cpp
    ../Common/RendElements/OpticsReference.h
    ../Common/RendElements/OpticsReference.cpp
    ../Common/Textures/ITextureStreamer.cpp
    ../Common/Textures/PlanningTextureStreamer.cpp
    ../Common/Textures/PlanningTextureStreamer_Jobs.cpp
    ../Common/Textures/DynTexture.cpp
    ../Common/Textures/PowerOf2BlockPacker.cpp
    ../Common/Textures/StereoTexture.cpp
    ../Common/Textures/StereoTexture.h
    ../Common/Textures/Texture.cpp
    ../Common/Textures/Texture.h
    ../Common/Textures/TextureManager.h
    ../Common/Textures/TextureManager.cpp
    ../Common/Textures/TextureStreaming.cpp
    ../Common/Textures/PlanningTextureStreamer.h
    ../Common/Textures/TextureArrayAlloc.h
    ../Common/Textures/TextureStreamPool.h
    ../Common/Textures/PowerOf2BlockPacker.h
    ../Common/Textures/TextureHelpers.cpp
    ../Common/Textures/TextureHelpers.h
    ../Common/Textures/Image/CImage.cpp
    ../Common/Textures/Image/DDSImage.cpp
    ../Common/Textures/Image/JpgImage.cpp
    ../Common/Textures/Image/TgaImage.cpp
    ../Common/Textures/Image/CImage.h
    ../Common/Textures/Image/DDSImage.h
    GraphicsPipeline/Common/FullscreenPass.h
    GraphicsPipeline/Common/FullscreenPass.cpp
    GraphicsPipeline/Common/GraphicsPipeline.h
    GraphicsPipeline/Common/GraphicsPipeline.cpp
    GraphicsPipeline/Common/GraphicsPipelinePass.h
    GraphicsPipeline/Common/UtilityPasses.h
    GraphicsPipeline/Common/UtilityPasses.cpp
    GraphicsPipeline/Common/GraphicsPipelineStateSet.h
    GraphicsPipeline/Common/GraphicsPipelineStateSet.cpp
    GraphicsPipeline/AutoExposure.h
    GraphicsPipeline/AutoExposure.cpp
    GraphicsPipeline/Bloom.h
    GraphicsPipeline/Bloom.cpp
    GraphicsPipeline/DepthOfField.h
    GraphicsPipeline/DepthOfField.cpp
    GraphicsPipeline/FurBendData.h
    GraphicsPipeline/FurBendData.cpp
    GraphicsPipeline/FurPasses.h
    GraphicsPipeline/FurPasses.cpp
    GraphicsPipeline/MotionBlur.h
    GraphicsPipeline/MotionBlur.cpp
    GraphicsPipeline/PostAA.h
    GraphicsPipeline/PostAA.cpp
    GraphicsPipeline/ScreenSpaceObscurance.h
    GraphicsPipeline/ScreenSpaceObscurance.cpp
    GraphicsPipeline/ScreenSpaceReflections.h
    GraphicsPipeline/ScreenSpaceReflections.cpp
    GraphicsPipeline/ScreenSpaceSSS.h
    GraphicsPipeline/ScreenSpaceSSS.cpp
    GraphicsPipeline/StandardGraphicsPipeline.h
    GraphicsPipeline/StandardGraphicsPipeline.cpp
    GraphicsPipeline/VideoRenderPass.h
    GraphicsPipeline/VideoRenderPass.cpp
    ../RenderDll_precompiled.cpp
    D3DVolumetricFog.cpp
    D3DVolumetricFog.h
)
