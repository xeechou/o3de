/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates, or
* a third party where indicated.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include "Atom_RHI_Metal_precompiled.h"

#import <QuartzCore/CAMetalLayer.h>
#include <RHI/Buffer.h>
#include <RHI/BufferPool.h>
#include <RHI/MetalView_Platform.h>
#include <RHI/PhysicalDevice.h>

namespace Platform
{
    AZ::RHI::PhysicalDeviceType GetPhysicalDeviceType(id<MTLDevice> mtlDevice)
    {
        return AZ::RHI::PhysicalDeviceType::GpuIntegrated;
    }

    AZ::RHI::PhysicalDeviceList EnumerateDevices()
    {
        AZ::RHI::PhysicalDeviceList physicalDeviceList;
        AZ::Metal::PhysicalDevice* physicalDevice = aznew AZ::Metal::PhysicalDevice;
        physicalDevice->Init(nil);
        physicalDeviceList.emplace_back(physicalDevice);
        return physicalDeviceList;
    }

    void PresentInternal(id <MTLCommandBuffer> mtlCommandBuffer, id<CAMetalDrawable> drawable, float syncInterval)
    {
        bool hasPresentAfterMinimumDuration = [drawable respondsToSelector:@selector(presentAfterMinimumDuration:)];
                
        if (hasPresentAfterMinimumDuration && syncInterval > 0.0f)
        {
            [mtlCommandBuffer presentDrawable:drawable afterMinimumDuration:syncInterval];
        }
        else
        {
            [mtlCommandBuffer presentDrawable:drawable];
        }
    }

    CGRect GetScreenBounds(NativeWindowType* nativeWindow)
    {
        AZ_UNUSED(nativeWindow);
        NativeScreenType* nativeScreen = [NativeScreenType mainScreen];
        return [nativeScreen bounds];
    }

    CGFloat GetScreenScale()
    {
        NativeScreenType* nativeScreen = [NativeScreenType mainScreen];
        return [nativeScreen scale];
    }

    void AttachViewController(NativeWindowType* nativeWindow, NativeViewControllerType* viewController, RHIMetalView* metalView)
    {
        AZ_UNUSED(metalView);
        nativeWindow.rootViewController = viewController;
    }

    void UnAttachViewController(NativeWindowType* nativeWindow, NativeViewControllerType* viewController)
    {
        if (nativeWindow.rootViewController == viewController)
        {
            nativeWindow.rootViewController = nil;
        }
        [viewController setView : nil];
    }

    void ResizeInternal(RHIMetalView* metalView, CGSize viewSize)
    {
        AZ_UNUSED(metalView);
        AZ_UNUSED(viewSize);
    }

    RHIMetalView* GetMetalView(NativeWindowType* nativeWindow)
    {
        return reinterpret_cast<RHIMetalView*>([nativeWindow.rootViewController view]);
    }

    void ApplyTileDimentions(MTLRenderPassDescriptor* mtlRenderPassDescriptor)
    {
        //Metal driver has a bug where if the tile dimensions changes between passes it will
        //generate incorrect vertex positions (possible vertex invariance). For example vertex invariance was
        //observed between Depth pass and forward pass. Hence for now we are setting global tile dimentions across all passes.
        //For performance sake we should eventually remove this once this bug is addressed.
        //[GFX_TODO][ATOM-13440] - Remove once driver bug is addressed.
        mtlRenderPassDescriptor.tileWidth = 16;
        mtlRenderPassDescriptor.tileHeight = 16;
    }

    void SynchronizeBufferOnCPU(id<MTLBuffer> mtlBuffer, size_t bufferOffset, size_t bufferSize)
    {
        //No synchronization needed as ios uses shared memory and does not support MTLStorageModeManaged
        AZ_UNUSED(mtlBuffer);
        AZ_UNUSED(bufferOffset);
        AZ_UNUSED(bufferSize);
    }

    void SynchronizeBufferOnGPU(id<MTLBlitCommandEncoder> blitEncoder, id<MTLBuffer> mtlBuffer)
    {
        //No synchronization needed as ios uses shared memory and does not support MTLStorageModeManaged
        AZ_UNUSED(blitEncoder);
        AZ_UNUSED(mtlBuffer);
    }

    void SynchronizeTextureOnGPU(id<MTLBlitCommandEncoder> blitEncoder, id<MTLTexture> mtlTexture)
    {
        //No synchronization needed as ios uses shared memory and does not support MTLStorageModeManaged
        AZ_UNUSED(blitEncoder);
        AZ_UNUSED(mtlTexture);
    }

    AZ::RHI::ResultCode MapBufferInternal(const AZ::RHI::BufferMapRequest& request, AZ::RHI::BufferMapResponse& response)
    {
        //No need to do anything here as ios does not support MTLStorageModeManaged
        AZ_UNUSED(request);
        AZ_UNUSED(response);
        return AZ::RHI::ResultCode::Success;
    }
    
    void UnMapBufferInternal(AZ::RHI::Buffer& bufferBase)
    {
        //No need to do anything here as ios does not support MTLStorageModeManaged
        AZ_UNUSED(bufferBase);
    }
}


