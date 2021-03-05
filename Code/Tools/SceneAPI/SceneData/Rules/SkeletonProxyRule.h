#pragma once

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

#include <AzCore/Memory/Memory.h>
#include <AzCore/std/containers/fixed_vector.h>
#include <SceneAPI/SceneCore/DataTypes/Rules/ISkeletonProxyRule.h>

namespace AZ
{
    class ReflectContext;

    namespace SceneAPI
    {
        namespace Containers
        {
            class Scene;
        }
        namespace SceneData
        {
            struct SkeletonProxy
            {
                AZ_RTTI(SkeletonProxy, "{49E188A9-CA04-4B85-9AD8-A0262796EA27}");
                
                virtual ~SkeletonProxy() = default;
                
                AZStd::string m_jointName;
                AZStd::string m_proxyName;

                static void Reflect(ReflectContext* context);
            };

            struct SkeletonProxyGroup
            {
                AZ_RTTI(SkeletonProxyGroup, "{243B8186-EDDB-48C7-BCE7-FC2D1974B58A}");
                AZ_CLASS_ALLOCATOR_DECL

                virtual ~SkeletonProxyGroup() = default;
                
                AZStd::string m_proxyMaterialName;
                AZStd::vector<SkeletonProxy> m_skeletonProxies;

                static void Reflect(ReflectContext* context);
            };

            class SkeletonProxyRule
                : public DataTypes::ISkeletonProxyRule
            {
            public:
                AZ_RTTI(SkeletonProxyRule, "{142CF206-FC12-4138-B30C-FFA64EC3BB4E}", DataTypes::ISkeletonProxyRule);
                AZ_CLASS_ALLOCATOR_DECL

                ~SkeletonProxyRule() override = default;

                size_t GetProxyGroupCount() const override;
                const SkeletonProxyGroup& GetProxyGroup() const;

                static void Reflect(ReflectContext* context);

            protected:
                AZStd::vector<SkeletonProxyGroup> m_proxyGroups;
            };
        } // SceneData
    } // SceneAPI
} // AZ
