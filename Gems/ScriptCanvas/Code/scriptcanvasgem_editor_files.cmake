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
    Editor/precompiled.h
    Editor/ScriptCanvasEditorGem.cpp
    Editor/Settings.h
    Editor/Settings.cpp
    Editor/ReflectComponent.h
    Editor/ReflectComponent.cpp
    Editor/SystemComponent.h
    Editor/SystemComponent.cpp
    Editor/QtMetaTypes.h
    Editor/Assets/ScriptCanvasAssetTracker.cpp
    Editor/Assets/ScriptCanvasAssetTracker.h
    Editor/Assets/ScriptCanvasAssetTrackerBus.h
    Editor/Assets/ScriptCanvasAssetHelpers.h
    Editor/Assets/ScriptCanvasAssetHelpers.cpp
    Editor/Assets/ScriptCanvasAssetTrackerDefinitions.h
    Editor/Include/ScriptCanvas/Assets/Functions/ScriptCanvasFunctionAssetHandler.h
    Editor/Assets/Functions/ScriptCanvasFunctionAssetHandler.cpp
    Editor/Include/ScriptCanvas/Assets/ScriptCanvasAsset.h
    Editor/Assets/ScriptCanvasAsset.cpp
    Editor/Include/ScriptCanvas/Assets/ScriptCanvasAssetBus.h
    Editor/Include/ScriptCanvas/Assets/ScriptCanvasAssetTypes.h
    Editor/Include/ScriptCanvas/Assets/ScriptCanvasAssetHandler.h
    Editor/Assets/ScriptCanvasAssetHandler.cpp
    Editor/Assets/ScriptCanvasAssetHolder.h
    Editor/Assets/ScriptCanvasAssetHolder.cpp
    Editor/Assets/ScriptCanvasAssetInstance.h
    Editor/Assets/ScriptCanvasAssetInstance.cpp
    Editor/Assets/ScriptCanvasAssetReference.h
    Editor/Assets/ScriptCanvasAssetReference.cpp
    Editor/Assets/ScriptCanvasAssetReferenceContainer.h
    Editor/Assets/ScriptCanvasMemoryAsset.h
    Editor/Assets/ScriptCanvasMemoryAsset.cpp
    Editor/Assets/ScriptCanvasUndoHelper.h
    Editor/Assets/ScriptCanvasUndoHelper.cpp
    Editor/Include/ScriptCanvas/Bus/RequestBus.h
    Editor/Include/ScriptCanvas/Bus/NodeIdPair.h
    Editor/Include/ScriptCanvas/Bus/EditorScriptCanvasBus.h
    Editor/Include/ScriptCanvas/Bus/ScriptCanvasBus.h
    Editor/Include/ScriptCanvas/Bus/IconBus.h
    Editor/Include/ScriptCanvas/Bus/GraphBus.h
    Editor/Include/ScriptCanvas/Bus/UndoBus.h
    Editor/Include/ScriptCanvas/Bus/EditorSceneVariableManagerBus.h
    Editor/Include/ScriptCanvas/Bus/ScriptCanvasExecutionBus.h
    Editor/Include/ScriptCanvas/Bus/UnitTestVerificationBus.h
    Editor/Include/ScriptCanvas/Components/GraphUpgrade.h
    Editor/Include/ScriptCanvas/Components/EditorGraph.h
    Editor/Include/ScriptCanvas/Components/EditorUtils.h
    Editor/Components/EditorGraph.cpp
    Editor/Components/EditorUtils.cpp
    Editor/Components/GraphUpgrade.cpp
    Editor/Include/ScriptCanvas/Components/EditorGraphVariableManagerComponent.h
    Editor/Components/EditorGraphVariableManagerComponent.cpp
    Editor/Include/ScriptCanvas/Components/EditorScriptCanvasComponent.h
    Editor/Components/EditorScriptCanvasComponent.cpp
    Editor/Components/IconComponent.h
    Editor/Components/IconComponent.cpp
    Editor/Include/ScriptCanvas/GraphCanvas/DynamicSlotBus.h
    Editor/Include/ScriptCanvas/GraphCanvas/NodeDescriptorBus.h
    Editor/Include/ScriptCanvas/GraphCanvas/MappingBus.h
    Editor/GraphCanvas/AutomationIds.h
    Editor/GraphCanvas/GraphCanvasEditorNotificationBusId.h
    Editor/GraphCanvas/PropertySlotIds.h
    Editor/GraphCanvas/Components/DynamicOrderingDynamicSlotComponent.cpp
    Editor/GraphCanvas/Components/DynamicOrderingDynamicSlotComponent.h
    Editor/GraphCanvas/Components/DynamicSlotComponent.cpp
    Editor/GraphCanvas/Components/DynamicSlotComponent.h
    Editor/GraphCanvas/Components/MappingComponent.cpp
    Editor/GraphCanvas/Components/MappingComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/ClassMethodNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/ClassMethodNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/AzEventHandlerNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/AzEventHandlerNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/EBusHandlerEventNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/EBusHandlerEventNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/EBusHandlerNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/EBusHandlerNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/EBusSenderNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/EBusSenderNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/EntityRefNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/EntityRefNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/FunctionNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/FunctionNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/FunctionDefinitionNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/FunctionDefinitionNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/GetVariableNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/GetVariableNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/NodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/NodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/NodelingDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/NodelingDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/SetVariableNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/SetVariableNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/UserDefinedNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/UserDefinedNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/VariableNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/VariableNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/ScriptEventReceiverEventNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/ScriptEventReceiverEventNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/ScriptEventReceiverNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/ScriptEventReceiverNodeDescriptorComponent.h
    Editor/GraphCanvas/Components/NodeDescriptors/ScriptEventSenderNodeDescriptorComponent.cpp
    Editor/GraphCanvas/Components/NodeDescriptors/ScriptEventSenderNodeDescriptorComponent.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasAssetIdDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasBoolDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasEntityIdDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasEnumDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasColorDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasCRCDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasNumericDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasReadOnlyDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasStringDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasVectorDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasVariableDataInterface.h
    Editor/GraphCanvas/DataInterfaces/ScriptCanvasQuaternionDataInterface.h
    Editor/GraphCanvas/PropertyInterfaces/ScriptCanvasPropertyDataInterface.h
    Editor/GraphCanvas/PropertyInterfaces/ScriptCanvasStringPropertyDataInterface.h
    Editor/Model/EntityMimeDataHandler.h
    Editor/Model/EntityMimeDataHandler.cpp
    Editor/Model/LibraryDataModel.h
    Editor/Model/LibraryDataModel.cpp
    Editor/Model/UnitTestBrowserFilterModel.h
    Editor/Model/UnitTestBrowserFilterModel.cpp
    Editor/Nodes/EditorLibrary.h
    Editor/Nodes/EditorLibrary.cpp
    Editor/Nodes/NodeCreateUtils.h
    Editor/Nodes/NodeCreateUtils.cpp
    Editor/Nodes/NodeDisplayUtils.h
    Editor/Nodes/NodeDisplayUtils.cpp
    Editor/Nodes/NodeUtils.h
    Editor/Nodes/NodeUtils.cpp
    Editor/Nodes/ScriptCanvasAssetNode.h
    Editor/Nodes/ScriptCanvasAssetNode.cpp
    Editor/Translation/TranslationHelper.h
    Editor/Undo/ScriptCanvasGraphCommand.cpp
    Editor/Undo/ScriptCanvasGraphCommand.h
    Editor/Undo/ScriptCanvasUndoManager.cpp
    Editor/Undo/ScriptCanvasUndoManager.h
    Editor/Utilities/Command.h
    Editor/Utilities/Command.cpp
    Editor/Utilities/CommonSettingsConfigurations.h
    Editor/Utilities/CommonSettingsConfigurations.cpp
    Editor/Utilities/RecentFiles.h
    Editor/Utilities/RecentFiles.cpp
    Editor/Utilities/RecentAssetPath.h
    Editor/Utilities/RecentAssetPath.cpp
    Editor/View/Dialogs/NewGraphDialog.h
    Editor/View/Dialogs/NewGraphDialog.cpp
    Editor/View/Dialogs/NewGraphDialog.ui
    Editor/View/Dialogs/SettingsDialog.h
    Editor/View/Dialogs/SettingsDialog.cpp
    Editor/View/Dialogs/SettingsDialog.ui
    Editor/View/Dialogs/UnsavedChangesDialog.h
    Editor/View/Dialogs/UnsavedChangesDialog.cpp
    Editor/View/Dialogs/UnsavedChangesDialog.ui
    Editor/View/Dialogs/ContainerWizard/ContainerTypeLineEdit.h
    Editor/View/Dialogs/ContainerWizard/ContainerTypeLineEdit.cpp
    Editor/View/Dialogs/ContainerWizard/ContainerTypeLineEdit.ui
    Editor/View/Dialogs/ContainerWizard/ContainerWizard.h
    Editor/View/Dialogs/ContainerWizard/ContainerWizard.cpp
    Editor/View/Dialogs/ContainerWizard/ContainerWizard.ui
    Editor/View/Widgets/CanvasWidget.ui
    Editor/View/Widgets/CanvasWidget.h
    Editor/View/Widgets/CanvasWidget.cpp
    Editor/View/Widgets/GraphTabBar.h
    Editor/View/Widgets/GraphTabBar.cpp
    Editor/View/Widgets/PropertyGrid.h
    Editor/View/Widgets/PropertyGrid.cpp
    Editor/View/Widgets/PropertyGridBus.h
    Editor/View/Widgets/PropertyGridContextMenu.h
    Editor/View/Widgets/PropertyGridContextMenu.cpp
    Editor/View/Widgets/CommandLine.h
    Editor/View/Widgets/CommandLine.cpp
    Editor/View/Widgets/CommandLine.ui
    Editor/View/Widgets/LogPanel.h
    Editor/View/Widgets/LogPanel.cpp
    Editor/View/Widgets/LogPanel.ui
    Editor/View/Widgets/MainWindowStatusWidget.h
    Editor/View/Widgets/MainWindowStatusWidget.cpp
    Editor/View/Widgets/MainWindowStatusWidget.ui
    Editor/View/Widgets/ScriptCanvasNodePaletteDockWidget.h
    Editor/View/Widgets/ScriptCanvasNodePaletteDockWidget.cpp
    Editor/View/Widgets/ScriptCanvasNodePaletteToolbar.ui
    Editor/View/Widgets/WidgetBus.h
    Editor/View/Widgets/DataTypePalette/DataTypePaletteModel.cpp
    Editor/View/Widgets/DataTypePalette/DataTypePaletteModel.h
    Editor/View/Widgets/NodePalette/CreateNodeMimeEvent.cpp
    Editor/View/Widgets/NodePalette/CreateNodeMimeEvent.h
    Editor/View/Widgets/NodePalette/FunctionNodePaletteTreeItemTypes.cpp
    Editor/View/Widgets/NodePalette/FunctionNodePaletteTreeItemTypes.h
    Editor/View/Widgets/NodePalette/ScriptEventsNodePaletteTreeItemTypes.cpp
    Editor/View/Widgets/NodePalette/ScriptEventsNodePaletteTreeItemTypes.h
    Editor/View/Widgets/NodePalette/EBusNodePaletteTreeItemTypes.cpp
    Editor/View/Widgets/NodePalette/EBusNodePaletteTreeItemTypes.h
    Editor/View/Widgets/NodePalette/GeneralNodePaletteTreeItemTypes.cpp
    Editor/View/Widgets/NodePalette/GeneralNodePaletteTreeItemTypes.h
    Editor/View/Widgets/NodePalette/NodePaletteModelBus.h
    Editor/View/Widgets/NodePalette/NodePaletteModel.cpp
    Editor/View/Widgets/NodePalette/NodePaletteModel.h
    Editor/View/Widgets/NodePalette/SpecializedNodePaletteTreeItemTypes.cpp
    Editor/View/Widgets/NodePalette/SpecializedNodePaletteTreeItemTypes.h
    Editor/View/Widgets/NodePalette/VariableNodePaletteTreeItemTypes.cpp
    Editor/View/Widgets/NodePalette/VariableNodePaletteTreeItemTypes.h
    Editor/View/Widgets/LoggingPanel/LoggingDataAggregator.cpp
    Editor/View/Widgets/LoggingPanel/LoggingDataAggregator.h
    Editor/View/Widgets/LoggingPanel/LoggingTypes.cpp
    Editor/View/Widgets/LoggingPanel/LoggingTypes.h
    Editor/View/Widgets/LoggingPanel/LoggingWindow.cpp
    Editor/View/Widgets/LoggingPanel/LoggingWindow.h
    Editor/View/Widgets/LoggingPanel/LoggingWindow.ui
    Editor/View/Widgets/LoggingPanel/LoggingWindowSession.cpp
    Editor/View/Widgets/LoggingPanel/LoggingWindowSession.h
    Editor/View/Widgets/LoggingPanel/LoggingWindowSession.ui
    Editor/View/Widgets/LoggingPanel/LoggingWindowTreeItems.cpp
    Editor/View/Widgets/LoggingPanel/LoggingWindowTreeItems.h
    Editor/View/Widgets/LoggingPanel/LiveWindowSession/LiveLoggingDataAggregator.cpp
    Editor/View/Widgets/LoggingPanel/LiveWindowSession/LiveLoggingDataAggregator.h
    Editor/View/Widgets/LoggingPanel/LiveWindowSession/LiveLoggingWindowSession.cpp
    Editor/View/Widgets/LoggingPanel/LiveWindowSession/LiveLoggingWindowSession.h
    Editor/View/Widgets/LoggingPanel/AssetWindowSession/LoggingAssetDataAggregator.cpp
    Editor/View/Widgets/LoggingPanel/AssetWindowSession/LoggingAssetDataAggregator.h
    Editor/View/Widgets/LoggingPanel/AssetWindowSession/LoggingAssetWindowSession.cpp
    Editor/View/Widgets/LoggingPanel/AssetWindowSession/LoggingAssetWindowSession.h
    Editor/View/Widgets/LoggingPanel/PivotTree/PivotTreeWidget.cpp
    Editor/View/Widgets/LoggingPanel/PivotTree/PivotTreeWidget.h
    Editor/View/Widgets/LoggingPanel/PivotTree/PivotTreeWidget.ui
    Editor/View/Widgets/LoggingPanel/PivotTree/EntityPivotTree/EntityPivotTree.cpp
    Editor/View/Widgets/LoggingPanel/PivotTree/EntityPivotTree/EntityPivotTree.h
    Editor/View/Widgets/LoggingPanel/PivotTree/GraphPivotTree/GraphPivotTree.cpp
    Editor/View/Widgets/LoggingPanel/PivotTree/GraphPivotTree/GraphPivotTree.h
    Editor/View/Widgets/ValidationPanel/GraphValidationDockWidget.cpp
    Editor/View/Widgets/ValidationPanel/GraphValidationDockWidget.h
    Editor/View/Widgets/ValidationPanel/GraphValidationDockWidgetBus.h
    Editor/View/Widgets/ValidationPanel/GraphValidationPanel.ui
    Editor/View/Widgets/StatisticsDialog/ScriptCanvasStatisticsDialog.cpp
    Editor/View/Widgets/StatisticsDialog/ScriptCanvasStatisticsDialog.h
    Editor/View/Widgets/StatisticsDialog/ScriptCanvasStatisticsDialog.ui
    Editor/View/Widgets/StatisticsDialog/NodeUsageTreeItem.cpp
    Editor/View/Widgets/StatisticsDialog/NodeUsageTreeItem.h
    Editor/View/Widgets/VariablePanel/GraphVariablesTableView.cpp
    Editor/View/Widgets/VariablePanel/GraphVariablesTableView.h
    Editor/View/Widgets/VariablePanel/VariableDockWidget.cpp
    Editor/View/Widgets/VariablePanel/VariableDockWidget.h
    Editor/View/Widgets/VariablePanel/VariableDockWidget.ui
    Editor/View/Widgets/VariablePanel/VariablePaletteTableView.cpp
    Editor/View/Widgets/VariablePanel/VariablePaletteTableView.h
    Editor/View/Widgets/VariablePanel/SlotTypeSelectorWidget.cpp
    Editor/View/Widgets/VariablePanel/SlotTypeSelectorWidget.h
    Editor/View/Widgets/VariablePanel/SlotTypeSelectorWidget.ui    
    Editor/View/Widgets/UnitTestPanel/UnitTestDockWidget.cpp
    Editor/View/Widgets/UnitTestPanel/UnitTestDockWidget.h
    Editor/View/Widgets/UnitTestPanel/UnitTestDockWidget.ui
    Editor/View/Widgets/UnitTestPanel/UnitTestTreeView.cpp
    Editor/View/Widgets/UnitTestPanel/UnitTestTreeView.h
    Editor/View/Windows/EBusHandlerActionMenu.cpp
    Editor/View/Windows/EBusHandlerActionMenu.h
    Editor/View/Windows/ebushandleractionlistwidget.ui
    Editor/View/Windows/MainWindow.cpp
    Editor/View/Windows/MainWindow.h
    Editor/View/Windows/mainwindow.ui
    Editor/View/Windows/MainWindowBus.h
    Editor/View/Windows/ScriptCanvasContextMenus.cpp
    Editor/View/Windows/ScriptCanvasContextMenus.h
    Editor/View/Windows/ScriptCanvasEditorResources.qrc
    Editor/View/Windows/Tools/UpgradeTool/UpgradeHelper.h
    Editor/View/Windows/Tools/UpgradeTool/UpgradeHelper.cpp
    Editor/View/Windows/Tools/UpgradeTool/UpgradeHelper.ui
    Editor/View/Windows/Tools/UpgradeTool/UpgradeTool.cpp
    Editor/View/Windows/Tools/UpgradeTool/UpgradeTool.h
    Editor/View/Windows/Tools/UpgradeTool/UpgradeTool.ui
    Editor/View/Windows/Tools/UpgradeTool/VersionExplorer.h
    Editor/View/Windows/Tools/UpgradeTool/VersionExplorer.cpp
    Editor/View/Windows/Tools/UpgradeTool/VersionExplorer.ui
    Editor/Framework/ScriptCanvasGraphUtilities.inl
    Editor/Framework/ScriptCanvasGraphUtilities.h
    Editor/Framework/ScriptCanvasTraceUtilities.h
    Editor/Framework/ScriptCanvasReporter.inl
    Editor/Framework/ScriptCanvasReporter.h
)

set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Editor/ScriptCanvasEditorGem.cpp
)
