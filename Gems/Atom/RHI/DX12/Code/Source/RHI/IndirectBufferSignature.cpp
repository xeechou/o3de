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
#include "RHI/Atom_RHI_DX12_precompiled.h"
#include <RHI/IndirectBufferSignature.h>
#include <RHI/Device.h>
#include <RHI/Buffer.h>

namespace AZ
{
    namespace DX12
    {
        RHI::Ptr<IndirectBufferSignature> IndirectBufferSignature::Create()
        {
            return aznew IndirectBufferSignature();
        }

        ID3D12CommandSignature* IndirectBufferSignature::Get() const
        {
            return m_signature.get();
        }

        RHI::ResultCode IndirectBufferSignature::InitInternal(RHI::Device& deviceBase, const RHI::IndirectBufferSignatureDescriptor& descriptor)
        {
            auto& device = static_cast<Device&>(deviceBase);
            auto commands = descriptor.m_layout.GetCommands();

            const PipelineState* pipelineState = static_cast<const PipelineState*>(descriptor.m_pipelineState);

            // Calculate the offset of the commands and the stride of the whole sequence.
            AZStd::vector<D3D12_INDIRECT_ARGUMENT_DESC> argumentsDesc(commands.size());
            m_stride = 0;
            m_offsets.resize(commands.size());
            for (uint32_t i = 0; i < commands.size(); ++i)
            {
                m_offsets[i] = m_stride;
                const RHI::IndirectCommandDescriptor& command = commands[i];
                D3D12_INDIRECT_ARGUMENT_DESC& argDesc = argumentsDesc[i];
                switch (command.m_type)
                {
                case RHI::IndirectCommandType::Draw:
                    argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
                    m_stride += sizeof(D3D12_DRAW_ARGUMENTS);
                    break;
                case RHI::IndirectCommandType::DrawIndexed:
                    argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
                    m_stride += sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
                    break;
                case RHI::IndirectCommandType::Dispatch:
                    argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
                    m_stride += sizeof(D3D12_DISPATCH_ARGUMENTS);
                    break;
                case RHI::IndirectCommandType::VertexBufferView:
                    argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
                    argDesc.VertexBuffer.Slot = command.m_vertexBufferArgs.m_slot;
                    m_stride += sizeof(D3D12_VERTEX_BUFFER_VIEW);
                    break;
                case RHI::IndirectCommandType::IndexBufferView:
                    argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW;
                    m_stride += sizeof(D3D12_INDEX_BUFFER_VIEW);
                    break;
                case RHI::IndirectCommandType::RootConstants:
                {
                    if (!pipelineState)
                    {
                        AZ_Assert(pipelineState, "PipelineState is required when using inline constant commands");
                        return RHI::ResultCode::InvalidArgument;
                    }

                    const PipelineLayoutDescriptor& pipelineLayoutDesc = static_cast<const PipelineLayoutDescriptor&>(pipelineState->GetPipelineLayout().GetPipelineLayoutDescriptor());
                    argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT;
                    argDesc.Constant.DestOffsetIn32BitValues = 0;
                    argDesc.Constant.Num32BitValuesToSet = pipelineLayoutDesc.GetRootConstantBinding().m_constantCount;
                    argDesc.Constant.RootParameterIndex = pipelineState->GetPipelineLayout().GetRootConstantsRootParameterIndex().GetIndex();

                    m_stride += sizeof(uint32_t) * argDesc.Constant.Num32BitValuesToSet;
                    break;
                }
                default:
                    AZ_Assert(false, "Invalid indirect argument type");
                    return RHI::ResultCode::InvalidArgument;
                }
            }

            ID3D12RootSignature* rootSignature = pipelineState ? pipelineState->GetPipelineLayout().Get() : nullptr;

            D3D12_COMMAND_SIGNATURE_DESC desc = {};
            desc.ByteStride = m_stride;
            desc.NumArgumentDescs = static_cast<UINT>(argumentsDesc.size());
            desc.pArgumentDescs = argumentsDesc.data();

            Microsoft::WRL::ComPtr<ID3D12CommandSignature> signatureComPtr;
            HRESULT  hr = device.GetDevice()->CreateCommandSignature(&desc, rootSignature, IID_GRAPHICS_PPV_ARGS(signatureComPtr.GetAddressOf()));
            if (!AssertSuccess(hr))
            {
                return RHI::ResultCode::Fail;
            }

            m_signature = signatureComPtr.Get();

            return RHI::ResultCode::Success;
        }

        uint32_t IndirectBufferSignature::GetByteStrideInternal() const
        {
            return m_stride;
        }

        void IndirectBufferSignature::ShutdownInternal()
        {
            m_signature = nullptr;
            m_stride = 0;
        }

        uint32_t IndirectBufferSignature::GetOffsetInternal(RHI::IndirectCommandIndex index) const
        {
            return m_offsets[index.GetIndex()];
        }
    }
}
