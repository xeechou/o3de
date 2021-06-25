/*
 * Copyright (c) Contributors to the Open 3D Engine Project
 * 
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AzCore/RTTI/TypeInfo.h>
#include <Atom/RHI.Reflect/Handle.h>

namespace AZ
{
    namespace RPI
    {
        // Common bit positions for ShaderAsset and ShaderVariantAsset product SubIds.
        static constexpr uint32_t RhiIndexBitPosition = 30;
        static constexpr uint32_t RhiIndexNumBits = 32 - RhiIndexBitPosition;
        static constexpr uint32_t RhiIndexMaxValue = (1 << RhiIndexNumBits) - 1;

        static constexpr uint32_t SupervariantIndexBitPosition = 22;
        static constexpr uint32_t SupervariantIndexNumBits = RhiIndexBitPosition - SupervariantIndexBitPosition;
        static constexpr uint32_t SupervariantIndexMaxValue = (1 << SupervariantIndexNumBits) - 1;

        //! A wrapper around a supervariant index for type conformity.
        //! A supervariant index is required to find shader data from
        //! Shader and ShaderAsset related APIs.
        using SupervariantIndex = RHI::Handle<uint32_t, class ShaderAsset>;
        static const SupervariantIndex DefaultSupervariantIndex(0);
        static const SupervariantIndex InvalidSupervariantIndex;

        enum class ShaderStageType : uint32_t
        {
            Vertex,
            Geometry,
            TessellationControl,
            TessellationEvaluation,
            Fragment,
            Compute,
            RayTracing
        };

        const char* ToString(ShaderStageType shaderStageType);

        void ReflectShaderStageType(ReflectContext* context);

    } // namespace RPI

    AZ_TYPE_INFO_SPECIALIZE(RPI::ShaderStageType, "{A6408508-748B-4963-B618-E1E6ECA3629A}");
    
} // namespace AZ
