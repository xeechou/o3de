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

#include <AzCore/Memory/SystemAllocator.h>

#include <Atom/RPI.Reflect/Pass/RenderToTexturePassData.h>

#include <Atom/RPI.Public/Pass/AttachmentReadback.h>
#include <Atom/RPI.Public/Pass/ParentPass.h>

namespace AZ
{
    namespace RPI
    {
        //! RenderToTexturePass creates a render target and a child pass tree then have the child pass tree render to this render target.
        //! The RenderToTexturePass's pass descriptor defines the render target's size and format and the child pass's template name.
        //! RenderToTexturePass can also read back the render target.
        //! This is useful to render a render pipeline to a render target and (optional) read back its data to cpu memory for later use. 
        class RenderToTexturePass final
            : public ParentPass
        {
        public:
            AZ_RTTI(RenderToTexturePass, "{4FBA3461-A072-4538-84D1-311D2756B27E}", ParentPass);
            AZ_CLASS_ALLOCATOR(RenderToTexturePass, SystemAllocator, 0);

            RenderToTexturePass(const PassDescriptor& descriptor);
            ~RenderToTexturePass();

            // Pass class need to have its own Create function for its PassFactory Creator
            static Ptr<RenderToTexturePass> Create(const PassDescriptor& descriptor);

            Ptr<ParentPass> Recreate() const override;
            
            void ReadbackOutput(AZStd::shared_ptr<AttachmentReadback> readback);

            void ResizeOutput(uint32_t width, uint32_t height);

        protected:
            // Pass behavior overrides
            void BuildAttachmentsInternal() override;
            void FrameBeginInternal(FramePrepareParams params) override;

            // Function to be called when output size changed
            void OnUpdateOutputSize();

        private:
            using Base = ParentPass;

            RHI::Scissor m_scissor;
            RHI::Viewport m_viewport;

            // The master child pass used to drive rendering
            Ptr<Pass> m_childPass = nullptr;

            // Name of the template used to create the child pass. Needed for Recreate()
            Name m_childTemplateName;

            // For read back output
            AZStd::shared_ptr<AttachmentReadback> m_readback;

            Ptr<PassAttachment> m_outputAttachment;

            // saved settings for this pass
            RenderToTexturePassData m_passData;
        };

    }   // namespace RPI
}   // namespace AZ
