#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#
# For complete copyright and license terms please see the LICENSE at the root of this
# distribution (the "License"). All use of this software is governed by the License,
# or, if provided, by the license below or the license accompanying this file. Do not
# remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#

if (LY_ENABLE_OPENGL_WINDOWS)
    set(DXGL_TRAIT_BUILD_OPENGL_SUPPORTED TRUE)
else()
    set(DXGL_TRAIT_BUILD_OPENGL_SUPPORTED FALSE)
endif()

set(LY_TRAIT_USE_LEGACY_IN_MONOLITHIC FALSE)

