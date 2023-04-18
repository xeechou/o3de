#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Include/Atom/RPI.Public/AssetInitBus.h
    Include/Atom/RPI.Public/AssetQuality.h
    Include/Atom/RPI.Public/AssetTagBus.h
    Include/Atom/RPI.Public/Base.h
    Include/Atom/RPI.Public/BlockCompression.h
    Include/Atom/RPI.Public/Culling.h
    Include/Atom/RPI.Public/FeatureProcessor.h
    Include/Atom/RPI.Public/FeatureProcessorFactory.h
    Include/Atom/RPI.Public/MeshDrawPacket.h
    Include/Atom/RPI.Public/PipelinePassChanges.h
    Include/Atom/RPI.Public/PipelineState.h
    Include/Atom/RPI.Public/RenderPipeline.h
    Include/Atom/RPI.Public/RPISystem.h
    Include/Atom/RPI.Public/RPISystemInterface.h
    Include/Atom/RPI.Public/RPIUtils.h
    Include/Atom/RPI.Public/Scene.h
    Include/Atom/RPI.Public/SceneBus.h
    Include/Atom/RPI.Public/View.h
    Include/Atom/RPI.Public/ViewGroup.h
    Include/Atom/RPI.Public/ViewportContext.h
    Include/Atom/RPI.Public/ViewportContextBus.h
    Include/Atom/RPI.Public/ViewportContextManager.h
    Include/Atom/RPI.Public/ViewProviderBus.h
    Include/Atom/RPI.Public/VisibleObjectContext.h
    Include/Atom/RPI.Public/WindowContext.h
    Include/Atom/RPI.Public/WindowContextBus.h
    Include/Atom/RPI.Public/AuxGeom/AuxGeomDraw.h
    Include/Atom/RPI.Public/AuxGeom/AuxGeomFeatureProcessorInterface.h
    Include/Atom/RPI.Public/Buffer/Buffer.h
    Include/Atom/RPI.Public/Buffer/BufferPool.h
    Include/Atom/RPI.Public/Buffer/BufferSystem.h
    Include/Atom/RPI.Public/Buffer/BufferSystemInterface.h
    Include/Atom/RPI.Public/ColorManagement/TransformColor.h
    Include/Atom/RPI.Public/DynamicDraw/DynamicBuffer.h
    Include/Atom/RPI.Public/DynamicDraw/DynamicBufferAllocator.h
    Include/Atom/RPI.Public/DynamicDraw/DynamicDrawContext.h
    Include/Atom/RPI.Public/DynamicDraw/DynamicDrawSystem.h
    Include/Atom/RPI.Public/DynamicDraw/DynamicDrawInterface.h
    Include/Atom/RPI.Public/Image/AttachmentImage.h
    Include/Atom/RPI.Public/Image/AttachmentImagePool.h
    Include/Atom/RPI.Public/Image/ImageSystem.h
    Include/Atom/RPI.Public/Image/ImageSystemInterface.h
    Include/Atom/RPI.Public/Image/ImageTagSystemComponent.h
    Include/Atom/RPI.Public/Image/StreamingImage.h
    Include/Atom/RPI.Public/Image/StreamingImageContext.h
    Include/Atom/RPI.Public/Image/StreamingImageController.h
    Include/Atom/RPI.Public/Image/StreamingImagePool.h
    Include/Atom/RPI.Public/Material/Material.h
    Include/Atom/RPI.Public/Material/MaterialSystem.h
    Include/Atom/RPI.Public/Model/Model.h
    Include/Atom/RPI.Public/Model/ModelLod.h
    Include/Atom/RPI.Public/Model/ModelLodUtils.h
    Include/Atom/RPI.Public/Model/ModelSystem.h
    Include/Atom/RPI.Public/Model/ModelTagSystemComponent.h
    Include/Atom/RPI.Public/Model/UvStreamTangentBitmask.h
    Include/Atom/RPI.Public/Pass/AttachmentReadback.h
    Include/Atom/RPI.Public/Pass/ComputePass.h
    Include/Atom/RPI.Public/Pass/CopyPass.h
    Include/Atom/RPI.Public/Pass/FullscreenTrianglePass.h
    Include/Atom/RPI.Public/Pass/ParentPass.h
    Include/Atom/RPI.Public/Pass/Pass.h
    Include/Atom/RPI.Public/Pass/PassAttachment.h
    Include/Atom/RPI.Public/Pass/PassTree.h
    Include/Atom/RPI.Public/Pass/PassDefines.h
    Include/Atom/RPI.Public/Pass/PassFactory.h
    Include/Atom/RPI.Public/Pass/PassFilter.h
    Include/Atom/RPI.Public/Pass/PassLibrary.h
    Include/Atom/RPI.Public/Pass/PassSystem.h
    Include/Atom/RPI.Public/Pass/PassSystemInterface.h
    Include/Atom/RPI.Public/Pass/PassUtils.h
    Include/Atom/RPI.Public/Pass/RasterPass.h
    Include/Atom/RPI.Public/Pass/RenderPass.h
    Include/Atom/RPI.Public/Pass/MSAAResolvePass.h
    Include/Atom/RPI.Public/Pass/SlowClearPass.h
    Include/Atom/RPI.Public/Pass/Specific/DownsampleMipChainPass.h
    Include/Atom/RPI.Public/Pass/Specific/DownsampleSinglePassLuminancePass.h
    Include/Atom/RPI.Public/Pass/Specific/ImageAttachmentPreviewPass.h
    Include/Atom/RPI.Public/Pass/Specific/EnvironmentCubeMapPass.h
    Include/Atom/RPI.Public/Pass/Specific/MSAAResolveFullScreenPass.h
    Include/Atom/RPI.Public/Pass/Specific/RenderToTexturePass.h
    Include/Atom/RPI.Public/Pass/Specific/SelectorPass.h
    Include/Atom/RPI.Public/Pass/Specific/SwapChainPass.h
    Include/Atom/RPI.Public/Shader/Shader.h
    Include/Atom/RPI.Public/Shader/ShaderReloadNotificationBus.h
    Include/Atom/RPI.Public/Shader/ShaderVariant.h
    Include/Atom/RPI.Public/Shader/ShaderReloadDebugTracker.h
    Include/Atom/RPI.Public/Shader/ShaderResourceGroup.h
    Include/Atom/RPI.Public/Shader/ShaderResourceGroupPool.h
    Include/Atom/RPI.Public/Shader/ShaderSystem.h
    Include/Atom/RPI.Public/Shader/ShaderSystemInterface.h
    Include/Atom/RPI.Public/Shader/ShaderVariantAsyncLoader.h
    Include/Atom/RPI.Public/GpuQuery/GpuQuerySystem.h
    Include/Atom/RPI.Public/GpuQuery/GpuQuerySystemInterface.h
    Include/Atom/RPI.Public/GpuQuery/GpuQueryTypes.h
    Include/Atom/RPI.Public/GpuQuery/Query.h
    Include/Atom/RPI.Public/GpuQuery/QueryPool.h
    Include/Atom/RPI.Public/GpuQuery/TimestampQueryPool.h
    Include/Atom/RPI.Public/GpuQuery/GpuPassProfiler.h
    Include/Atom/RPI.Public/XR/XRRenderingInterface.h
    Source/RPI.Public/Culling.cpp
    Source/RPI.Public/FeatureProcessor.cpp
    Source/RPI.Public/FeatureProcessorFactory.cpp
    Source/RPI.Public/MeshDrawPacket.cpp
    Source/RPI.Public/PipelinePassChanges.cpp
    Source/RPI.Public/PipelineState.cpp
    Source/RPI.Public/RenderPipeline.cpp
    Source/RPI.Public/RPISystem.cpp
    Source/RPI.Public/RPIUtils.cpp
    Source/RPI.Public/Scene.cpp
    Source/RPI.Public/View.cpp
    Source/RPI.Public/ViewGroup.cpp
    Source/RPI.Public/ViewportContext.cpp
    Source/RPI.Public/ViewportContextManager.cpp
    Source/RPI.Public/VisibleObjectContext.cpp
    Source/RPI.Public/WindowContext.cpp
    Source/RPI.Public/AuxGeomFeatureProcessorInterface.cpp
    Source/RPI.Public/Buffer/Buffer.cpp
    Source/RPI.Public/Buffer/BufferPool.cpp
    Source/RPI.Public/Buffer/BufferSystem.cpp
    Source/RPI.Public/DynamicDraw/DynamicBuffer.cpp
    Source/RPI.Public/DynamicDraw/DynamicBufferAllocator.cpp
    Source/RPI.Public/DynamicDraw/DynamicDrawContext.cpp
    Source/RPI.Public/DynamicDraw/DynamicDrawSystem.cpp
    Source/RPI.Public/Image/AttachmentImage.cpp
    Source/RPI.Public/Image/AttachmentImagePool.cpp
    Source/RPI.Public/Image/ImageSystem.cpp
    Source/RPI.Public/Image/ImageTagSystemComponent.cpp
    Source/RPI.Public/Image/StreamingImage.cpp
    Source/RPI.Public/Image/StreamingImageContext.cpp
    Source/RPI.Public/Image/StreamingImageController.cpp
    Source/RPI.Public/Image/StreamingImagePool.cpp
    Source/RPI.Public/Material/Material.cpp
    Source/RPI.Public/Material/MaterialSystem.cpp
    Source/RPI.Public/Model/Model.cpp
    Source/RPI.Public/Model/ModelLod.cpp
    Source/RPI.Public/Model/ModelLodUtils.cpp
    Source/RPI.Public/Model/ModelSystem.cpp
    Source/RPI.Public/Model/ModelTagSystemComponent.cpp
    Source/RPI.Public/Model/UvStreamTangentBitmask.cpp
    Source/RPI.Public/Pass/AttachmentReadback.cpp
    Source/RPI.Public/Pass/ComputePass.cpp
    Source/RPI.Public/Pass/CopyPass.cpp
    Source/RPI.Public/Pass/FullscreenTrianglePass.cpp
    Source/RPI.Public/Pass/ParentPass.cpp
    Source/RPI.Public/Pass/Pass.cpp
    Source/RPI.Public/Pass/PassAttachment.cpp
    Source/RPI.Public/Pass/PassTree.cpp
    Source/RPI.Public/Pass/PassFactory.cpp
    Source/RPI.Public/Pass/PassFilter.cpp
    Source/RPI.Public/Pass/PassLibrary.cpp
    Source/RPI.Public/Pass/PassSystem.cpp
    Source/RPI.Public/Pass/PassUtils.cpp
    Source/RPI.Public/Pass/RasterPass.cpp
    Source/RPI.Public/Pass/RenderPass.cpp
    Source/RPI.Public/Pass/MSAAResolvePass.cpp
    Source/RPI.Public/Pass/SlowClearPass.cpp
    Source/RPI.Public/Pass/Specific/DownsampleMipChainPass.cpp
    Source/RPI.Public/Pass/Specific/DownsampleSinglePassLuminancePass.cpp
    Source/RPI.Public/Pass/Specific/ImageAttachmentPreviewPass.cpp
    Source/RPI.Public/Pass/Specific/EnvironmentCubeMapPass.cpp
    Source/RPI.Public/Pass/Specific/MSAAResolveFullScreenPass.cpp
    Source/RPI.Public/Pass/Specific/RenderToTexturePass.cpp
    Source/RPI.Public/Pass/Specific/SelectorPass.cpp
    Source/RPI.Public/Pass/Specific/SwapChainPass.cpp
    Source/RPI.Public/Shader/Shader.cpp
    Source/RPI.Public/Shader/ShaderVariant.cpp
    Source/RPI.Public/Shader/ShaderReloadDebugTracker.cpp
    Source/RPI.Public/Shader/ShaderResourceGroup.cpp
    Source/RPI.Public/Shader/ShaderResourceGroupPool.cpp
    Source/RPI.Public/Shader/ShaderSystem.cpp
    Source/RPI.Public/Shader/ShaderVariantAsyncLoader.cpp
    Source/RPI.Public/ColorManagement/GeneratedTransforms/ColorConversionConstants.inl
    Source/RPI.Public/ColorManagement/GeneratedTransforms/LinearSrgb_To_AcesCg.inl
    Source/RPI.Public/ColorManagement/GeneratedTransforms/AcesCg_To_LinearSrgb.inl
    Source/RPI.Public/ColorManagement/GeneratedTransforms/XYZ_To_AcesCg.inl
    Source/RPI.Public/ColorManagement/TransformColor.cpp
    Source/RPI.Public/GpuQuery/GpuQuerySystem.cpp
    Source/RPI.Public/GpuQuery/GpuQueryTypes.cpp
    Source/RPI.Public/GpuQuery/Query.cpp
    Source/RPI.Public/GpuQuery/QueryPool.cpp
    Source/RPI.Public/GpuQuery/TimestampQueryPool.cpp
    Source/RPI.Public/GpuQuery/GpuPassProfiler.cpp
)