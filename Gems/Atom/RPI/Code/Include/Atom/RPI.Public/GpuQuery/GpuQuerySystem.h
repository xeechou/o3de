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

#include <Atom/RPI.Public/GpuQuery/GpuQuerySystemInterface.h>
#include <Atom/RPI.Public/GpuQuery/QueryPool.h>

#include <AzCore/Memory/SystemAllocator.h>

namespace AZ
{
    namespace RPI
    {
        class CommandList;
        struct GpuQuerySystemDescriptor;

        class  GpuQuerySystem
            : public GpuQuerySystemInterface
        {
        public:
            AZ_TYPE_INFO(GpuQuerySystem, "{7A314998-AB21-464B-8AFC-525018D2150A}");
            AZ_CLASS_ALLOCATOR(GpuQuerySystem, AZ::SystemAllocator, 0);

            GpuQuerySystem() = default;

            void Init(const GpuQuerySystemDescriptor& desc);
            void Shutdown();
            void Update();

        private:
            // GpuQuerySystemInterface Overrides...
            RHI::Ptr<Query> CreateQuery(RHI::QueryType queryType,
                RHI::QueryPoolScopeAttachmentType attachmentType,
                RHI::ScopeAttachmentAccess attachmentAccess) final;

            // Caches the QueryTypes that are supported for the device that is owned by the RHISystem.
            void CacheFeatureSupport();
            // Checks whether the QueryType is valid.
            bool IsQueryTypeValid(RHI::QueryType queryType);
            // Returns whether the QueryType is supported for the device that is owned by the RHISystem.
            bool IsQueryTypeSupported(RHI::QueryType queryType);

            // Gets the RPI QueryPool depending on the QueryType.
            RPI::QueryPool* GetQueryPoolByType(RHI::QueryType queryType);

            // Cache QueryTypeFlags support for all command queues associated with the device that is owned by the RHISystem.
            RHI::QueryTypeFlags m_queryTypeSupport = RHI::QueryTypeFlags::None;

            // Array of RPI QueryPools. One for each QueryType.
            AZStd::array<AZStd::unique_ptr<RPI::QueryPool>, static_cast<uint32_t>(RHI::QueryType::Count)> m_queryPoolArray = {};
        };

    }; // namespace RPI
}; // namespace AZ
