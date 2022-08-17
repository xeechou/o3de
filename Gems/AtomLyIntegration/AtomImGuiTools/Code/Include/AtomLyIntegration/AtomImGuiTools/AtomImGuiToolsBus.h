/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Component/EntityId.h>
#include <Atom/RPI.Public/MeshDrawPacket.h>
#include <Atom/Feature/Material/MaterialAssignmentId.h>

namespace AZ
{
    namespace RPI
    {
        class Material;
    }
}

namespace AtomImGuiTools
{
    //! AtomImGuiToolsRequestBus provides an interface to interact with Atom ImGui debug tools
    class AtomImGuiToolsRequests : public AZ::EBusTraits
    {
    public:
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

        virtual void ShowMaterialShaderDetailsForEntity(AZ::EntityId entity, bool autoOpenDialog) = 0;
        virtual void ShowMaterialShaderDetailsForEntityMaterialAssignment(
            AZ::EntityId entity, const AZ::Render::MaterialAssignmentId& materialAssignmentId, AZ::Data::Instance<AZ::RPI::Material> material, bool autoOpenDialog) = 0;
    };
    using AtomImGuiToolsRequestBus = AZ::EBus<AtomImGuiToolsRequests>;
    
    class AtomImGuiMeshCallbacks : public AZ::ComponentBus
    {
    public:
        virtual const AZ::RPI::MeshDrawPacketLods* GetDrawPackets() const = 0;
    };
    using AtomImGuiMeshCallbackBus = AZ::EBus<AtomImGuiMeshCallbacks>;

} 
