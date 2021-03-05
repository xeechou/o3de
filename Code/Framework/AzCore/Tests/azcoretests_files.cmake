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
    Main.cpp
    Asset/AssetCommon.cpp
    Asset/AssetDataStreamTests.cpp
    Asset/AssetManagerLoadingTests.cpp
    Asset/AssetManagerStreamingTests.cpp
    Asset/BaseAssetManagerTest.cpp
    Asset/BaseAssetManagerTest.h
    Asset/MockLoadAssetCatalogAndHandler.h
    Asset/TestAssetTypes.h
    AssetJsonSerializerTests.cpp
    EBus/ScheduledEventTests.cpp
    AssetManager.cpp
    TestCatalog.h
    TestCatalog.cpp
    AssetSerializerTests.cpp
    BehaviorContext.cpp
    BehaviorContextFixture.h
    Components.cpp
    Console/LoggerSystemComponentTests.cpp
    Console/ConsoleTests.cpp
    Debug.cpp
    DLL.cpp
    Driller.cpp
    EBus.cpp
    EntityIdTests.cpp
    EntityTests.cpp
    EnumTests.cpp
    EventTests.cpp
    FileIOBaseTestTypes.h
    Geometry2DUtils.cpp
    Interface.cpp
    IO/Path/PathTests.cpp
    IPC.cpp
    Jobs.cpp
    JSON.cpp
    FixedWidthIntegers.cpp
    GenericStreamMock.h
    GenericStreamTests.cpp
    Memory.cpp
    Module.cpp
    ModuleTestBus.h
    OrderedEventBenchmarks.cpp
    OrderedEventTests.cpp
    Outcome.cpp
    Patching.cpp
    RemappableId.cpp
    Rtti.cpp
    Script.cpp
    ScriptMath.cpp
    Serialization.cpp
    SerializeContextFixture.h
    Slice.cpp
    State.cpp
    StatisticalProfiler.cpp
    Statistics.cpp
    StreamerTests.cpp
    StringFunc.cpp
    SystemFile.cpp
    TickBusTest.cpp
    TimeDataStatistics.cpp
    UUIDTests.cpp
    XML.cpp
    Debug/AssetTracking.cpp
    Debug/Trace.cpp
    Name/NameJsonSerializerTests.cpp
    Name/NameTests.cpp
    RTTI/TypeSafeIntegralTests.cpp
    SettingsRegistryTests.cpp
    SettingsRegistryMergeUtilsTests.cpp
    Settings/CommandLineTests.cpp
    Settings/SettingsRegistryConsoleUtilsTests.cpp
    Settings/SettingsRegistryScriptUtilsTests.cpp
    Streamer/BlockCacheTests.cpp
    Streamer/DedicatedCacheTests.cpp
    Streamer/FullDecompressorTests.cpp
    Streamer/IStreamerMock.h
    Streamer/IStreamerTypesMock.h
    Streamer/ReadSplitterTests.cpp
    Streamer/SchedulerTests.cpp
    Streamer/StreamStackEntryConformityTests.h
    Streamer/StreamStackEntryMock.h
    Streamer/StreamStackEntryTests.cpp
    Serialization/Json/ArraySerializerTests.cpp
    Serialization/Json/BaseJsonSerializerFixture.h
    Serialization/Json/BaseJsonSerializerTests.cpp
    Serialization/Json/BasicContainerSerializerTests.cpp
    Serialization/Json/BoolSerializerTests.cpp
    Serialization/Json/ColorSerializerTests.cpp
    Serialization/Json/DoubleSerializerTests.cpp
    Serialization/Json/IntSerializerTests.cpp
    Serialization/Json/JsonRegistrationContextTests.cpp
    Serialization/Json/JsonSerializationMetadataTests.cpp
    Serialization/Json/JsonSerializationResultTests.cpp
    Serialization/Json/JsonSerializationTests.h
    Serialization/Json/JsonSerializationTests.cpp
    Serialization/Json/JsonSerializerConformityTests.h
    Serialization/Json/JsonSerializerMock.h
    Serialization/Json/MapSerializerTests.cpp
    Serialization/Json/MathVectorSerializerTests.cpp
    Serialization/Json/SmartPointerSerializerTests.cpp
    Serialization/Json/StringSerializerTests.cpp
    Serialization/Json/TestCases.h
    Serialization/Json/TestCases_Base.h
    Serialization/Json/TestCases_Base.cpp
    Serialization/Json/TestCases_Classes.h
    Serialization/Json/TestCases_Classes.cpp
    Serialization/Json/TestCases_Compare.cpp
    Serialization/Json/TestCases_Enum.cpp
    Serialization/Json/TestCases_Patching.cpp
    Serialization/Json/TestCases_Pointers.h
    Serialization/Json/TestCases_Pointers.cpp
    Serialization/Json/TestCases_TypeId.cpp
    Serialization/Json/TupleSerializerTests.cpp
    Serialization/Json/UnorderedSetSerializerTests.cpp
    Serialization/Json/UuidSerializerTests.cpp
    Math/AabbTests.cpp
    Math/ColorTests.cpp
    Math/CrcTests.cpp
    Math/CrcTestsCompileTimeLiterals.h
    Math/FrustumTests.cpp
    Math/FrustumPerformanceTests.cpp
    Math/IntersectionTests.cpp
    Math/MathIntrinsicsTests.cpp
    Math/MathUtilsTests.cpp
    Math/Matrix3x3PerformanceTests.cpp
    Math/Matrix3x3Tests.cpp
    Math/Matrix3x4PerformanceTests.cpp
    Math/Matrix3x4Tests.cpp
    Math/Matrix4x4PerformanceTests.cpp
    Math/Matrix4x4Tests.cpp
    Math/MatrixUtilsTests.cpp
    Math/MathTestData.h
    Math/ObbPerformanceTests.cpp
    Math/ObbTests.cpp
    Math/PlanePerformanceTests.cpp
    Math/PlaneTests.cpp
    Math/QuaternionPerformanceTests.cpp
    Math/QuaternionTests.cpp
    Math/ShapeIntersectionPerformanceTests.cpp
    Math/ShapeIntersectionTests.cpp
    Math/SfmtTests.cpp
    Math/SimdMathTests.cpp
    Math/SphereTests.cpp
    Math/SplineTests.cpp
    Math/TransformPerformanceTests.cpp
    Math/TransformTests.cpp
    Math/Vector2PerformanceTests.cpp
    Math/Vector2Tests.cpp
    Math/Vector3PerformanceTests.cpp
    Math/Vector3Tests.cpp
    Math/Vector4PerformanceTests.cpp
    Math/Vector4Tests.cpp
    Memory/AllocatorManager.cpp
    Memory/HphaSchema.cpp
    Memory/HphaSchemaErrorDetection.cpp
    Memory/LeakDetection.cpp
    Memory/MallocSchema.cpp
    AZStd/Algorithms.cpp
    AZStd/Allocators.cpp
    AZStd/Atomics.cpp
    AZStd/Any.cpp
    AZStd/Bitset.cpp
    AZStd/CreateDestroy.cpp
    AZStd/ConcurrentAllocators.cpp
    AZStd/ConcurrentContainers.cpp
    AZStd/ChronoTests.cpp
    AZStd/DequeAndSimilar.cpp
    AZStd/Examples.cpp
    AZStd/FunctionalBasic.cpp
    AZStd/FunctorsBind.cpp
    AZStd/Hashed.cpp
    AZStd/Invoke.cpp
    AZStd/Iterators.cpp
    AZStd/Lists.cpp
    AZStd/ListsFixed.cpp
    AZStd/ListsIntrusive.cpp
    AZStd/LockFreeQueues.cpp
    AZStd/LockFreeStacks.cpp
    AZStd/LockTests.cpp
    AZStd/Numeric.cpp
    AZStd/Ordered.cpp
    AZStd/Optional.cpp
    AZStd/Pair.cpp
    AZStd/Parallel.cpp
    AZStd/ScopedLockTests.cpp
    AZStd/SetsIntrusive.cpp
    AZStd/SmartPtr.cpp
    AZStd/String.cpp
    AZStd/TypeTraits.cpp
    AZStd/Tuple.cpp
    AZStd/UserTypes.h
    AZStd/Variant.cpp
    AZStd/VariantSerialization.cpp
    AZStd/VectorAndArray.cpp
)

# Prevent the following files from being grouped in UNITY builds
set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Memory/HphaSchema.cpp
    Memory/HphaSchemaErrorDetection.cpp
)
