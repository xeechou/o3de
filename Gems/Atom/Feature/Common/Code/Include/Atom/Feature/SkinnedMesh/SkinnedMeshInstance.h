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

#include <Atom/Feature/SkinnedMesh/SkinnedMeshOutputStreamManagerInterface.h>
#include <Atom/RPI.Reflect/Buffer/BufferAssetView.h>
#include <Atom/RPI.Public/Model/Model.h>
#include <AtomCore/Instance/InstanceData.h>

namespace AZ
{
    namespace Render
    {
        //! SkinnedMeshInstance contains the data that is needed to represent the output from skinning a single instance of a skinned mesh.
        //! It does not contain the actual skinned vertex data, but rather views into the buffers that do contain the data, which are owned by the SkinnedMeshOutputStreamManager
        class SkinnedMeshInstance
            : public AZStd::intrusive_base
        {
        public:
            AZ_CLASS_ALLOCATOR(SkinnedMeshInstance, AZ::SystemAllocator, 0);

            //! The target model, which is used by the MeshFeatureProcessor to render the mesh
            Data::Instance<RPI::Model> m_model;

            //! Offsets into the skinned vertex data which are used by SkinnedMeshDispatchItem to target the correct location to store the skinning results
            AZStd::vector<AZStd::vector<uint32_t>> m_outputStreamOffsetsInBytes;

            //! Virtual addresses that represent the location of the data within the skinned mesh output stream.
            //! When they are released, they automatically mark the memory as freed so the SkinnedMeshOutputStreamManager can re-purpose the memory
            AZStd::vector<AZStd::vector<AZStd::intrusive_ptr<SkinnedMeshOutputStreamAllocation>>> m_allocations;

            //! Offsets into the output stream buffer to a location that contains accumulated morph target deltas from the morph pass. One offset per-lod.
            //! Set to MorphTargetConstants::s_invalidDeltaOffset if there are no morph targets for the lod
            AZStd::vector<uint32_t> m_accumulatedMorphTargetDeltaOffsetsInBytes;

            //! Typically, when a SkinnedMeshInstance goes out of scope and the memory is freed, the SkinnedMeshOutputStreamManager will signal an event indicating more memory is available
            //! If the creation of a SkinnedMeshInstance fails part way through after some memory has already been allocated,
            //! calling SupressSignalOnDeallocate before releasing the SkinnedMeshInstance will prevent this event since there is not really any new memory available that wasn't available before
            void SuppressSignalOnDeallocate();
        };
    }// Render
}// AZ
