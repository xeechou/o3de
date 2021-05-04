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

#include <Target/TestImpactBuildTarget.h>

#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/containers/unordered_set.h>
#include <AzCore/std/containers/vector.h>

namespace TestImpact
{
    class ProductionTarget;
    class TestTarget;

    //! Representation of a source dependency's parent target.
    class ParentTarget
    {
    public:
        //! Constructor overload for test target types.
        ParentTarget(const TestTarget* target);

        //! Constructor overload for production target types.
        ParentTarget(const ProductionTarget* target);

        //! Returns the base build target pointer for this parent.
        const BuildTarget* GetBuildTarget() const;

        //! Returns the specialized target pointer for this parent.
        const Target& GetTarget() const;

        bool operator==(const ParentTarget& other) const;
    private:
        Target m_target; //! The specialized target pointer for this parent.
    };
}

namespace AZStd
{
    //! Hash function for ParentTarget types for use in maps and sets
    template<> struct hash<TestImpact::ParentTarget>
    {
        size_t operator()(const TestImpact::ParentTarget& parentTarget) const noexcept
        {
            return reinterpret_cast<size_t>(parentTarget.GetBuildTarget());
        }
    };
}

namespace TestImpact
{
    struct DependencyData
    {
        AZStd::unordered_set<ParentTarget> m_parentTargets;
        AZStd::unordered_set<const TestTarget*> m_coveringTestTargets;
    };

    //! Test target coverage and build target dependency information for a given source file in the dynamic dependency map.
    class SourceDependency
    {
    public:
        SourceDependency(
            const AZStd::string& path,
            DependencyData&& dependencyData);

        //! Returns the path of this source file.
        const AZStd::string& GetPath() const;

        //! Returns the number of parent build targets this source belongs to.
        size_t GetNumParentTargets() const;

        //! Returns the number of test targets covering this source file.
        size_t GetNumCoveringTestTargets() const;

        //! Returns the parent targets that this source file belongs to.
        const AZStd::unordered_set<ParentTarget>& GetParentTargets() const;

        //! Returns the test targets covering this source file.
        const AZStd::unordered_set<const TestTarget*>& GetCoveringTestTargets() const;
    private:
        AZStd::string m_path; //!< The path of this source file.
        DependencyData m_dependencyData; //!< The dependency data for this source file.
    };
} // namespace TestImpact
