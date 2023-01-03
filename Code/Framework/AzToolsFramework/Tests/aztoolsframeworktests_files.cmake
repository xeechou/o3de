#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Main.cpp
    ActionManager/ActionManagerFixture.cpp
    ActionManager/ActionManagerFixture.h
    ActionManager/ActionManagerTests.cpp
    ActionManager/HotKeyManagerTests.cpp
    ActionManager/MenuManagerTests.cpp
    ActionManager/ToolBarManagerTests.cpp
    ArchiveTests.cpp
    AssetFileInfoListComparison.cpp
    AssetSeedManager.cpp
    AssetSystemMocks.h
    AzToolsFrameworkTestHelpersTest.cpp
    BoundsTestComponent.cpp
    BoundsTestComponent.h
    ComponentAdapterTests.cpp
    ComponentAddRemove.cpp
    ComponentModeTestDoubles.cpp
    ComponentModeTestDoubles.h
    ComponentModeTestFixture.cpp
    ComponentModeTestFixture.h
    ComponentModeTests.cpp
    ComponentModeSwitcherTests.cpp
    EditorTransformComponentSelectionTests.cpp
    EditorVertexSelectionTests.cpp
    EditorViewportIconTests.cpp
    EditorViewportHelperTests.cpp
    Entity/EditorEntityContextComponentTests.cpp
    Entity/EditorEntityHelpersTests.cpp
    Entity/EditorEntitySearchComponentTests.cpp
    Entity/EditorEntitySelectionTests.cpp
    Entity/EntityUtilityComponentTests.cpp
    Entity/ReadOnly/ReadOnlyEntityFixture.cpp
    Entity/ReadOnly/ReadOnlyEntityFixture.h
    Entity/ReadOnly/ReadOnlyEntityTests.cpp
    EntityIdQLabelTests.cpp
    EntityInspectorTests.cpp
    EntityOwnershipService/EntityOwnershipServiceTestFixture.cpp
    EntityOwnershipService/EntityOwnershipServiceTestFixture.h
    EntityOwnershipService/SliceEditorEntityOwnershipTests.cpp
    EntityOwnershipService/SliceEntityOwnershipTests.cpp
    EntityTestbed.h
    FileFunc.cpp
    FingerprintingTests.cpp
    FocusMode/ContainerEntitySelectionTests.cpp
    FocusMode/ContainerEntityTests.cpp
    FocusMode/EditorFocusModeFixture.cpp
    FocusMode/EditorFocusModeFixture.h
    FocusMode/EditorFocusModeSelectionFixture.h
    FocusMode/EditorFocusModeSelectionTests.cpp
    FocusMode/EditorFocusModeTests.cpp
    GenericComponentWrapperTest.cpp
    Input/QtEventToAzInputMapperTests.cpp
    InstanceDataHierarchy.cpp
    IntegerPrimtitiveTestConfig.h
    LogLines.cpp
    ManipulatorBoundsTests.cpp
    ManipulatorCoreTests.cpp
    ManipulatorViewTests.cpp
    MetadataManagerTests.cpp
    PerforceComponentTests.cpp
    PlatformAddressedAssetCatalogTests.cpp
    Prefab/Benchmark/Link/SingleInstanceMultiplePatchesBenchmarks.cpp
    Prefab/Benchmark/Link/SingleInstanceMultiplePatchesBenchmarks.h
    Prefab/Benchmark/PrefabBenchmarkFixture.cpp
    Prefab/Benchmark/PrefabBenchmarkFixture.h
    Prefab/Benchmark/PrefabCreateBenchmarks.cpp
    Prefab/Benchmark/PrefabInstantiateBenchmarks.cpp
    Prefab/Benchmark/PrefabLoadBenchmarks.cpp
    Prefab/Benchmark/PrefabUpdateInstancesBenchmarks.cpp
    Prefab/Benchmark/Propagation/PropagationBenchmarkFixture.cpp
    Prefab/Benchmark/Propagation/PropagationBenchmarkFixture.h
    Prefab/Benchmark/Propagation/SingleInstanceMultipleNestedInstancesBenchmarks.cpp
    Prefab/Benchmark/Propagation/SingleInstanceMultipleNestedInstancesBenchmarks.h
    Prefab/Benchmark/Propagation/SingleInstanceMultipleEntityBenchmarks.cpp
    Prefab/Benchmark/Propagation/SingleInstanceMultipleEntityBenchmarks.h
    Prefab/Benchmark/Propagation/SingleInstanceOverrideBenchmarks.cpp
    Prefab/Benchmark/Propagation/SingleInstanceOverrideBenchmarks.h
    Prefab/Benchmark/SpawnableCreateBenchmarks.cpp
    Prefab/Benchmark/Spawnable/SpawnableBenchmarkFixture.h
    Prefab/Benchmark/Spawnable/SpawnableBenchmarkFixture.cpp
    Prefab/Benchmark/Spawnable/SpawnAllEntitiesBenchmarks.cpp
    Prefab/Instance/InstanceDeserializationTests.cpp
    Prefab/Link/PrefabLinkDomTestFixture.cpp
    Prefab/Link/PrefabLinkDomTestFixture.h
    Prefab/Link/PrefabLinkDomTests.cpp
    Prefab/PrefabFocus/PrefabFocusTests.cpp
    Prefab/MockPrefabFileIOActionValidator.cpp
    Prefab/MockPrefabFileIOActionValidator.h
    Prefab/Overrides/PrefabOverridePublicInterfaceTests.cpp
    Prefab/Overrides/PrefabOverrideTestFixture.cpp
    Prefab/Overrides/PrefabOverrideTestFixture.h
    Prefab/PrefabDeleteTests.cpp
    Prefab/PrefabDeleteAsOverrideTests.cpp
    Prefab/PrefabDuplicateTests.cpp
    Prefab/PrefabEntityAliasTests.cpp
    Prefab/PrefabInstanceToTemplatePropagatorTests.cpp
    Prefab/PrefabInstantiateTests.cpp
    Prefab/PrefabAssetFixupTests.cpp
    Prefab/PrefabLoadTemplateTests.cpp
    Prefab/PrefabTestComponent.cpp
    Prefab/PrefabTestComponent.h
    Prefab/PrefabTestData.cpp
    Prefab/PrefabTestData.h
    Prefab/PrefabTestDataUtils.cpp
    Prefab/PrefabTestDataUtils.h
    Prefab/PrefabTestDomUtils.cpp
    Prefab/PrefabTestDomUtils.h
    Prefab/PrefabTestFixture.cpp
    Prefab/PrefabTestFixture.h
    Prefab/PrefabTestUndoFixture.cpp
    Prefab/PrefabTestUndoFixture.h
    Prefab/PrefabTestUtils.h
    Prefab/PrefabUndoAddEntityTestFixture.cpp
    Prefab/PrefabUndoAddEntityTestFixture.h
    Prefab/PrefabUndoAddEntityTests.cpp
    Prefab/PrefabUndoDeleteTests.cpp
    Prefab/PrefabUndoLinkTests.cpp
    Prefab/PrefabUndoTests.cpp
    Prefab/PrefabUpdateInstancesTests.cpp
    Prefab/PrefabUpdateTemplateTests.cpp
    Prefab/PrefabUpdateWithPatchesTests.cpp
    Prefab/Spawnable/SpawnableMetaDataTests.cpp
    Prefab/Spawnable/SpawnableTestFixture.h
    Prefab/Spawnable/SpawnableTestFixture.cpp
    Prefab/Spawnable/SpawnableTicketTests.cpp
    Prefab/SpawnableCreateTests.cpp
    Prefab/SpawnableRemoveEditorInfoTestFixture.cpp
    Prefab/SpawnableRemoveEditorInfoTestFixture.h
    Prefab/SpawnableRemoveEditorInfoTests.cpp
    Prefab/SpawnableSortEntitiesTestFixture.cpp
    Prefab/SpawnableSortEntitiesTestFixture.h
    Prefab/SpawnableSortEntitiesTests.cpp
    Prefab/PrefabScriptingTests.cpp
    Prefab/ProceduralPrefabAssetTests.cpp
    Prefab/ProceduralPrefabSystemComponentTests.cpp
    PropertyIntCtrlCommonTests.cpp
    PropertyIntCtrlCommonTests.h
    PropertyIntSliderCtrlTests.cpp
    PropertyIntSpinCtrlTests.cpp
    PropertyTreeEditorTests.cpp
    PythonBindingTests.cpp
    QtWidgetLimitsTests.cpp
    Script/LuaEditorSystemComponentTests.cpp
    Script/ScriptComponentTests.cpp
    Script/ScriptEntityTests.cpp
    Slice.cpp
    Slices.cpp
    SliceStabilityTests/SliceStabilityCreateTests.cpp
    SliceStabilityTests/SliceStabilityPushTests.cpp
    SliceStabilityTests/SliceStabilityReParentTests.cpp
    SliceStabilityTests/SliceStabilityTestFramework.cpp
    SliceStabilityTests/SliceStabilityTestFramework.h
    SliceUpgradeTests.cpp
    SliceUpgradeTestsData.h
    SpinBoxTests.cpp
    SQLiteConnectionTests.cpp
    ThumbnailerTests.cpp
    ToolsComponents/EditorLayerComponentTests.cpp
    ToolsComponents/EditorTransformComponentTests.cpp
    TransformComponent.cpp
    UI/EntityIdQLineEditTests.cpp
    UI/EntityOutlinerTests.cpp
    UI/EntityPropertyEditorTests.cpp
    UI/AssetBrowserTests.cpp
    UndoStack.cpp
    Viewport/ClusterTests.cpp
    Viewport/ViewportEditorModeTests.cpp
    Viewport/ViewportScreenTests.cpp
    Viewport/ViewportUiClusterTests.cpp
    Viewport/ViewportUiDisplayTests.cpp
    Viewport/ViewportUiManagerTests.cpp
    Viewport/ViewportUiWidgetManagerTests.cpp
    Viewport/ViewportInteractionTests.cpp
    Viewport/ViewBookmarkTests.cpp
    Visibility/EditorVisibilityTests.cpp
)
