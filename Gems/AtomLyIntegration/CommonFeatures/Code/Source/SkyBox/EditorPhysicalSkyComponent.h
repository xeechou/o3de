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

#include <Atom/Feature/Utils/EditorRenderComponentAdapter.h>
#include <Atom/Feature/SkyBox/SkyboxConstants.h>
#include <SkyBox/PhysicalSkyComponent.h>

namespace AZ
{
    namespace Render
    {
        class EditorPhysicalSkyComponent final
            : public EditorRenderComponentAdapter<PhysicalSkyComponentController, PhysicalSkyComponent, PhysicalSkyComponentConfig>
        {
        public:

            using BaseClass = EditorRenderComponentAdapter<PhysicalSkyComponentController, PhysicalSkyComponent, PhysicalSkyComponentConfig>;
            AZ_EDITOR_COMPONENT(AZ::Render::EditorPhysicalSkyComponent, EditorPhysicalSkyComponentTypeId, BaseClass);

            static void Reflect(AZ::ReflectContext* context);

            EditorPhysicalSkyComponent() = default;
            EditorPhysicalSkyComponent(const PhysicalSkyComponentConfig& config);

            //! EditorRenderComponentAdapter overrides...
            AZ::u32 OnConfigurationChanged() override;
        };

    } // namespace Render
} // namespace AZ
