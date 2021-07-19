/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Atom/Document/ShaderManagementConsoleDocumentModule.h>
#include <Document/ShaderManagementConsoleDocumentSystemComponent.h>

#include <AzToolsFramework/UI/PropertyEditor/PropertyManagerComponent.h>
#include <AzToolsFramework/Asset/AssetSystemComponent.h>

namespace ShaderManagementConsole
{
    ShaderManagementConsoleDocumentModule::ShaderManagementConsoleDocumentModule()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        m_descriptors.insert(m_descriptors.end(), {
            ShaderManagementConsoleDocumentSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList ShaderManagementConsoleDocumentModule::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<AzToolsFramework::AssetSystem::AssetSystemComponent>(),
            azrtti_typeid<ShaderManagementConsoleDocumentSystemComponent>(),
        };
    }
}
