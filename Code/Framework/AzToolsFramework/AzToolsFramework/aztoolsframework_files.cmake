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
    AzToolsFramework_precompiled.h
    AssetEditor/AssetEditorBus.h
    AssetEditor/AssetEditorToolbar.ui
    AssetEditor/AssetEditorStatusBar.ui
    AssetEditor/AssetEditorWidget.cpp
    AssetEditor/AssetEditorWidget.h
    AssetEditor/AssetEditorHeader.cpp
    AssetEditor/AssetEditorHeader.h
    AssetEditor/AssetEditorUtils.h
    AssetEditor/Resources/AssetEditorResources.qrc
    UI/Logging/LogLine.h
    UI/Logging/LogLine.cpp
    UI/PropertyEditor/Resources/Icons.qrc
    UI/SearchWidget/SearchCriteriaWidget.hxx
    UI/SearchWidget/SearchCriteriaWidget.cpp
    UI/SearchWidget/SearchWidgetTypes.hxx
    AzToolsFrameworkModule.h
    AzToolsFrameworkModule.cpp
    API/ToolsApplicationAPI.h
    API/EditorAssetSystemAPI.h
    API/AssetDatabaseBus.h
    API/ComponentEntityObjectBus.h
    API/ComponentEntitySelectionBus.h
    API/EditorCameraBus.h
    API/EditorCameraBus.cpp
    API/EditorAnimationSystemRequestBus.h
    API/EditorEntityAPI.h
    API/EditorLevelNotificationBus.h
    API/EditorVegetationRequestsBus.h
    API/EditorPythonConsoleBus.h
    API/EditorPythonRunnerRequestsBus.h
    API/EntityPropertyEditorRequestsBus.h
    API/EditorWindowRequestBus.h
    API/EntityCompositionRequestBus.h
    API/EntityCompositionNotificationBus.h
    API/EditorViewportIconDisplayInterface.h
    API/ViewPaneOptions.h
    Application/Ticker.h
    Application/Ticker.cpp
    Application/EditorEntityManager.cpp
    Application/EditorEntityManager.h
    Application/ToolsApplication.cpp
    Application/ToolsApplication.h
    Asset/AssetDebugInfo.h
    Asset/AssetDebugInfo.cpp
    Asset/AssetProcessorMessages.cpp
    Asset/AssetProcessorMessages.h
    Asset/AssetSystemComponent.cpp
    Asset/AssetSystemComponent.h
    Asset/AssetSeedManager.h
    Asset/AssetSeedManager.cpp
    Asset/AssetBundler.h
    Asset/AssetBundler.cpp
    Asset/AssetUtils.h
    Asset/AssetUtils.cpp
    AssetCatalog/PlatformAddressedAssetCatalogBus.h
    AssetCatalog/PlatformAddressedAssetCatalog.h
    AssetCatalog/PlatformAddressedAssetCatalog.cpp
    AssetCatalog/PlatformAddressedAssetCatalogManager.h
    AssetCatalog/PlatformAddressedAssetCatalogManager.cpp
    Thumbnails/ThumbnailerComponent.cpp
    Thumbnails/ThumbnailerComponent.h
    Thumbnails/ThumbnailerNullComponent.cpp
    Thumbnails/ThumbnailerNullComponent.h
    Thumbnails/LoadingThumbnail.cpp
    Thumbnails/LoadingThumbnail.h
    Thumbnails/MissingThumbnail.cpp
    Thumbnails/MissingThumbnail.h
    Thumbnails/SourceControlThumbnail.cpp
    Thumbnails/SourceControlThumbnail.h
    Thumbnails/SourceControlThumbnailBus.h
    Thumbnails/Thumbnail.cpp
    Thumbnails/Thumbnail.h
    Thumbnails/Thumbnail.inl
    Thumbnails/ThumbnailContext.cpp
    Thumbnails/ThumbnailContext.h
    Thumbnails/ThumbnailerBus.h
    Thumbnails/ThumbnailWidget.cpp
    Thumbnails/ThumbnailWidget.h
    AssetBundle/AssetBundleAPI.h
    AssetBundle/AssetBundleComponent.cpp
    AssetBundle/AssetBundleComponent.h
    AssetDatabase/AssetDatabaseConnection.cpp
    AssetDatabase/AssetDatabaseConnection.h
    Debug/TraceContext.inl
    Debug/TraceContext.h
    Debug/TraceContextStackInterface.h
    Debug/TraceContextStack.h
    Debug/TraceContextStack.cpp
    Debug/TraceContextSingleStackHandler.h
    Debug/TraceContextSingleStackHandler.cpp
    Debug/TraceContextMultiStackHandler.h
    Debug/TraceContextMultiStackHandler.cpp
    Debug/TraceContextBufferedFormatter.cpp
    Debug/TraceContextBufferedFormatter.inl
    Debug/TraceContextBufferedFormatter.h
    Debug/TraceContextLogFormatter.cpp
    Debug/TraceContextLogFormatter.h
    Component/EditorComponentAPIBus.h
    Component/EditorComponentAPIComponent.cpp
    Component/EditorComponentAPIComponent.h
    Component/EditorLevelComponentAPIBus.h
    Component/EditorLevelComponentAPIComponent.cpp
    Component/EditorLevelComponentAPIComponent.h
    Editor/EditorContextMenuBus.h
    Editor/EditorSettingsAPIBus.h
    Entity/EditorEntityStartStatus.h
    Entity/EditorEntityAPIBus.h
    Entity/EditorEntityContextComponent.cpp
    Entity/EditorEntityContextComponent.h
    Entity/EditorEntityContextBus.h
    Entity/EditorEntityContextPickingBus.h
    Entity/EditorEntityActionComponent.cpp
    Entity/EditorEntityActionComponent.h
    Entity/EditorEntityFixupComponent.cpp
    Entity/EditorEntityFixupComponent.h
    Entity/EditorEntityHelpers.cpp
    Entity/EditorEntityHelpers.h
    Entity/EditorEntityInfoBus.h
    Entity/EditorEntityModel.cpp
    Entity/EditorEntityModel.h
    Entity/EditorEntityModelBus.h
    Entity/EditorEntityModelComponent.cpp
    Entity/EditorEntityModelComponent.h
    Entity/EditorEntityRuntimeActivationBus.h
    Entity/EditorEntitySearchBus.h
    Entity/EditorEntitySearchComponent.cpp
    Entity/EditorEntitySearchComponent.h
    Entity/EditorEntitySortBus.h
    Entity/EditorEntitySortComponent.cpp
    Entity/EditorEntitySortComponent.h
    Entity/EditorEntityTransformBus.h
    Entity/PrefabEditorEntityOwnershipInterface.h
    Entity/PrefabEditorEntityOwnershipService.h
    Entity/PrefabEditorEntityOwnershipService.cpp
    Entity/SliceEditorEntityOwnershipService.h
    Entity/SliceEditorEntityOwnershipService.cpp
    Entity/SliceEditorEntityOwnershipServiceBus.h
    Fingerprinting/TypeFingerprinter.h
    Fingerprinting/TypeFingerprinter.cpp
    Manipulators/AngularManipulator.cpp
    Manipulators/AngularManipulator.h
    Manipulators/BaseManipulator.cpp
    Manipulators/BaseManipulator.h
    Manipulators/BoxManipulatorRequestBus.h
    Manipulators/EditorVertexSelection.h
    Manipulators/EditorVertexSelection.cpp
    Manipulators/HoverSelection.h
    Manipulators/LineHoverSelection.h
    Manipulators/LineHoverSelection.cpp
    Manipulators/LineSegmentSelectionManipulator.cpp
    Manipulators/LineSegmentSelectionManipulator.h
    Manipulators/LinearManipulator.cpp
    Manipulators/LinearManipulator.h
    Manipulators/MultiLinearManipulator.cpp
    Manipulators/MultiLinearManipulator.h
    Manipulators/ManipulatorBus.h
    Manipulators/ManipulatorDebug.cpp
    Manipulators/ManipulatorDebug.h
    Manipulators/ManipulatorManager.cpp
    Manipulators/ManipulatorManager.h
    Manipulators/ManipulatorView.cpp
    Manipulators/ManipulatorView.h
    Manipulators/ManipulatorSnapping.cpp
    Manipulators/ManipulatorSnapping.h
    Manipulators/ManipulatorSpace.cpp
    Manipulators/ManipulatorSpace.h
    Manipulators/PlanarManipulator.cpp
    Manipulators/PlanarManipulator.h
    Manipulators/RotationManipulators.cpp
    Manipulators/RotationManipulators.h
    Manipulators/ScaleManipulators.cpp
    Manipulators/ScaleManipulators.h
    Manipulators/SelectionManipulator.cpp
    Manipulators/SelectionManipulator.h
    Manipulators/SplineHoverSelection.h
    Manipulators/SplineHoverSelection.cpp
    Manipulators/SplineSelectionManipulator.h
    Manipulators/SplineSelectionManipulator.cpp
    Manipulators/SurfaceManipulator.cpp
    Manipulators/SurfaceManipulator.h
    Manipulators/TranslationManipulators.cpp
    Manipulators/TranslationManipulators.h
    Maths/TransformUtils.h
    Picking/BoundInterface.h
    Picking/ContextBoundAPI.h
    Picking/Manipulators/ManipulatorBoundManager.cpp
    Picking/Manipulators/ManipulatorBoundManager.h
    Picking/Manipulators/ManipulatorBounds.cpp
    Picking/Manipulators/ManipulatorBounds.h
    PropertyTreeEditor/PropertyTreeEditor.cpp
    PropertyTreeEditor/PropertyTreeEditor.h
    PropertyTreeEditor/PropertyTreeEditorComponent.cpp
    PropertyTreeEditor/PropertyTreeEditorComponent.h
    Render/EditorIntersectorComponent.cpp
    Render/EditorIntersectorComponent.h
    Slice/SliceDataFlagsCommand.h
    Slice/SliceDataFlagsCommand.cpp
    Slice/SliceUtilities.h
    Slice/SliceUtilities.cpp
    Slice/SliceCompilation.h
    Slice/SliceCompilation.cpp
    Slice/SliceTransaction.cpp
    Slice/SliceTransaction.h
    Slice/SliceDependencyBrowserComponent.h
    Slice/SliceDependencyBrowserComponent.cpp
    Slice/SliceDependencyBrowserBus.h
    Slice/SliceRelationshipNode.h
    Slice/SliceRelationshipNode.cpp
    Slice/SliceRequestBus.h
    Slice/SliceRequestComponent.h
    Slice/SliceRequestComponent.cpp
    Slice/SliceMetadataEntityContextBus.h
    Slice/SliceMetadataEntityContextComponent.h
    Slice/SliceMetadataEntityContextComponent.cpp
    SourceControl/LocalFileSCComponent.cpp
    SourceControl/LocalFileSCComponent.h
    SourceControl/PerforceComponent.cpp
    SourceControl/PerforceComponent.h
    SourceControl/PerforceConnection.cpp
    SourceControl/PerforceConnection.h
    SourceControl/QtSourceControlNotificationHandler.cpp
    SourceControl/QtSourceControlNotificationHandler.h
    SourceControl/SourceControlAPI.h
    SQLite/SQLiteConnection.cpp
    SQLite/SQLiteConnection.h
    SQLite/SQLiteQuery.cpp
    SQLite/SQLiteQuery.h
    SQLite/SQLiteQueryLogBus.h
    SQLite/SQLiteBoundColumnSet.h
    SQLite/SQLiteBoundColumnSet.cpp
    ToolsComponents/ComponentAssetMimeDataContainer.cpp
    ToolsComponents/ComponentAssetMimeDataContainer.h
    ToolsComponents/ComponentMimeData.cpp
    ToolsComponents/ComponentMimeData.h
    ToolsComponents/EditorAssetMimeDataContainer.cpp
    ToolsComponents/EditorAssetMimeDataContainer.h
    ToolsComponents/EditorAssetReference.cpp
    ToolsComponents/EditorAssetReference.h
    ToolsComponents/EditorComponentAdapter.inl
    ToolsComponents/EditorComponentAdapter.h
    ToolsComponents/EditorComponentBase.cpp
    ToolsComponents/EditorComponentBase.h
    ToolsComponents/EditorDisabledCompositionComponent.h
    ToolsComponents/EditorDisabledCompositionComponent.cpp
    ToolsComponents/EditorDisabledCompositionBus.h
    ToolsComponents/EditorOnlyEntityComponent.h
    ToolsComponents/EditorOnlyEntityComponent.cpp
    ToolsComponents/EditorOnlyEntityComponentBus.h
    ToolsComponents/EditorEntityIdContainer.cpp
    ToolsComponents/EditorEntityIdContainer.h
    ToolsComponents/EditorEntityIconComponent.h
    ToolsComponents/EditorEntityIconComponent.cpp
    ToolsComponents/EditorEntityIconComponentBus.h
    ToolsComponents/EditorLayerComponent.h
    ToolsComponents/EditorLayerComponent.cpp
    ToolsComponents/EditorLayerComponentBus.h
    ToolsComponents/LayerResult.cpp
    ToolsComponents/LayerResult.h
    ToolsComponents/EditorLockComponent.cpp
    ToolsComponents/EditorLockComponent.h
    ToolsComponents/EditorLockComponentBus.h
    ToolsComponents/EditorInspectorComponent.h
    ToolsComponents/EditorInspectorComponent.cpp
    ToolsComponents/EditorInspectorComponentBus.h
    ToolsComponents/EditorPendingCompositionComponent.h
    ToolsComponents/EditorPendingCompositionComponent.cpp
    ToolsComponents/EditorPendingCompositionBus.h
    ToolsComponents/EditorSelectionAccentSystemComponent.cpp
    ToolsComponents/EditorSelectionAccentSystemComponent.h
    ToolsComponents/EditorSelectionAccentingBus.h
    ToolsComponents/EditorVisibilityComponent.cpp
    ToolsComponents/EditorVisibilityComponent.h
    ToolsComponents/EditorVisibilityBus.h
    ToolsComponents/GenericComponentWrapper.cpp
    ToolsComponents/GenericComponentWrapper.h
    ToolsComponents/SelectionComponent.cpp
    ToolsComponents/SelectionComponent.h
    ToolsComponents/SelectionComponentBus.h
    ToolsComponents/TransformComponent.h
    ToolsComponents/TransformComponent.cpp
    ToolsComponents/TransformComponentBus.h
    ToolsComponents/ScriptEditorComponent.cpp
    ToolsComponents/ScriptEditorComponent.h
    ToolsComponents/ToolsAssetCatalogComponent.cpp
    ToolsComponents/ToolsAssetCatalogComponent.h
    ToolsComponents/ToolsAssetCatalogBus.h
    ToolsComponents/AzToolsFrameworkConfigurationSystemComponent.h
    ToolsComponents/AzToolsFrameworkConfigurationSystemComponent.cpp
    ToolsComponents/EditorNonUniformScaleComponent.h
    ToolsComponents/EditorNonUniformScaleComponent.cpp
    ToolsComponents/EditorNonUniformScaleComponentMode.h
    ToolsComponents/EditorNonUniformScaleComponentMode.cpp
    ToolsMessaging/EntityHighlightBus.h
    UI/Docking/DockWidgetUtils.cpp
    UI/Docking/DockWidgetUtils.h
    UI/Layer/AddToLayerMenu.cpp
    UI/Layer/AddToLayerMenu.h
    UI/Logging/LoggingCommon.h
    UI/Logging/LogEntry.h
    UI/Logging/LogEntry.cpp
    UI/Logging/LogControl.h
    UI/Logging/LogControl.cpp
    UI/Logging/NewLogTabDialog.h
    UI/Logging/NewLogTabDialog.cpp
    UI/Logging/LogPanel_Panel.h
    UI/Logging/LogPanel_Panel.cpp
    UI/Logging/TracePrintFLogPanel.h
    UI/Logging/TracePrintFLogPanel.cpp
    UI/Logging/GenericLogPanel.h
    UI/Logging/GenericLogPanel.cpp
    UI/Logging/NewLogTabDialog.ui
    UI/Logging/NewLogTabDialog.qss
    UI/Logging/LogTableModel.cpp
    UI/Logging/LogTableModel.h
    UI/Logging/LogTableItemDelegate.cpp
    UI/Logging/LogTableItemDelegate.h
    UI/Logging/StyledLogPanel.cpp
    UI/Logging/StyledLogPanel.h
    UI/Logging/StyledTracePrintFLogPanel.cpp
    UI/Logging/StyledTracePrintFLogPanel.h
    UI/ComponentPalette/ComponentPaletteWidget.hxx
    UI/ComponentPalette/ComponentPaletteWidget.cpp
    UI/ComponentPalette/ComponentPaletteModel.hxx
    UI/ComponentPalette/ComponentPaletteModel.cpp
    UI/ComponentPalette/ComponentPaletteModelFilter.hxx
    UI/ComponentPalette/ComponentPaletteModelFilter.cpp
    UI/ComponentPalette/ComponentPaletteUtil.hxx
    UI/ComponentPalette/ComponentPaletteUtil.cpp
    UI/Layer/NameConflictWarning.hxx
    UI/Layer/NameConflictWarning.cpp
    UI/PropertyEditor/EntityPropertyEditor.ui
    UI/PropertyEditor/ComponentEditor.hxx
    UI/PropertyEditor/ComponentEditor.cpp
    UI/PropertyEditor/ComponentEditorHeader.hxx
    UI/PropertyEditor/ComponentEditorHeader.cpp
    UI/PropertyEditor/QtWidgetLimits.h
    UI/PropertyEditor/DHQComboBox.hxx
    UI/PropertyEditor/DHQComboBox.cpp
    UI/PropertyEditor/DHQSlider.hxx
    UI/PropertyEditor/DHQSlider.cpp
    UI/PropertyEditor/EntityIdQLabel.hxx
    UI/PropertyEditor/EntityIdQLabel.cpp
    UI/PropertyEditor/EntityIdQLineEdit.h
    UI/PropertyEditor/EntityIdQLineEdit.cpp
    UI/PropertyEditor/EntityPropertyEditor.hxx
    UI/PropertyEditor/EntityPropertyEditor.cpp
    UI/PropertyEditor/GenericComboBoxCtrl.h
    UI/PropertyEditor/GenericComboBoxCtrl.cpp
    UI/PropertyEditor/GenericComboBoxCtrl.inl
    UI/PropertyEditor/InstanceDataHierarchy.cpp
    UI/PropertyEditor/InstanceDataHierarchy.h
    UI/PropertyEditor/PropertyAssetCtrl.hxx
    UI/PropertyEditor/PropertyAssetCtrl.cpp
    UI/PropertyEditor/PropertyAudioCtrl.h
    UI/PropertyEditor/PropertyAudioCtrl.cpp
    UI/PropertyEditor/PropertyAudioCtrlTypes.h
    UI/PropertyEditor/PropertyBoolComboBoxCtrl.hxx
    UI/PropertyEditor/PropertyBoolComboBoxCtrl.cpp
    UI/PropertyEditor/PropertyBoolRadioButtonsCtrl.hxx
    UI/PropertyEditor/PropertyBoolRadioButtonsCtrl.cpp
    UI/PropertyEditor/PropertyButtonCtrl.hxx
    UI/PropertyEditor/PropertyButtonCtrl.cpp
    UI/PropertyEditor/PropertyCheckBoxCtrl.hxx
    UI/PropertyEditor/PropertyCheckBoxCtrl.cpp
    UI/PropertyEditor/PropertyColorCtrl.hxx
    UI/PropertyEditor/PropertyColorCtrl.cpp
    UI/PropertyEditor/PropertyDoubleSliderCtrl.hxx
    UI/PropertyEditor/PropertyDoubleSliderCtrl.cpp
    UI/PropertyEditor/PropertyDoubleSpinCtrl.hxx
    UI/PropertyEditor/PropertyDoubleSpinCtrl.cpp
    UI/PropertyEditor/PropertyEditor_UITypes.h
    UI/PropertyEditor/PropertyEditorAPI.h
    UI/PropertyEditor/PropertyEditorApi.cpp
    UI/PropertyEditor/PropertyEditorAPI_Internals.h
    UI/PropertyEditor/PropertyEditorAPI_Internals_Impl.h
    UI/PropertyEditor/PropertyEntityIdCtrl.hxx
    UI/PropertyEditor/PropertyEntityIdCtrl.cpp
    UI/PropertyEditor/PropertyEnumComboBoxCtrl.hxx
    UI/PropertyEditor/PropertyEnumComboBoxCtrl.cpp
    UI/PropertyEditor/PropertyIntCtrlCommon.h
    UI/PropertyEditor/PropertyIntSliderCtrl.hxx
    UI/PropertyEditor/PropertyIntSliderCtrl.cpp
    UI/PropertyEditor/PropertyIntSpinCtrl.hxx
    UI/PropertyEditor/PropertyIntSpinCtrl.cpp
    UI/PropertyEditor/PropertyCRCCtrl.h
    UI/PropertyEditor/PropertyCRCCtrl.cpp
    UI/PropertyEditor/PropertyManagerComponent.cpp
    UI/PropertyEditor/PropertyManagerComponent.h
    UI/PropertyEditor/PropertyQTConstants.h
    UI/PropertyEditor/PropertyRowWidget.hxx
    UI/PropertyEditor/PropertyRowWidget.cpp
    UI/PropertyEditor/PropertyStringComboBoxCtrl.hxx
    UI/PropertyEditor/PropertyStringComboBoxCtrl.cpp
    UI/PropertyEditor/PropertyStringLineEditCtrl.hxx
    UI/PropertyEditor/PropertyStringLineEditCtrl.cpp
    UI/PropertyEditor/PropertyVectorCtrl.hxx
    UI/PropertyEditor/PropertyVectorCtrl.cpp
    UI/PropertyEditor/ReflectedPropertyEditor.hxx
    UI/PropertyEditor/ReflectedPropertyEditor.cpp
    UI/PropertyEditor/GrowTextEdit.h
    UI/PropertyEditor/GrowTextEdit.cpp
    UI/PropertyEditor/MultiLineTextEditHandler.h
    UI/PropertyEditor/MultiLineTextEditHandler.cpp
    UI/PropertyEditor/ThumbnailPropertyCtrl.h
    UI/PropertyEditor/ThumbnailPropertyCtrl.cpp
    UI/Slice/SlicePushWidget.cpp
    UI/Slice/SlicePushWidget.hxx
    UI/Slice/SliceOverridesNotificationWindow.cpp
    UI/Slice/SliceOverridesNotificationWindow.hxx
    UI/Slice/SliceOverridesNotificationWindowManager.cpp
    UI/Slice/SliceOverridesNotificationWindowManager.hxx
    UI/Slice/NotificationWindow.ui
    UI/Slice/Constants.h
    UI/Slice/SliceRelationshipWidget.cpp
    UI/Slice/SliceRelationshipWidget.hxx
    UI/UICore/AspectRatioAwarePixmapWidget.hxx
    UI/UICore/AspectRatioAwarePixmapWidget.cpp
    UI/UICore/AZAutoSizingScrollArea.hxx
    UI/UICore/AZAutoSizingScrollArea.cpp
    UI/UICore/ColorPickerDelegate.hxx
    UI/UICore/ColorPickerDelegate.cpp
    UI/UICore/ClickableLabel.hxx
    UI/UICore/ClickableLabel.cpp
    UI/UICore/IconButton.hxx
    UI/UICore/IconButton.cpp
    UI/UICore/PlainTextEdit.hxx
    UI/UICore/PlainTextEdit.cpp
    UI/UICore/ProgressShield.hxx
    UI/UICore/ProgressShield.cpp
    UI/UICore/ProgressShield.ui
    UI/UICore/QTreeViewStateSaver.hxx
    UI/UICore/QTreeViewStateSaver.cpp
    UI/UICore/QWidgetSavedState.h
    UI/UICore/QWidgetSavedState.cpp
    UI/UICore/TargetSelectorButton.hxx
    UI/UICore/TargetSelectorButton.cpp
    UI/UICore/WidgetHelpers.h
    Undo/UndoSystem.cpp
    Undo/UndoSystem.h
    Undo/UndoCacheInterface.h
    Commands/ComponentModeCommand.cpp
    Commands/ComponentModeCommand.h
    Commands/EntityManipulatorCommand.h
    Commands/EntityManipulatorCommand.cpp
    Commands/EntityStateCommand.cpp
    Commands/EntityStateCommand.h
    Commands/SelectionCommand.cpp
    Commands/SelectionCommand.h
    Commands/EntityTransformCommand.cpp
    Commands/EntityTransformCommand.h
    Commands/PreemptiveUndoCache.cpp
    Commands/PreemptiveUndoCache.h
    Commands/LegacyCommand.h
    Commands/BaseSliceCommand.cpp
    Commands/BaseSliceCommand.h
    Commands/SliceDetachEntityCommand.cpp
    Commands/SliceDetachEntityCommand.h
    Commands/DetachSubSliceInstanceCommand.cpp
    Commands/DetachSubSliceInstanceCommand.h
    Commands/PushToSliceCommand.cpp
    Commands/PushToSliceCommand.h
    Commands/CreateSliceCommand.cpp
    Commands/CreateSliceCommand.h
    Viewport/ActionBus.h
    Viewport/EditorContextMenu.h
    Viewport/EditorContextMenu.cpp
    Viewport/VertexContainerDisplay.h
    Viewport/VertexContainerDisplay.cpp
    Viewport/ViewportMessages.h
    Viewport/ViewportTypes.h
    Viewport/ViewportTypes.cpp
    ViewportUi/Button.h
    ViewportUi/Button.cpp
    ViewportUi/ButtonGroup.h
    ViewportUi/ButtonGroup.cpp
    ViewportUi/TextField.h
    ViewportUi/TextField.cpp
    ViewportUi/ViewportUiDisplay.h
    ViewportUi/ViewportUiDisplay.cpp
    ViewportUi/ViewportUiManager.h
    ViewportUi/ViewportUiManager.cpp
    ViewportUi/ViewportUiRequestBus.h
    ViewportUi/ViewportUiTextField.h
    ViewportUi/ViewportUiTextField.cpp
    ViewportUi/ViewportUiCluster.h
    ViewportUi/ViewportUiCluster.cpp
    ViewportUi/ViewportUiSwitcher.h
    ViewportUi/ViewportUiSwitcher.cpp
    ViewportUi/ViewportUiWidgetCallbacks.h
    ViewportUi/ViewportUiWidgetCallbacks.cpp
    ViewportUi/ViewportUiDisplayLayout.h
    ViewportUi/ViewportUiDisplayLayout.cpp
    ComponentMode/EditorComponentModeBus.h
    ComponentMode/ComponentModeCollection.h
    ComponentMode/ComponentModeCollection.cpp
    ComponentMode/ComponentModeDelegate.h
    ComponentMode/ComponentModeDelegate.cpp
    ComponentMode/ComponentModeViewportUi.h
    ComponentMode/ComponentModeViewportUi.cpp
    ComponentMode/ComponentModeViewportUiRequestBus.h
    ComponentMode/EditorBaseComponentMode.h
    ComponentMode/EditorBaseComponentMode.cpp
    ComponentModes/BoxComponentMode.h
    ComponentModes/BoxComponentMode.cpp
    ComponentModes/BoxViewportEdit.h
    ComponentModes/BoxViewportEdit.cpp
    ViewportSelection/EditorBoxSelect.h
    ViewportSelection/EditorBoxSelect.cpp
    ViewportSelection/EditorDefaultSelection.h
    ViewportSelection/EditorDefaultSelection.cpp
    ViewportSelection/EditorHelpers.h
    ViewportSelection/EditorHelpers.cpp
    ViewportSelection/EditorInteractionSystemComponent.h
    ViewportSelection/EditorInteractionSystemComponent.cpp
    ViewportSelection/EditorInteractionSystemViewportSelectionRequestBus.h
    ViewportSelection/EditorPickEntitySelection.h
    ViewportSelection/EditorPickEntitySelection.cpp
    ViewportSelection/EditorSelectionUtil.h
    ViewportSelection/EditorSelectionUtil.cpp
    ViewportSelection/EditorTransformComponentSelection.h
    ViewportSelection/EditorTransformComponentSelection.cpp
    ViewportSelection/EditorTransformComponentSelectionRequestBus.h
    ViewportSelection/EditorTransformComponentSelectionRequestBus.cpp
    ViewportSelection/EditorVisibleEntityDataCache.h
    ViewportSelection/EditorVisibleEntityDataCache.cpp
    ToolsFileUtils/ToolsFileUtils.h
    AssetBrowser/AssetBrowserBus.h
    AssetBrowser/AssetBrowserSourceDropBus.h
    AssetBrowser/AssetBrowserComponent.cpp
    AssetBrowser/AssetBrowserComponent.h
    AssetBrowser/AssetBrowserEntry.h
    AssetBrowser/AssetBrowserFilterModel.cpp
    AssetBrowser/AssetBrowserFilterModel.h
    AssetBrowser/AssetBrowserModel.cpp
    AssetBrowser/AssetBrowserModel.h
    AssetBrowser/AssetEntryChange.h
    AssetBrowser/AssetEntryChangeset.cpp
    AssetBrowser/AssetEntryChangeset.h
    AssetBrowser/AssetSelectionModel.cpp
    AssetBrowser/AssetSelectionModel.h
    AssetBrowser/EBusFindAssetTypeByName.h
    AssetBrowser/Views/AssetBrowserTreeView.cpp
    AssetBrowser/Views/AssetBrowserTreeView.h
    AssetBrowser/Views/EntryDelegate.cpp
    AssetBrowser/Views/EntryDelegate.h
    AssetBrowser/Views/AssetBrowserFolderWidget.cpp
    AssetBrowser/Views/AssetBrowserFolderWidget.h
    AssetBrowser/Views/AssetBrowserFolderWidget.qrc
    AssetBrowser/Entries/AssetBrowserEntry.cpp
    AssetBrowser/Entries/AssetBrowserEntry.h
    AssetBrowser/Entries/AssetBrowserEntry.inl
    AssetBrowser/Entries/AssetBrowserEntryCache.cpp
    AssetBrowser/Entries/AssetBrowserEntryCache.h
    AssetBrowser/Entries/FolderAssetBrowserEntry.cpp
    AssetBrowser/Entries/FolderAssetBrowserEntry.h
    AssetBrowser/Entries/ProductAssetBrowserEntry.cpp
    AssetBrowser/Entries/ProductAssetBrowserEntry.h
    AssetBrowser/Entries/RootAssetBrowserEntry.cpp
    AssetBrowser/Entries/RootAssetBrowserEntry.h
    AssetBrowser/Entries/SourceAssetBrowserEntry.cpp
    AssetBrowser/Entries/SourceAssetBrowserEntry.h
    AssetBrowser/Search/Filter.cpp
    AssetBrowser/Search/Filter.h
    AssetBrowser/Search/FilterByWidget.cpp
    AssetBrowser/Search/FilterByWidget.h
    AssetBrowser/Search/FilterByWidget.ui
    AssetBrowser/Search/search.qrc
    AssetBrowser/Search/SearchAssetTypeSelectorWidget.cpp
    AssetBrowser/Search/SearchAssetTypeSelectorWidget.h
    AssetBrowser/Search/SearchAssetTypeSelectorWidget.ui
    AssetBrowser/Search/SearchParametersWidget.cpp
    AssetBrowser/Search/SearchParametersWidget.h
    AssetBrowser/Search/SearchParametersWidget.ui
    AssetBrowser/Search/SearchWidget.cpp
    AssetBrowser/Search/SearchWidget.h
    AssetBrowser/Search/SearchWidget.ui
    AssetBrowser/Thumbnails/FolderThumbnail.cpp
    AssetBrowser/Thumbnails/FolderThumbnail.h
    AssetBrowser/Thumbnails/ProductThumbnail.cpp
    AssetBrowser/Thumbnails/ProductThumbnail.h
    AssetBrowser/Thumbnails/SourceThumbnail.cpp
    AssetBrowser/Thumbnails/SourceThumbnail.h
    AssetBrowser/AssetPicker/AssetPickerDialog.cpp
    AssetBrowser/AssetPicker/AssetPickerDialog.h
    AssetBrowser/AssetPicker/AssetPickerDialog.ui
    AssetBrowser/Previewer/EmptyPreviewer.cpp
    AssetBrowser/Previewer/EmptyPreviewer.h
    AssetBrowser/Previewer/EmptyPreviewer.ui
    AssetBrowser/Previewer/Previewer.cpp
    AssetBrowser/Previewer/Previewer.h
    AssetBrowser/Previewer/PreviewerBus.h
    AssetBrowser/Previewer/PreviewerFactory.h
    AssetBrowser/Previewer/PreviewerFrame.cpp
    AssetBrowser/Previewer/PreviewerFrame.h
    Archive/ArchiveComponent.h
    Archive/ArchiveComponent.cpp
    Archive/NullArchiveComponent.h
    Archive/NullArchiveComponent.cpp
    Archive/ArchiveAPI.h
    UI/PropertyEditor/Model/AssetCompleterModel.h
    UI/PropertyEditor/Model/AssetCompleterModel.cpp
    UI/PropertyEditor/View/AssetCompleterListView.h
    UI/PropertyEditor/View/AssetCompleterListView.cpp
    Prefab/EditorPrefabComponent.h
    Prefab/EditorPrefabComponent.cpp
    Prefab/PrefabDomTypes.h
    Prefab/PrefabDomUtils.h
    Prefab/PrefabDomUtils.cpp
    Prefab/PrefabIdTypes.h
    Prefab/PrefabLoader.h
    Prefab/PrefabLoader.cpp
    Prefab/PrefabLoaderInterface.h
    Prefab/PrefabSystemComponent.h
    Prefab/PrefabSystemComponent.cpp
    Prefab/PrefabSystemComponentInterface.h
    Prefab/Instance/Instance.h
    Prefab/Instance/Instance.cpp
    Prefab/Instance/InstanceSerializer.h
    Prefab/Instance/InstanceSerializer.cpp
    Prefab/Instance/InstanceEntityScrubber.h
    Prefab/Instance/InstanceEntityScrubber.cpp
    Prefab/Instance/InstanceEntityIdMapper.h
    Prefab/Instance/InstanceEntityIdMapper.cpp
    Prefab/Instance/InstanceEntityMapper.h
    Prefab/Instance/InstanceEntityMapper.cpp
    Prefab/Instance/InstanceEntityMapperInterface.h
    Prefab/Instance/InstanceToTemplateInterface.h
    Prefab/Instance/InstanceToTemplatePropagator.cpp
    Prefab/Instance/InstanceToTemplatePropagator.h
    Prefab/Instance/InstanceUpdateExecutor.cpp
    Prefab/Instance/InstanceUpdateExecutor.h
    Prefab/Instance/InstanceUpdateExecutorInterface.h
    Prefab/Instance/TemplateInstanceMapper.h
    Prefab/Instance/TemplateInstanceMapper.cpp
    Prefab/Instance/TemplateInstanceMapperInterface.h
    Prefab/Link/Link.h
    Prefab/Link/Link.cpp
    Prefab/PrefabPublicHandler.h
    Prefab/PrefabPublicHandler.cpp
    Prefab/PrefabPublicInterface.h
    Prefab/PrefabPublicNotificationBus.h
    Prefab/PrefabUndo.h
    Prefab/PrefabUndo.cpp
    Prefab/PrefabUndoCache.cpp
    Prefab/PrefabUndoCache.h
    Prefab/PrefabUndoHelpers.cpp
    Prefab/PrefabUndoHelpers.h
    Prefab/Spawnable/ComponentRequirementsValidator.h
    Prefab/Spawnable/ComponentRequirementsValidator.cpp
    Prefab/Spawnable/EditorInfoRemover.h
    Prefab/Spawnable/EditorInfoRemover.cpp
    Prefab/Spawnable/EditorOnlyEntityHandler/EditorOnlyEntityHandler.h
    Prefab/Spawnable/EditorOnlyEntityHandler/EditorOnlyEntityHandler.cpp
    Prefab/Spawnable/EditorOnlyEntityHandler/UiEditorOnlyEntityHandler.h
    Prefab/Spawnable/EditorOnlyEntityHandler/UiEditorOnlyEntityHandler.cpp
    Prefab/Spawnable/EditorOnlyEntityHandler/WorldEditorOnlyEntityHandler.h
    Prefab/Spawnable/EditorOnlyEntityHandler/WorldEditorOnlyEntityHandler.cpp
    Prefab/Spawnable/PrefabCatchmentProcessor.h
    Prefab/Spawnable/PrefabCatchmentProcessor.cpp
    Prefab/Spawnable/PrefabConversionPipeline.h
    Prefab/Spawnable/PrefabConversionPipeline.cpp
    Prefab/Spawnable/ProcesedObjectStore.h
    Prefab/Spawnable/ProcesedObjectStore.cpp
    Prefab/Spawnable/PrefabProcessor.h
    Prefab/Spawnable/PrefabProcessorContext.h
    Prefab/Spawnable/PrefabProcessorContext.cpp
    Prefab/Spawnable/SpawnableMetaDataBuilder.h
    Prefab/Spawnable/SpawnableMetaDataBuilder.cpp
    Prefab/Spawnable/SpawnableUtils.h
    Prefab/Spawnable/SpawnableUtils.cpp
    Prefab/Template/Template.h
    Prefab/Template/Template.cpp
    UI/Outliner/EntityOutlinerDisplayOptionsMenu.h
    UI/Outliner/EntityOutlinerDisplayOptionsMenu.cpp
    UI/Outliner/EntityOutlinerTreeView.hxx
    UI/Outliner/EntityOutlinerTreeView.cpp
    UI/Outliner/EntityOutlinerWidget.hxx
    UI/Outliner/EntityOutlinerWidget.cpp
    UI/Outliner/EntityOutlinerCacheBus.h
    UI/Outliner/EntityOutlinerListModel.hxx
    UI/Outliner/EntityOutlinerListModel.cpp
    UI/Outliner/EntityOutlinerSearchWidget.h
    UI/Outliner/EntityOutlinerSearchWidget.cpp
    UI/Outliner/EntityOutlinerSortFilterProxyModel.hxx
    UI/Outliner/EntityOutlinerSortFilterProxyModel.cpp
    UI/Outliner/EntityOutlinerWidget.ui
    UI/Outliner/EntityOutliner.qss
    UI/Outliner/resources.qrc
    UI/EditorEntityUi/EditorEntityUiInterface.h
    UI/EditorEntityUi/EditorEntityUiHandlerBase.h
    UI/EditorEntityUi/EditorEntityUiHandlerBase.cpp
    UI/EditorEntityUi/EditorEntityUiSystemComponent.h
    UI/EditorEntityUi/EditorEntityUiSystemComponent.cpp
    UI/Layer/LayerUiHandler.h
    UI/Layer/LayerUiHandler.cpp
    UI/Prefab/LevelRootUiHandler.h
    UI/Prefab/LevelRootUiHandler.cpp
    UI/Prefab/PrefabEditInterface.h
    UI/Prefab/PrefabEditManager.h
    UI/Prefab/PrefabEditManager.cpp
    UI/Prefab/PrefabIntegrationBus.h
    UI/Prefab/PrefabIntegrationManager.h
    UI/Prefab/PrefabIntegrationManager.cpp
    UI/Prefab/PrefabIntegrationInterface.h
    UI/Prefab/PrefabUiHandler.h
    UI/Prefab/PrefabUiHandler.cpp
    PythonTerminal/ScriptHelpDialog.cpp
    PythonTerminal/ScriptHelpDialog.h
    PythonTerminal/ScriptHelpDialog.ui
    PythonTerminal/ScriptTermDialog.cpp
    PythonTerminal/ScriptTermDialog.h
    PythonTerminal/ScriptTermDialog.ui
)

# Prevent the following files from being grouped in UNITY builds
set(SKIP_UNITY_BUILD_INCLUSION_FILES
    # The following files are skipped from unity to avoid duplicated symbols related to an ebus
    AzToolsFrameworkModule.cpp
    Application/ToolsApplication.cpp
    UI/PropertyEditor/PropertyEntityIdCtrl.cpp
    UI/PropertyEditor/PropertyManagerComponent.cpp
    ViewportSelection/EditorDefaultSelection.cpp
    ViewportSelection/EditorInteractionSystemComponent.cpp
    ViewportSelection/EditorTransformComponentSelection.cpp
)
