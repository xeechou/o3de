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

#include <Atom/RHI/IndirectBufferWriter.h>
#include <AzCore/Memory/PoolAllocator.h>

namespace AZ
{
    namespace Vulkan
    {
        //! Vulkan implementation of the helper RHI class IndirectBufferWriter.
        //! It writes indirect commands into a memory location following the layout of
        //! the Vulkan's commands.
        //! It only supports Tier1 commands.
        class IndirectBufferWriter final
            : public RHI::IndirectBufferWriter
        {
            using Base = RHI::IndirectBufferWriter;
        public:
            AZ_CLASS_ALLOCATOR(IndirectBufferWriter, AZ::ThreadPoolAllocator, 0);
            AZ_RTTI(IndirectBufferWriter, "{089BDED9-EDF3-4C72-9B52-57926DD29BBA}", Base);

            static RHI::Ptr<IndirectBufferWriter> Create();

        private:
            IndirectBufferWriter() = default;

            //////////////////////////////////////////////////////////////////////////
            // RHI::IndirectBufferWriter
            void SetVertexViewInternal(RHI::IndirectCommandIndex index, const RHI::StreamBufferView& view) override;
            void SetIndexViewInternal(RHI::IndirectCommandIndex index, const RHI::IndexBufferView& view) override;
            void DrawInternal(RHI::IndirectCommandIndex index, const RHI::DrawLinear& arguments) override;
            void DrawIndexedInternal(RHI::IndirectCommandIndex index, const RHI::DrawIndexed& arguments) override;
            void DispatchInternal(RHI::IndirectCommandIndex index, const RHI::DispatchDirect& arguments) override;
            void SetRootConstantsInternal(RHI::IndirectCommandIndex index, const uint8_t* data, uint32_t byteSize) override;
            //////////////////////////////////////////////////////////////////////////

            uint8_t* GetCommandMemoryMap(RHI::IndirectCommandIndex index) const;
        };
    }
}
