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

#include <Target/TestImpactTargetException.h>

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/optional.h>
#include <AzCore/std/sort.h>
#include <AzCore/std/string/string.h>

#include <algorithm>

namespace TestImpact
{
    //! Container for unique set of sorted build target types.
    //! @tparam Target The specialized build target type.
    template<typename Target>
    class BuildTargetList
    {
    public:
        using TargetType = Target;

        BuildTargetList(AZStd::vector<typename Target::Descriptor>&& descriptors);

        //! Returns the targets in the collection.
        const AZStd::vector<Target>& GetTargets() const;

        //! Returns the target with the specified name.
        const Target* GetTarget(const AZStd::string& name) const;

        //! Returns the target with the specified name or throws if target not found.
        const Target* GetTargetOrThrow(const AZStd::string& name) const;

        // Returns the number of targets in the list.
        size_t GetNumTargets() const;

    private:
        AZStd::vector<Target> m_targets;
    };

    template<typename Target>
    BuildTargetList<Target>::BuildTargetList(AZStd::vector<typename Target::Descriptor>&& descriptors)
    {
        AZ_TestImpact_Eval(!descriptors.empty(), TargetException, "Target list is empty");

        AZStd::sort(
            descriptors.begin(), descriptors.end(), [](const typename Target::Descriptor& lhs, const typename Target::Descriptor& rhs)
        {
                return lhs.m_buildMetaData.m_name < rhs.m_buildMetaData.m_name;
        });

        const auto duplicateElement = AZStd::adjacent_find(
            descriptors.begin(), descriptors.end(), [](const typename Target::Descriptor& lhs, const typename Target::Descriptor& rhs)
        {
                return lhs.m_buildMetaData.m_name == rhs.m_buildMetaData.m_name;
        });

        AZ_TestImpact_Eval(duplicateElement == descriptors.end(), TargetException, "Target list contains duplicate targets");

        m_targets.reserve(descriptors.size());
        for (auto&& descriptor : descriptors)
        {
            m_targets.emplace_back(Target(AZStd::move(descriptor)));
        }
    }

    template<typename Target>
    const AZStd::vector<Target>& BuildTargetList<Target>::GetTargets() const
    {
        return m_targets;
    }

    template<typename Target>
    size_t BuildTargetList<Target>::GetNumTargets() const
    {
        return m_targets.size();
    }

    template<typename Target>
    const Target* BuildTargetList<Target>::GetTarget(const AZStd::string& name) const
    {
        struct TargetComparator
        {
            bool operator()(const Target& target, const AZStd::string& name) const
            {
                return target.GetName() < name;
            }

            bool operator()(const AZStd::string& name, const Target& target) const
            {
                return name < target.GetName();
            }
        };

        const auto targetRange = std::equal_range(m_targets.begin(), m_targets.end(), name, TargetComparator{});

        if (targetRange.first != targetRange.second)
        {
            return targetRange.first;
        }
        else
        {
            return nullptr;
        }
    }

    template<typename Target>
    const Target* BuildTargetList<Target>::GetTargetOrThrow(const AZStd::string& name) const
    {
        const Target* target = GetTarget(name);
        AZ_TestImpact_Eval(target, TargetException, AZStd::string::format("Couldn't find target %s", name.c_str()).c_str());
        return target;
    }
} // namespace TestImpact
