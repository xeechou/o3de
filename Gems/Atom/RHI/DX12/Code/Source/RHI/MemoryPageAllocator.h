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

#include <Atom/RHI/ObjectPool.h>
#include <Atom/RHI.Reflect/MemoryUsage.h>
#include <Atom/RHI.Reflect/BufferDescriptor.h>
#include <RHI/Memory.h>

namespace AZ
{
    namespace DX12
    {
        class Device;

        class MemoryPageFactory
            : public RHI::ObjectFactoryBase<Memory>
        {
        public:
            using GetHeapMemoryUsageFunction = AZStd::function<RHI::HeapMemoryUsage*()>;

            struct Descriptor
            {
                Device* m_device = nullptr;
                uint32_t m_pageSizeInBytes = 0;
                RHI::BufferBindFlags m_bindFlags = RHI::BufferBindFlags::None;
                RHI::HeapMemoryLevel m_heapMemoryLevel = RHI::HeapMemoryLevel::Host;
                RHI::HostMemoryAccess m_hostMemoryAccess = RHI::HostMemoryAccess::Write;
                GetHeapMemoryUsageFunction m_getHeapMemoryUsageFunction;
                bool m_recycleOnCollect = true;
            };

            void Init(const Descriptor& descriptor);

            RHI::Ptr<Memory> CreateObject();

            void ShutdownObject(Memory& object, bool isPoolShutdown);

            bool CollectObject(Memory& object);

            const Descriptor& GetDescriptor() const;

        private:
            Descriptor m_descriptor;
            D3D12_HEAP_TYPE m_heapType = D3D12_HEAP_TYPE_DEFAULT;
        };

        class MemoryPageAllocatorTraits
            : public RHI::ObjectPoolTraits
        {
        public:
            using ObjectType = Memory;
            using ObjectFactoryType = MemoryPageFactory;
            using MutexType = AZStd::mutex;
        };

        class MemoryPageAllocator
            : public RHI::ObjectPool<MemoryPageAllocatorTraits>
        {
        public:
            size_t GetPageCount() const
            {
                return GetObjectCount();
            }

            size_t GetPageSize() const
            {
                return GetFactory().GetDescriptor().m_pageSizeInBytes;
            }
        };
    }
}
