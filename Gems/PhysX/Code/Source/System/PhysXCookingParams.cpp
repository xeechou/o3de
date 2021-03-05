/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates, or
* a third party where indicated.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#include <System/PhysXCookingParams.h>

namespace PhysX
{
    namespace PxCooking
    {
        //! Return PxCookingParams better suited for use at run-time, these parameters will improve cooking time.
        //! Reference: https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html#triangle-meshes
        physx::PxCookingParams GetRealTimeCookingParams()
        {
            physx::PxCookingParams params{ physx::PxTolerancesScale() };
            // disable mesh cleaning - perform mesh validation on development configurations
            params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
            // disable edge pre-compute, edges are set for each triangle, slows contact generation
            params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

            return params;
        }

        //! Return PxCookingParams better suited for use at edit-time, these parameters will
        //! increase cooking time but improve accuracy/precision.
        physx::PxCookingParams GetEditTimeCookingParams()
        {
            physx::PxCookingParams params{ physx::PxTolerancesScale() };
            // when set, mesh welding is performed - clean mesh must be enabled
            params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eWELD_VERTICES;
            // note: default value in PxCookingParams is 0.0f;
            const float physx_CookWeldTolerance = 0.0001f;
            params.meshWeldTolerance = physx_CookWeldTolerance;

            return params;
        }
    }
}
