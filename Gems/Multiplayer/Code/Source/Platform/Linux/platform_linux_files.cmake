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

set(FILES
    ../Common/Multiplayer/BehaviorContext/GridSystemContext_Unimplemented.cpp
    ../Common/Multiplayer/MultiplayerLobbyComponent_Unimplemented.cpp
    Multiplayer_GridMateServiceWrapper_Platform.h
    Multiplayer_GridMateServiceWrapper_Linux.h
    Multiplayer_Traits_Platform.h
    Multiplayer_Traits_Linux.h
)

set(LY_COMPILE_DEFINITIONS
    PRIVATE
        NET_SUPPORT_SECURE_SOCKET_DRIVER=1
)
