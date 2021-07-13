#
# Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.
# 
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Editor/LyShineEditorSystemComponent.cpp
    Editor/LyShineEditorSystemComponent.h
    Editor/UiCanvasEditor.qrc
    Editor/Animation/UiAnimViewDialog.cpp
    Editor/Animation/UiAnimViewDialog.h
    Editor/Animation/UiAnimViewDialog.qrc
    Editor/Animation/UiAVCustomizeTrackColorsDialog.ui
    Editor/Animation/UiAVCustomizeTrackColorsDlg.cpp
    Editor/Animation/UiAVCustomizeTrackColorsDlg.h
    Editor/AlignToolbarSection.cpp
    Editor/AlignToolbarSection.h
    Editor/AnchorPresets.cpp
    Editor/AnchorPresets.h
    Editor/AnchorPresetsWidget.cpp
    Editor/AnchorPresetsWidget.h
    Editor/AssetDropHelpers.cpp
    Editor/AssetDropHelpers.h
    Editor/AssetTreeEntry.cpp
    Editor/AssetTreeEntry.h
    Editor/CanvasHelpers.cpp
    Editor/CanvasHelpers.h
    Editor/CanvasSizeToolbarSection.cpp
    Editor/CanvasSizeToolbarSection.h
    Editor/CommandCanvasPropertiesChange.cpp
    Editor/CommandCanvasPropertiesChange.h
    Editor/CommandCanvasSize.cpp
    Editor/CommandCanvasSize.h
    Editor/CommandCanvasSizeToolbarIndex.cpp
    Editor/CommandCanvasSizeToolbarIndex.h
    Editor/CommandHierarchyItemCreate.cpp
    Editor/CommandHierarchyItemCreate.h
    Editor/CommandHierarchyItemCreateFromData.cpp
    Editor/CommandHierarchyItemCreateFromData.h
    Editor/CommandHierarchyItemDelete.cpp
    Editor/CommandHierarchyItemDelete.h
    Editor/CommandHierarchyItemRename.cpp
    Editor/CommandHierarchyItemRename.h
    Editor/CommandHierarchyItemReparent.cpp
    Editor/CommandHierarchyItemReparent.h
    Editor/CommandHierarchyItemToggleIsExpanded.cpp
    Editor/CommandHierarchyItemToggleIsExpanded.h
    Editor/CommandHierarchyItemToggleIsSelectable.cpp
    Editor/CommandHierarchyItemToggleIsSelectable.h
    Editor/CommandHierarchyItemToggleIsSelected.cpp
    Editor/CommandHierarchyItemToggleIsSelected.h
    Editor/CommandHierarchyItemToggleIsVisible.cpp
    Editor/CommandHierarchyItemToggleIsVisible.h
    Editor/CommandPropertiesChange.cpp
    Editor/CommandPropertiesChange.h
    Editor/CommandViewportInteractionMode.cpp
    Editor/CommandViewportInteractionMode.h
    Editor/ComponentAssetHelpers.h
    Editor/ComponentButton.cpp
    Editor/ComponentButton.h
    Editor/ComponentHelpers.cpp
    Editor/ComponentHelpers.h
    Editor/CoordinateSystemToolbarSection.cpp
    Editor/CoordinateSystemToolbarSection.h
    Editor/EditorCommon.cpp
    Editor/EditorCommon.h
    Editor/EditorMenu.cpp
    Editor/EditorWindow.cpp
    Editor/EditorWindow.h
    Editor/EnterPreviewToolbar.cpp
    Editor/EnterPreviewToolbar.h
    Editor/EntityHelpers.cpp
    Editor/EntityHelpers.h
    Editor/FeedbackDialog.cpp
    Editor/FeedbackDialog.h
    Editor/FileHelpers.cpp
    Editor/FileHelpers.h
    Editor/FindEntityItemModel.cpp
    Editor/FindEntityItemModel.h
    Editor/FindEntitySortFilterProxyModel.cpp
    Editor/FindEntitySortFilterProxyModel.h
    Editor/FindEntityWidget.cpp
    Editor/FindEntityWidget.h
    Editor/GuideHelpers.cpp
    Editor/GuideHelpers.h
    Editor/HierarchyClipboard.cpp
    Editor/HierarchyClipboard.h
    Editor/HierarchyHeader.cpp
    Editor/HierarchyHeader.h
    Editor/HierarchyHelpers.cpp
    Editor/HierarchyHelpers.h
    Editor/HierarchyItem.cpp
    Editor/HierarchyItem.h
    Editor/HierarchyMenu.cpp
    Editor/HierarchyMenu.h
    Editor/HierarchyWidget.cpp
    Editor/HierarchyWidget.h
    Editor/MainToolbar.cpp
    Editor/MainToolbar.h
    Editor/ModeToolbar.cpp
    Editor/ModeToolbar.h
    Editor/NewElementToolbarSection.cpp
    Editor/NewElementToolbarSection.h
    Editor/PivotPresets.cpp
    Editor/PivotPresets.h
    Editor/PivotPresetsWidget.cpp
    Editor/PivotPresetsWidget.h
    Editor/PresetButton.cpp
    Editor/PresetButton.h
    Editor/PreviewActionLog.cpp
    Editor/PreviewActionLog.h
    Editor/PreviewAnimationList.cpp
    Editor/PreviewAnimationList.h
    Editor/PreviewToolbar.cpp
    Editor/PreviewToolbar.h
    Editor/PropertiesContainer.cpp
    Editor/PropertiesContainer.h
    Editor/PropertiesWidget.cpp
    Editor/PropertiesWidget.h
    Editor/PropertiesWrapper.cpp
    Editor/PropertiesWrapper.h
    Editor/PropertyHandlerAnchor.cpp
    Editor/PropertyHandlerAnchor.h
    Editor/PropertyHandlerChar.cpp
    Editor/PropertyHandlerChar.h
    Editor/PropertyHandlerDirectory.cpp
    Editor/PropertyHandlerDirectory.h
    Editor/PropertyHandlerEntityIdComboBox.cpp
    Editor/PropertyHandlerEntityIdComboBox.h
    Editor/PropertyHandlerLayoutPadding.cpp
    Editor/PropertyHandlerLayoutPadding.h
    Editor/PropertyHandlerOffset.cpp
    Editor/PropertyHandlerOffset.h
    Editor/PropertyHandlerUiParticleColorKeyframe.cpp
    Editor/PropertyHandlerUiParticleColorKeyframe.h
    Editor/PropertyHandlerUiParticleFloatKeyframe.cpp
    Editor/PropertyHandlerUiParticleFloatKeyframe.h
    Editor/PropertyHandlerPivot.cpp
    Editor/PropertyHandlerPivot.h
    Editor/PropertyHandlers.cpp
    Editor/PropertyHandlers.h
    Editor/PropertyHandlerSprite.cpp
    Editor/PropertyHandlerSprite.h
    Editor/PropertyHandlerVec.cpp
    Editor/PropertyHandlerVec.h
    Editor/QtHelpers.cpp
    Editor/QtHelpers.h
    Editor/RecentFiles.cpp
    Editor/RecentFiles.h
    Editor/RulerWidget.cpp
    Editor/RulerWidget.h
    Editor/SelectionHelpers.cpp
    Editor/SelectionHelpers.h
    Editor/SerializeHelpers.cpp
    Editor/SerializeHelpers.h
    Editor/SliceMenuHelpers.cpp
    Editor/SliceMenuHelpers.h
    Editor/SlicerEdit.cpp
    Editor/SlicerEdit.h
    Editor/SlicerManipulator.cpp
    Editor/SlicerManipulator.h
    Editor/SlicerView.cpp
    Editor/SlicerView.h
    Editor/SpriteBorderEditor.cpp
    Editor/SpriteBorderEditor.h
    Editor/SpriteBorderEditorCommon.cpp
    Editor/SpriteBorderEditorCommon.h
    Editor/UiEditorInternalBus.h
    Editor/UiEditorEntityContext.cpp
    Editor/UiEditorEntityContext.h
    Editor/UiEditorEntityContextBus.h
    Editor/UiSliceManager.cpp
    Editor/UiSliceManager.h
    Editor/UIVectorPropertyHandlerBase.h
    Editor/UndoStack.cpp
    Editor/UndoStack.h
    Editor/UndoStackExecutionScope.cpp
    Editor/UndoStackExecutionScope.h
    Editor/ViewportAddGuideInteraction.cpp
    Editor/ViewportAddGuideInteraction.h
    Editor/ViewportAlign.cpp
    Editor/ViewportAlign.h
    Editor/ViewportAnchor.cpp
    Editor/ViewportAnchor.h
    Editor/ViewportCanvasBackground.cpp
    Editor/ViewportCanvasBackground.h
    Editor/ViewportDragInteraction.cpp
    Editor/ViewportDragInteraction.h
    Editor/ViewportElement.cpp
    Editor/ViewportElement.h
    Editor/ViewportHelpers.cpp
    Editor/ViewportHelpers.h
    Editor/ViewportHighlight.cpp
    Editor/ViewportHighlight.h
    Editor/ViewportIcon.cpp
    Editor/ViewportIcon.h
    Editor/ViewportInteraction.cpp
    Editor/ViewportInteraction.h
    Editor/ViewportMoveGuideInteraction.cpp
    Editor/ViewportMoveGuideInteraction.h
    Editor/ViewportMoveInteraction.cpp
    Editor/ViewportMoveInteraction.h
    Editor/ViewportNudge.cpp
    Editor/ViewportNudge.h
    Editor/ViewportPivot.cpp
    Editor/ViewportPivot.h
    Editor/ViewportSnap.cpp
    Editor/ViewportSnap.h
    Editor/ViewportWidget.cpp
    Editor/ViewportWidget.h
    Editor/Animation/AnimationContext.cpp
    Editor/Animation/AnimationContext.h
    Editor/Animation/UiAnimUndo.cpp
    Editor/Animation/UiAnimUndo.h
    Editor/Animation/UiAnimUndoManager.cpp
    Editor/Animation/UiAnimUndoManager.h
    Editor/Animation/UiAnimUndoObject.h
    Editor/Animation/UiAnimViewAnimNode.cpp
    Editor/Animation/UiAnimViewAnimNode.h
    Editor/Animation/UiAnimViewEventNode.cpp
    Editor/Animation/UiAnimViewEventNode.h
    Editor/Animation/UiAnimViewNode.cpp
    Editor/Animation/UiAnimViewNode.h
    Editor/Animation/UiAnimViewNodeFactories.cpp
    Editor/Animation/UiAnimViewNodeFactories.h
    Editor/Animation/UiAnimViewSequence.cpp
    Editor/Animation/UiAnimViewSequence.h
    Editor/Animation/UiAnimViewSequenceManager.cpp
    Editor/Animation/UiAnimViewSequenceManager.h
    Editor/Animation/UiAnimViewTrack.cpp
    Editor/Animation/UiAnimViewTrack.h
    Editor/Animation/UiAnimViewUndo.cpp
    Editor/Animation/UiAnimViewUndo.h
    Editor/Animation/Controls/UiSplineCtrlEx.cpp
    Editor/Animation/Controls/UiSplineCtrlEx.h
    Editor/Animation/Controls/UiTimelineCtrl.cpp
    Editor/Animation/Controls/UiTimelineCtrl.h
    Editor/Animation/Util/UiEditorUtils.cpp
    Editor/Animation/UiAnimViewCurveEditor.cpp
    Editor/Animation/UiAnimViewCurveEditor.h
    Editor/Animation/UiAnimViewCurveEditor.ui
    Editor/Animation/UiAnimViewDopeSheetBase.cpp
    Editor/Animation/UiAnimViewDopeSheetBase.h
    Editor/Animation/UiAnimViewFindDlg.cpp
    Editor/Animation/UiAnimViewFindDlg.h
    Editor/Animation/UiAnimViewFindDlg.ui
    Editor/Animation/UiAnimViewKeyPropertiesDlg.cpp
    Editor/Animation/UiAnimViewKeyPropertiesDlg.h
    Editor/Animation/UiAnimViewNewSequenceDialog.cpp
    Editor/Animation/UiAnimViewNewSequenceDialog.h
    Editor/Animation/UiAnimViewNewSequenceDialog.ui
    Editor/Animation/UiAnimViewNodes.cpp
    Editor/Animation/UiAnimViewNodes.h
    Editor/Animation/UiAnimViewNodes.ui
    Editor/Animation/UiAnimViewSplineCtrl.cpp
    Editor/Animation/UiAnimViewSplineCtrl.h
    Editor/Animation/UiAnimViewTrackPropsDlg.ui
    Editor/Animation/UiAVEventsDialog.cpp
    Editor/Animation/UiAVEventsDialog.h
    Editor/Animation/UiAVEventsDialog.ui
    Editor/Animation/UiAVSequenceProps.cpp
    Editor/Animation/UiAVSequenceProps.h
    Editor/Animation/UiAVSequenceProps.ui
    Editor/Animation/UiAVTrackEventKeyUIControls.cpp
    Editor/Animation/UiAVTrackEventKeyUIControls.h
)
