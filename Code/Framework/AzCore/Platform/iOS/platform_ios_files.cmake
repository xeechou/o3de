#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    AzCore/AzCore_Traits_Platform.h
    AzCore/AzCore_Traits_iOS.h
    AzCore/base_Platform.h
    AzCore/base_iOS.h
    AzCore/PlatformId/PlatformId_Platform.h
    AzCore/PlatformId/PlatformId_iOS.h
    ../Common/Apple/AzCore/std/parallel/internal/semaphore_Apple.h
    ../Common/Apple/AzCore/std/parallel/internal/thread_Apple.cpp
    ../Common/Apple/AzCore/std/parallel/internal/time_Apple.h
    ../Common/UnixLike/AzCore/std/parallel/internal/condition_variable_UnixLike.h
    ../Common/UnixLike/AzCore/std/parallel/internal/mutex_UnixLike.h
    ../Common/UnixLike/AzCore/std/parallel/internal/thread_UnixLike.cpp
    ../Common/UnixLike/AzCore/std/parallel/internal/thread_UnixLike.h
    AzCore/std/parallel/internal/condition_variable_Platform.h
    AzCore/std/parallel/internal/mutex_Platform.h
    AzCore/std/parallel/internal/semaphore_Platform.h
    AzCore/std/parallel/internal/thread_Platform.h
    ../Common/Apple/AzCore/std/parallel/config_Apple.h
    AzCore/std/parallel/config_Platform.h
    AzCore/std/string/fixed_string_Platform.inl
    ../Common/Clang/AzCore/std/string/fixed_string_Clang.inl
    ../Common/Apple/AzCore/Process/ProcessInfo_Apple.cpp
    ../Common/UnixLike/AzCore/Debug/StackTracer_UnixLike.cpp
    ../Common/Apple/AzCore/Debug/Trace_Apple.cpp
    ../Common/Default/AzCore/IO/Streamer/StreamerConfiguration_Default.cpp
    ../Common/Default/AzCore/IO/Streamer/StreamerContext_Default.cpp
    ../Common/Default/AzCore/IO/Streamer/StreamerContext_Default.h
    ../Common/Apple/AzCore/IO/SystemFile_Apple.cpp
    ../Common/Apple/AzCore/IO/SystemFile_Apple.h
    ../Common/UnixLike/AzCore/IO/AnsiTerminalUtils_UnixLike.cpp
    ../Common/UnixLike/AzCore/IO/SystemFile_UnixLike.cpp
    ../Common/UnixLike/AzCore/IO/Internal/SystemFileUtils_UnixLike.h
    ../Common/UnixLike/AzCore/IO/Internal/SystemFileUtils_UnixLike.cpp
    ../Common/UnixLikeDefault/AzCore/IO/SystemFile_UnixLikeDefault.cpp
    AzCore/IO/Streamer/StreamerContext_Platform.h
    AzCore/IO/SystemFile_Platform.h
    AzCore/IPC/SharedMemory_Platform.h
    ../Common/Apple/AzCore/Memory/OSAllocator_Apple.h
    AzCore/Memory/OSAllocator_Platform.h
    AzCore/Math/Internal/MathTypes_iOS.h
    ../Common/Default/AzCore/Module/Internal/ModuleManagerSearchPathTool_Default.cpp
    AzCore/Math/Random_Platform.h
    ../Common/UnixLike/AzCore/Math/Random_UnixLike.cpp
    ../Common/UnixLike/AzCore/Math/Random_UnixLike.h
    AzCore/Module/DynamicModuleHandle_iOS.cpp
    ../Common/UnixLike/AzCore/Module/DynamicModuleHandle_UnixLike.cpp
    AzCore/NativeUI/NativeUISystemComponent_iOS.mm
    ../Common/UnixLike/AzCore/PlatformIncl_UnixLike.h
    ../Common/UnixLike/AzCore/Platform_UnixLike.cpp
    AzCore/PlatformIncl_Platform.h
    ../Common/UnixLike/AzCore/Settings/CommandLineParser_UnixLike.cpp
    ../Common/UnixLike/AzCore/Socket/AzSocket_fwd_UnixLike.h
    ../Common/UnixLike/AzCore/Socket/AzSocket_UnixLike.cpp
    ../Common/UnixLike/AzCore/Socket/AzSocket_UnixLike.h
    AzCore/Socket/AzSocket_fwd_Platform.h
    AzCore/Socket/AzSocket_Platform.h
    ../Common/Apple/AzCore/std/time_Apple.cpp
    AzCore/Utils/SystemUtilsApple_Platform.h
    ../Common/Apple/AzCore/Utils/SystemUtilsApple.h
    ../Common/Apple/AzCore/Utils/SystemUtilsApple.mm
    AzCore/Utils/Utils_iOS.mm
    ../Common/Apple/AzCore/Utils/Utils_Apple.cpp
    ../Common/UnixLike/AzCore/Utils/Utils_UnixLike.cpp
    AzCore/Debug/Profiler_Platform.inl
    ../Common/Unimplemented/AzCore/Debug/Profiler_Unimplemented.inl
)
