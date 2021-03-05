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
#include "Atom_RHI_Vulkan_precompiled.h"
#include <RHI/IndirectBufferSignature.h>

namespace AZ
{
    namespace Vulkan
    {
        RHI::Ptr<IndirectBufferSignature> IndirectBufferSignature::Create()
        {
            return aznew IndirectBufferSignature();
        }

        RHI::ResultCode IndirectBufferSignature::InitInternal([[maybe_unused]] RHI::Device& deviceBase, const RHI::IndirectBufferSignatureDescriptor& descriptor)
        {
            // Vulkan doesn't have an object to represent an indirect buffer signature.
            // We just calculate the offsets of the commands and the stride of the whole sequence.
            auto commands = descriptor.m_layout.GetCommands();

            m_stride = 0;
            m_offsets.resize(commands.size());
            for (uint32_t i = 0; i < commands.size(); ++i)
            {
                m_offsets[i] = m_stride;
                const RHI::IndirectCommandDescriptor& command = commands[i];
                switch (command.m_type)
                {
                case RHI::IndirectCommandType::Draw:
                    m_stride += sizeof(VkDrawIndirectCommand);
                    break;
                case RHI::IndirectCommandType::DrawIndexed:
                    m_stride += sizeof(VkDrawIndexedIndirectCommand);
                    break;
                case RHI::IndirectCommandType::Dispatch:
                    m_stride += sizeof(VkDispatchIndirectCommand);
                    break;
                default:
                    // Unsupported command types.
                    AZ_Assert(false, "Unsupported indirect command type (%d)", command.m_type);
                    return RHI::ResultCode::InvalidArgument;
                }
            }

            return RHI::ResultCode::Success;
        }

        uint32_t IndirectBufferSignature::GetByteStrideInternal() const
        {
            return m_stride;
        }

        void IndirectBufferSignature::ShutdownInternal()
        {
            m_stride = 0;
        }

        uint32_t IndirectBufferSignature::GetOffsetInternal(RHI::IndirectCommandIndex index) const
        {
            return m_offsets[index.GetIndex()];
        }
    }
}
