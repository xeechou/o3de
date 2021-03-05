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

#include <AzCore/Math/Vector3.h>
#include <IRenderAuxGeom.h>
#include <Azcore/Math/Color.h>

namespace Blast
{
    struct DebugLine
    {
        DebugLine(const AZ::Vector3& p0, const AZ::Vector3& p1, const AZ::Color& color)
            : m_p0(p0)
            , m_p1(p1)
            , m_color(color)
        {
        }

        AZ::Vector3 m_p0;
        AZ::Vector3 m_p1;
        AZ::Color m_color;
    };

    struct DebugRenderBuffer
    {
        AZStd::vector<DebugLine> m_lines;
    };

    enum DebugRenderMode
    {
        DebugRenderDisabled = 0,
        DebugRenderHealthGraph = 1,
        DebugRenderCentroids = 2,
        DebugRenderHealthGraphCentroids = 3,
        DebugRenderJoints = 4, // Not actually active since we don't support joints at the moment
        DebugRenderAabbTreeCentroids = 5,
        DebugRenderAabbTreeSegments = 6,
        DebugRenderStressGraph = 7,
        DebugRenderStressGraphNodesImpulses = 8,
        DebugRenderStressGraphBondsImpulses = 9,

        // count
        DebugRenderModesCount
    };

} // namespace Blast
