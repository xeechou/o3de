/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzToolsFramework/UI/DocumentPropertyEditor/PropertyEditorToolsSystemInterface.h>

namespace AzToolsFramework
{
    class PropertyEditorToolsSystem : public PropertyEditorToolsSystemInterface
    {
    public:
        AZ_RTTI(PropertyEditorToolsSystem, "{78954B5C-D147-411F-BBDA-A28D2CA50A3A}", PropertyEditorToolsSystemInterface);
        AZ_CLASS_ALLOCATOR(PropertyEditorToolsSystem, AZ::OSAllocator, 0);

        PropertyEditorToolsSystem();
        ~PropertyEditorToolsSystem() override;

        PropertyHandlerId GetPropertyHandlerForNode(const AZ::Dom::Value node) override;
        PropertyHandlerInstance CreateHandlerInstance(PropertyHandlerId handlerId) override;
        PropertyHandlerId RegisterHandler(HandlerData handlerData) override;
        void UnregisterHandler(PropertyHandlerId handlerId) override;

    private:
        AZStd::unordered_map<AZ::Name, AZStd::list<HandlerData>> m_registeredHandlers;
    };
}
