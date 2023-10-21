#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

if(WWISE_VERSION VERSION_LESS_EQUAL "2021.1.10.0")
    set(WWISE_WINDOWS_LIB_NAMES
        AkAutobahn
        SFLib
    )
else()
    # SFLib is no longer used
    set(WWISE_WINDOWS_LIB_NAMES
        AkAutobahn
    )
endif()

# Current mapping of toolset to Wwise SDK folder name 
if(MSVC_TOOLSET_VERSION VERSION_EQUAL 142)
    set(WWISE_VS_VER "vc160")
elseif(MSVC_TOOLSET_VERSION VERSION_EQUAL 143)
    if(WWISE_VERSION VERSION_LESS "2021.1.10.0")
        message(FATAL_ERROR "Visual Studio 2022 is not supported in the current installed Wwise SDK: " ${WWISE_VERSION} " Please install Wwise version 2021.1.10 or higher.")
    endif()
    set(WWISE_VS_VER "vc170")
else()
    message(FATAL_ERROR "Unable to determine Wwise SDK library path for MSVC toolset version: " ${MSVC_TOOLSET_VERSION})
endif()

set(WWISE_LIB_PATH ${BASE_PATH}/SDK/x64_${WWISE_VS_VER}/$<IF:$<CONFIG:Debug>,Debug,$<IF:$<CONFIG:Profile>,Profile,Release>>/lib)

set(WWISE_LIBS)

foreach(libName IN LISTS WWISE_COMMON_LIB_NAMES WWISE_CODEC_LIB_NAMES WWISE_WINDOWS_LIB_NAMES WWISE_ADDITIONAL_LIB_NAMES)
    list(APPEND WWISE_LIBS ${WWISE_LIB_PATH}/${libName}.lib)
endforeach()

foreach(nonReleaseLibName IN LISTS WWISE_NON_RELEASE_LIB_NAMES)
    list(APPEND WWISE_LIBS $<$<NOT:$<CONFIG:Release>>:${WWISE_LIB_PATH}/${nonReleaseLibName}.lib>)
endforeach()

# windows OS libs
list(APPEND WWISE_LIBS
    dxguid.lib
    DInput8.lib
    $<$<NOT:$<CONFIG:Release>>:ws2_32.lib>
)
