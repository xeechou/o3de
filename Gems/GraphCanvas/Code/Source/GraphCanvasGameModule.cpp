/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GraphCanvasModule.h>

namespace GraphCanvas
{
    GraphCanvasModule::GraphCanvasModule()
    {
    }

    AZ::ComponentTypeList GraphCanvasModule::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{};
    }
}

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GraphCanvas::GraphCanvasModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GraphCanvas, GraphCanvas::GraphCanvasModule)
#endif
