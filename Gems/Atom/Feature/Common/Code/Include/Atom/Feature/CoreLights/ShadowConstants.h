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

#include <AzCore/base.h>
#include <AzCore/Preprocessor/Enum.h>

namespace AZ
{
    namespace Render
    {
        AZ_ENUM_CLASS_WITH_UNDERLYING_TYPE(ShadowmapSize, uint32_t,
            // For None, shadowmap image has width=1 and height=1.
            // Several passes require the image resource so, we minimize its memory footprint.
            (None, 1), // disabling shadowmap
            (Size256, 256),
            (Size512, 512),
            (Size1024, 1024),
            (Size2048, 2048));

        static constexpr ShadowmapSize MinShadowmapImageSize = ShadowmapSize::Size256;
        static constexpr ShadowmapSize MaxShadowmapImageSize = ShadowmapSize::Size2048;

        // This matchs ShadowFilterMethod in CoreLights/ViewSrg.azsli
        enum class ShadowFilterMethod : uint32_t
        {
            None = 0,
            Pcf, // Percentage Closer Filtering
            Esm, // Exponential Shadow Maps
            EsmPcf, // ESM with PCF fallback

            Count
        };

        namespace Shadow
        {
            // [GFX TODO][ATOM-2408] Make the max number of cascade modifiable at runtime.
            static constexpr uint16_t MaxNumberOfCascades = 4;
            static constexpr uint16_t MaxPcfSamplingCount = 64;
            static constexpr float MaxSofteningBoundaryWidth = 0.1f;
        } // namespace Shadow

    } // namespace Render

    AZ_TYPE_INFO_SPECIALIZE(Render::ShadowmapSize, "{3EC1CE83-483D-41FD-9909-D22B03E56F4E}");
    
} // namespace AZ
