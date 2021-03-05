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

#include "Woodpecker_precompiled.h"
#include "VRAMEvents.h"
#include "VRAMDataAggregator.hxx"

namespace Driller
{
    namespace VRAM
    {
        //=========================================================================

        CategoryInfo* GetCategory(Aggregator* data, AZ::u32 categoryId)
        {
            VRAMDataAggregator* aggregator = static_cast<VRAMDataAggregator*>(data);
            CategoryInfo* category = aggregator->FindCategory(categoryId);

            if (category == nullptr)
            {
                AZ_Assert(false, "VRAMDriller - Invalid Category");
                return nullptr;
            }
            return category;
        }

        //=========================================================================

        void VRAMDrillerRegisterAllocationEvent::StepForward(Aggregator* data)
        {
            CategoryInfo* m_categoryInfo = GetCategory(data, m_allocationInfo.m_category);
            if (!m_categoryInfo)
            {
                return;
            }

            // Add the allocation
            m_categoryInfo->m_allocations.insert(AZStd::make_pair(m_address, &m_allocationInfo));
            m_categoryInfo->m_allocatedMemory += m_allocationInfo.m_size;
        }

        void VRAMDrillerRegisterAllocationEvent::StepBackward(Aggregator* data)
        {
            CategoryInfo* m_categoryInfo = GetCategory(data, m_allocationInfo.m_category);
            if (!m_categoryInfo)
            {
                return;
            }

            // Remove the allocation
            m_categoryInfo->m_allocations.erase(m_address);
            m_categoryInfo->m_allocatedMemory -= m_allocationInfo.m_size;
        }

        //=========================================================================

        void VRAMDrillerUnregisterAllocationEvent::StepForward(Aggregator* data)
        {
            VRAMDataAggregator* aggregator = static_cast<VRAMDataAggregator*>(data);
            m_removedAllocationInfo = aggregator->FindAndRemoveAllocation(m_address);

            if (!m_removedAllocationInfo)
            {
                AZ_Warning("System", 0, "Error: Allocation not found for VRAMDrillerUnregisterAllocationEvent");
            }
        }

        void VRAMDrillerUnregisterAllocationEvent::StepBackward(Aggregator* data)
        {
            if (m_removedAllocationInfo == nullptr)
            {
                AZ_Warning("System", 0, "Error: Allocation not found for VRAMDrillerUnregisterAllocationEvent");
                return;
            }

            CategoryInfo* m_categoryInfo = GetCategory(data, m_removedAllocationInfo->m_category);
            if (!m_categoryInfo)
            {
                return;
            }

            // "Reallocation"
            auto insertionPair = AZStd::make_pair(m_address, m_removedAllocationInfo);
            m_categoryInfo->m_allocations.insert(insertionPair);

            // The opposite of deallocation, which is allocating, so we add:
            m_categoryInfo->m_allocatedMemory += m_removedAllocationInfo->m_size;
        }

        //=========================================================================

        void VRAMDrillerRegisterCategoryEvent::StepForward(Aggregator* data)
        {
            VRAMDataAggregator* aggregator = static_cast<VRAMDataAggregator*>(data);
            aggregator->RegisterCategory(m_categoryId, &m_categoryInfo);
        }

        void VRAMDrillerRegisterCategoryEvent::StepBackward(Aggregator* data)
        {
            VRAMDataAggregator* aggregator = static_cast<VRAMDataAggregator*>(data);
            aggregator->UnregisterCategory(m_categoryId);
        }

        //=========================================================================

        void VRAMDrillerUnregisterCategoryEvent::StepForward(Aggregator* data)
        {
            // TODO: Need to get m_unregisteredCategoryInfo from the category we are unregistering so we can re-register the category on StepBackward
            VRAMDataAggregator* aggregator = static_cast<VRAMDataAggregator*>(data);
            aggregator->UnregisterCategory(m_categoryId);
        }

        void VRAMDrillerUnregisterCategoryEvent::StepBackward(Aggregator* data)
        {
            VRAMDataAggregator* aggregator = static_cast<VRAMDataAggregator*>(data);
            aggregator->RegisterCategory(m_categoryId, &m_unregisteredCategoryInfo);
        }

        //=========================================================================
    } // namespace VRAM
} // namespace Driller
