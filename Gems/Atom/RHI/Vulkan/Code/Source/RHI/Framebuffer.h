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

#include <Atom/RHI/DeviceObject.h>
#include <Atom/RHI/ResourceInvalidateBus.h>
#include <AzCore/Memory/PoolAllocator.h>
#include <AzCore/std/containers/vector.h>
#include <RHI/ImageView.h>
#include <RHI/RenderPass.h>

namespace AZ
{
    namespace Vulkan
    {
        class Device;
        class ImageView;
        class RenderPass;

        class Framebuffer final
            : public RHI::DeviceObject
            , public RHI::ResourceInvalidateBus::MultiHandler
        {
            using Base = RHI::DeviceObject;

        public:
            AZ_CLASS_ALLOCATOR(Framebuffer, AZ::ThreadPoolAllocator, 0);
            AZ_RTTI(Framebuffer, "1EF7EE0F-CB6C-45EB-8D8A-8254F4AC5F67", Base);

            struct Descriptor
            {
                size_t GetHash() const;

                Device* m_device = nullptr;
                const RenderPass* m_renderPass = nullptr;
                AZStd::vector<const ImageView*> m_attachmentImageViews;
            };

            static RHI::Ptr<Framebuffer> Create();
            RHI::ResultCode Init(const Descriptor& descriptor);
            ~Framebuffer() = default;

            VkFramebuffer GetNativeFramebuffer() const;
            const RenderPass* GetRenderPass() const;

            const RHI::Size& GetSize() const;

        private:
            Framebuffer() = default;

            //////////////////////////////////////////////////////////////////////////
            // RHI::Object
            void SetNameInternal(const AZStd::string_view& name) override;
            //////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////
            // RHI::DeviceObject
            void Shutdown() override;
            //////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////
            // ResourceInvalidateBus::MultiHandler
            RHI::ResultCode OnResourceInvalidate() override final;
            //////////////////////////////////////////////////////////////////////////

            RHI::ResultCode BuildNativeFramebuffer();

            bool AreResourcesReady() const;
            void Invalidate();

            VkFramebuffer m_nativeFramebuffer = VK_NULL_HANDLE;
            AZStd::vector<RHI::ConstPtr<ImageView>> m_attachments;
            RHI::ConstPtr<RenderPass> m_renderPass;
        };
    }
}
