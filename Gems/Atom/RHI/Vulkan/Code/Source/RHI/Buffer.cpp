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
#include <AzCore/std/algorithm.h>
#include <AzCore/std/parallel/lock.h>
#include <AzCore/std/sort.h>
#include <Atom/RHI/BufferView.h>
#include <Atom/RHI/MemoryStatisticsBuilder.h>
#include <Atom/RHI.Reflect/BufferDescriptor.h>
#include <RHI/Buffer.h>
#include <RHI/BufferPool.h>
#include <RHI/Conversion.h>
#include <RHI/MemoryView.h>
#include <RHI/MemoryAllocator.h>
#include <RHI/Device.h>
#include <RHI/Queue.h>
#include <RHI/ReleaseContainer.h>

namespace AZ
{
    namespace Vulkan
    {
        RHI::Ptr<Buffer> Buffer::Create()
        {
            return aznew Buffer();
        }

        Buffer::~Buffer()
        {
            Invalidate();
        }

        void Buffer::Invalidate()
        {
            AZ_Assert(!m_memoryView.IsValid(), "Memory has not been deallocated");
            m_memoryView = BufferMemoryView();
            m_ownerQueue.Reset();
        }

        const BufferMemoryView* Buffer::GetBufferMemoryView() const
        {
            return &m_memoryView;
        }

        BufferMemoryView* Buffer::GetBufferMemoryView()
        {
            return &m_memoryView;
        }

        RHI::ResultCode Buffer::Init(Device& device, const RHI::BufferDescriptor& bufferDescriptor, BufferMemoryView& memmoryView)
        {
            DeviceObject::Init(device);
            m_ownerQueue.Init(bufferDescriptor);
            m_memoryView = memmoryView;

            SetName(GetName());
            return RHI::ResultCode::Success;
        }        

        AZStd::vector<Buffer::SubresourceRangeOwner> Buffer::GetOwnerQueue(const RHI::BufferSubresourceRange* range /*= nullptr*/) const
        {
            AZStd::lock_guard<AZStd::mutex> lock(m_ownerQueueMutex);
            return m_ownerQueue.Get(range ? *range : RHI::BufferSubresourceRange(GetDescriptor()));
        }

        AZStd::vector<Buffer::SubresourceRangeOwner> Buffer::GetOwnerQueue(const RHI::BufferView& bufferView) const
        {
            auto range = RHI::BufferSubresourceRange(bufferView.GetDescriptor());
            return GetOwnerQueue(&range);
        }

        void Buffer::SetOwnerQueue(const QueueId& queueId, const RHI::BufferSubresourceRange* range /*= nullptr*/)
        {
            AZStd::lock_guard<AZStd::mutex> lock(m_ownerQueueMutex);
            m_ownerQueue.Set(range ? *range : RHI::BufferSubresourceRange(GetDescriptor()), queueId);
        }

        void Buffer::SetOwnerQueue(const QueueId& queueId, const RHI::BufferView& bufferView)
        {
            auto range = RHI::BufferSubresourceRange(bufferView.GetDescriptor());
            return SetOwnerQueue(queueId , &range);
        }

        void Buffer::SetUploadHandle(const RHI::AsyncWorkHandle& handle)
        {
            m_uploadHandle = handle;
        }

        const RHI::AsyncWorkHandle& Buffer::GetUploadHandle() const
        {
            return m_uploadHandle;
        }

        void Buffer::SetNameInternal(const AZStd::string_view& name)
        {
            if (m_memoryView.IsValid())
            {
                m_memoryView.SetName(name);
            }
        }

        void Buffer::ReportMemoryUsage(RHI::MemoryStatisticsBuilder& builder) const
        {
            const RHI::BufferDescriptor& descriptor = GetDescriptor();

            RHI::MemoryStatistics::Buffer* bufferStats = builder.AddBuffer();
            bufferStats->m_name = GetName();
            bufferStats->m_bindFlags = descriptor.m_bindFlags;
            bufferStats->m_sizeInBytes = m_memoryView.GetSize();
        }
    }
}
