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
    Include/Atom/RHI.Reflect/AttachmentEnums.h
    Include/Atom/RHI.Reflect/AttachmentId.h
    Include/Atom/RHI.Reflect/UnifiedAttachmentDescriptor.h
    Source/RHI.Reflect/AttachmentEnums.cpp
    Source/RHI.Reflect/UnifiedAttachmentDescriptor.cpp
    Include/Atom/RHI.Reflect/Base.h
    Include/Atom/RHI.Reflect/Bits.h
    Include/Atom/RHI.Reflect/ClearValue.h
    Include/Atom/RHI.Reflect/Format.h
    Include/Atom/RHI.Reflect/Handle.h
    Include/Atom/RHI.Reflect/Interval.h
    Include/Atom/RHI.Reflect/NameIdReflectionMap.h
    Include/Atom/RHI.Reflect/Origin.h
    Include/Atom/RHI.Reflect/Size.h
    Source/RHI.Reflect/Base.cpp
    Source/RHI.Reflect/ClearValue.cpp
    Source/RHI.Reflect/Format.cpp
    Source/RHI.Reflect/Interval.cpp
    Source/RHI.Reflect/Origin.cpp
    Source/RHI.Reflect/Size.cpp
    Include/Atom/RHI.Reflect/BufferDescriptor.h
    Include/Atom/RHI.Reflect/BufferViewDescriptor.h
    Source/RHI.Reflect/BufferDescriptor.cpp
    Source/RHI.Reflect/BufferViewDescriptor.cpp
    Include/Atom/RHI.Reflect/BufferPoolDescriptor.h
    Source/RHI.Reflect/BufferPoolDescriptor.cpp
    Include/Atom/RHI.Reflect/Limits.h
    Include/Atom/RHI.Reflect/Scissor.h
    Include/Atom/RHI.Reflect/Viewport.h
    Source/RHI.Reflect/Scissor.cpp
    Source/RHI.Reflect/Viewport.cpp
    Include/Atom/RHI.Reflect/DeviceDescriptor.h
    Include/Atom/RHI.Reflect/DeviceFeatures.h
    Include/Atom/RHI.Reflect/DeviceLimits.h
    Source/RHI.Reflect/DeviceDescriptor.cpp
    Include/Atom/RHI.Reflect/FrameSchedulerEnums.h
    Include/Atom/RHI.Reflect/ImageDescriptor.h
    Include/Atom/RHI.Reflect/ImageSubresource.h
    Include/Atom/RHI.Reflect/ImageViewDescriptor.h
    Source/RHI.Reflect/ImageDescriptor.cpp
    Source/RHI.Reflect/ImageSubresource.cpp
    Source/RHI.Reflect/ImageViewDescriptor.cpp
    Include/Atom/RHI.Reflect/ImagePoolDescriptor.h
    Include/Atom/RHI.Reflect/StreamingImagePoolDescriptor.h
    Source/RHI.Reflect/ImagePoolDescriptor.cpp
    Source/RHI.Reflect/StreamingImagePoolDescriptor.cpp
    Include/Atom/RHI.Reflect/IndirectBufferLayout.h
    Source/RHI.Reflect/IndirectBufferLayout.cpp
    Include/Atom/RHI.Reflect/MemoryEnums.h
    Include/Atom/RHI.Reflect/MemoryUsage.h
    Source/RHI.Reflect/MemoryUsage.cpp
    Include/Atom/RHI.Reflect/PhysicalDeviceDescriptor.h
    Source/RHI.Reflect/PhysicalDeviceDescriptor.cpp
    Include/Atom/RHI.Reflect/InputStreamLayout.h
    Include/Atom/RHI.Reflect/InputStreamLayoutBuilder.h
    Include/Atom/RHI.Reflect/MultisampleState.h
    Include/Atom/RHI.Reflect/RenderAttachmentLayout.h
    Include/Atom/RHI.Reflect/RenderAttachmentLayoutBuilder.h
    Include/Atom/RHI.Reflect/PipelineLibraryData.h
    Include/Atom/RHI.Reflect/RenderStates.h
    Include/Atom/RHI.Reflect/SamplerState.h
    Include/Atom/RHI.Reflect/ShaderSemantic.h
    Source/RHI.Reflect/InputStreamLayout.cpp
    Source/RHI.Reflect/InputStreamLayoutBuilder.cpp
    Source/RHI.Reflect/MultisampleState.cpp
    Source/RHI.Reflect/RenderAttachmentLayout.cpp
    Source/RHI.Reflect/RenderAttachmentLayoutBuilder.cpp
    Source/RHI.Reflect/PipelineLibraryData.cpp
    Source/RHI.Reflect/RenderStates.cpp
    Source/RHI.Reflect/SamplerState.cpp
    Source/RHI.Reflect/ShaderSemantic.cpp
    Include/Atom/RHI.Reflect/QueryPoolDescriptor.h
    Source/RHI.Reflect/QueryPoolDescriptor.cpp
    Include/Atom/RHI.Reflect/ResourcePoolDescriptor.h
    Source/RHI.Reflect/ResourcePoolDescriptor.cpp
    Include/Atom/RHI.Reflect/ScopeId.h
    Include/Atom/RHI.Reflect/AttachmentLoadStoreAction.h
    Include/Atom/RHI.Reflect/BufferScopeAttachmentDescriptor.h
    Include/Atom/RHI.Reflect/ImageScopeAttachmentDescriptor.h
    Include/Atom/RHI.Reflect/ResolveScopeAttachmentDescriptor.h
    Include/Atom/RHI.Reflect/ScopeAttachmentDescriptor.h
    Include/Atom/RHI.Reflect/UnifiedScopeAttachmentDescriptor.h
    Source/RHI.Reflect/AttachmentLoadStoreAction.cpp
    Source/RHI.Reflect/BufferScopeAttachmentDescriptor.cpp
    Source/RHI.Reflect/ImageScopeAttachmentDescriptor.cpp
    Source/RHI.Reflect/ResolveScopeAttachmentDescriptor.cpp
    Source/RHI.Reflect/ScopeAttachmentDescriptor.cpp
    Source/RHI.Reflect/UnifiedScopeAttachmentDescriptor.cpp
    Include/Atom/RHI.Reflect/ConstantsLayout.h
    Include/Atom/RHI.Reflect/PipelineLayoutDescriptor.h
    Include/Atom/RHI.Reflect/ShaderStages.h
    Include/Atom/RHI.Reflect/ShaderStageFunction.h
    Source/RHI.Reflect/ConstantsLayout.cpp
    Source/RHI.Reflect/PipelineLayoutDescriptor.cpp
    Source/RHI.Reflect/ShaderStageFunction.cpp
    Include/Atom/RHI.Reflect/ShaderDataMappings.h
    Include/Atom/RHI.Reflect/ShaderResourceGroupLayout.h
    Include/Atom/RHI.Reflect/ShaderResourceGroupLayoutDescriptor.h
    Include/Atom/RHI.Reflect/ShaderResourceGroupPoolDescriptor.h
    Source/RHI.Reflect/ShaderDataMappings.cpp
    Source/RHI.Reflect/ShaderResourceGroupLayout.cpp
    Source/RHI.Reflect/ShaderResourceGroupLayoutDescriptor.cpp
    Source/RHI.Reflect/ShaderResourceGroupPoolDescriptor.cpp
    Include/Atom/RHI.Reflect/CpuTimingStatistics.h
    Include/Atom/RHI.Reflect/MemoryStatistics.h
    Include/Atom/RHI.Reflect/TransientAttachmentStatistics.h
    Include/Atom/RHI.Reflect/SwapChainDescriptor.h
    Source/RHI.Reflect/SwapChainDescriptor.cpp
    Include/Atom/RHI.Reflect/ReflectSystemComponent.h
    Include/Atom/RHI.Reflect/RHISystemDescriptor.h
    Source/RHI.Reflect/ReflectSystemComponent.cpp
    Source/RHI.Reflect/RHISystemDescriptor.cpp
    Include/Atom/RHI.Reflect/AliasedHeapEnums.h
    Include/Atom/RHI.Reflect/TransientBufferDescriptor.h
    Include/Atom/RHI.Reflect/TransientImageDescriptor.h
    Source/RHI.Reflect/AliasedHeapEnums.cpp
    Source/RHI.Reflect/TransientBufferDescriptor.cpp
    Source/RHI.Reflect/TransientImageDescriptor.cpp
    Include/Atom/RHI.Reflect/ImageEnums.h
    Include/Atom/RHI.Reflect/PlatformLimitsDescriptor.h
    Source/RHI.Reflect/PlatformLimitsDescriptor.cpp
)
