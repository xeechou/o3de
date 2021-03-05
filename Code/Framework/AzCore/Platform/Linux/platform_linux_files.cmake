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
    AzCore/AzCore_Traits_Platform.h
    AzCore/AzCore_Traits_Linux.h
    AzCore/base_Platform.h
    AzCore/base_Linux.h
    AzCore/PlatformId/PlatformId_Platform.h
    AzCore/PlatformId/PlatformId_Linux.h
    ../Common/UnixLike/AzCore/std/parallel/internal/condition_variable_UnixLike.h
    ../Common/UnixLike/AzCore/std/parallel/internal/mutex_UnixLike.h
    ../Common/UnixLike/AzCore/std/parallel/internal/semaphore_UnixLike.h
    ../Common/UnixLike/AzCore/std/parallel/internal/thread_UnixLike.cpp
    ../Common/UnixLike/AzCore/std/parallel/internal/thread_UnixLike.h
    ../Common/UnixLike/AzCore/std/parallel/internal/time_UnixLike.h
    AzCore/std/parallel/internal/condition_variable_Platform.h
    AzCore/std/parallel/internal/mutex_Platform.h
    AzCore/std/parallel/internal/semaphore_Platform.h
    AzCore/std/parallel/internal/thread_Linux.cpp
    AzCore/std/parallel/internal/thread_Platform.h
    AzCore/std/parallel/config_Linux.h
    AzCore/std/parallel/config_Platform.h
    AzCore/std/string/fixed_string_Platform.inl
    ../Common/Clang/AzCore/std/string/fixed_string_Clang.inl
    ../Common/UnixLike/AzCore/Debug/StackTracer_UnixLike.cpp
    ../Common/UnixLike/AzCore/Debug/Trace_UnixLike.cpp
    AzCore/Debug/Trace_Linux.cpp
    ../Common/Default/AzCore/IO/Streamer/StreamerConfiguration_Default.cpp
    ../Common/Default/AzCore/IO/Streamer/StreamerContext_Default.cpp
    ../Common/Default/AzCore/IO/Streamer/StreamerContext_Default.h
    ../Common/UnixLike/AzCore/IO/SystemFile_UnixLike.cpp
    ../Common/UnixLike/AzCore/IO/SystemFile_UnixLike.h
    ../Common/UnixLike/AzCore/IO/Internal/SystemFileUtils_UnixLike.h
    ../Common/UnixLike/AzCore/IO/Internal/SystemFileUtils_UnixLike.cpp
    ../Common/UnixLikeDefault/AzCore/IO/SystemFile_UnixLikeDefault.cpp
    AzCore/IO/SystemFile_Linux.cpp
    AzCore/IO/SystemFile_Platform.h
    AzCore/IPC/SharedMemory_Platform.h
    ../Common/Unimplemented/AzCore/Memory/OverrunDetectionAllocator_Unimplemented.h
    ../Common/UnixLike/AzCore/Memory/OSAllocator_UnixLike.h
    AzCore/Memory/HeapSchema_Linux.cpp
    AzCore/Memory/OSAllocator_Platform.h
    AzCore/Memory/OverrunDetectionAllocator_Platform.h
    AzCore/Module/Internal/ModuleManagerSearchPathTool_Linux.cpp
    AzCore/Math/Internal/MathTypes_Linux.h
    AzCore/Math/Random_Platform.h
    ../Common/UnixLike/AzCore/Math/Random_UnixLike.cpp
    ../Common/UnixLike/AzCore/Math/Random_UnixLike.h
    ../Common/UnixLike/AzCore/Module/DynamicModuleHandle_UnixLike.cpp
    AzCore/Module/DynamicModuleHandle_Linux.cpp
    ../Common/Unimplemented/AzCore/NativeUI/NativeUISystemComponent_Unimplemented.cpp
    ../Common/UnixLike/AzCore/PlatformIncl_UnixLike.h
    ../Common/UnixLike/AzCore/Platform_UnixLike.cpp
    AzCore/PlatformIncl_Platform.h
    ../Common/UnixLike/AzCore/Socket/AzSocket_fwd_UnixLike.h
    ../Common/UnixLike/AzCore/Socket/AzSocket_UnixLike.cpp
    ../Common/UnixLike/AzCore/Socket/AzSocket_UnixLike.h
    AzCore/Socket/AzSocket_fwd_Platform.h
    AzCore/Socket/AzSocket_Platform.h
    ../Common/UnixLike/AzCore/std/time_UnixLike.cpp
    AzCore/Utils/Utils_Linux.cpp
    ../Common/UnixLike/AzCore/Utils/Utils_UnixLike.cpp
)
