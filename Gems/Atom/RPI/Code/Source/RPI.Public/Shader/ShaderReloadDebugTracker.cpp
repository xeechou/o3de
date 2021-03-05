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

#include <Atom/RPI.Public/Shader/ShaderReloadDebugTracker.h>

namespace AZ
{
    namespace RPI
    {
        bool ShaderReloadDebugTracker::s_enabled = false;
        int ShaderReloadDebugTracker::s_indent = 0;

        bool ShaderReloadDebugTracker::IsEnabled()
        {
#ifdef AZ_ENABLE_SHADER_RELOAD_DEBUG_TRACKER
            // Set this to true in the debugger to turn on hot reload tracing.
            // If needed, we could hook this up to a CVar.
            return s_enabled;
#else
            return false;
#endif
        }

        ShaderReloadDebugTracker::ScopedSection::~ScopedSection()
        {
            ShaderReloadDebugTracker::EndSection("%s", m_sectionName.c_str());
        }

    } // namespace RPI
} // namespace AZ
