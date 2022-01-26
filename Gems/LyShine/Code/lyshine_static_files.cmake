#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Include/LyShine/IDraw2d.h
    Include/LyShine/IRenderGraph.h
    Include/LyShine/ISprite.h
    Include/LyShine/ILyShine.h
    Include/LyShine/UiAssetTypes.h
    Include/LyShine/UiBase.h
    Include/LyShine/UiLayoutCellBase.h
    Include/LyShine/UiSerializeHelpers.h
    Include/LyShine/UiComponentTypes.h
    Include/LyShine/UiEntityContext.h
    Include/LyShine/UiEditorDLLBus.h
    Include/LyShine/UiRenderFormats.h
    Include/LyShine/Animation/IUiAnimation.h
    Include/LyShine/Bus/UiAnimationBus.h
    Include/LyShine/Bus/UiAnimateEntityBus.h
    Include/LyShine/Bus/UiButtonBus.h
    Include/LyShine/Bus/UiCanvasBus.h
    Include/LyShine/Bus/UiCanvasManagerBus.h
    Include/LyShine/Bus/UiCanvasUpdateNotificationBus.h
    Include/LyShine/Bus/UiCheckboxBus.h
    Include/LyShine/Bus/UiCursorBus.h
    Include/LyShine/Bus/UiDraggableBus.h
    Include/LyShine/Bus/UiDropdownBus.h
    Include/LyShine/Bus/UiDropdownOptionBus.h
    Include/LyShine/Bus/UiDropTargetBus.h
    Include/LyShine/Bus/UiDynamicLayoutBus.h
    Include/LyShine/Bus/UiDynamicScrollBoxBus.h
    Include/LyShine/Bus/UiEditorBus.h
    Include/LyShine/Bus/UiEditorCanvasBus.h
    Include/LyShine/Bus/UiEditorChangeNotificationBus.h
    Include/LyShine/Bus/UiElementBus.h
    Include/LyShine/Bus/UiEntityContextBus.h
    Include/LyShine/Bus/UiFaderBus.h
    Include/LyShine/Bus/UiFlipbookAnimationBus.h
    Include/LyShine/Bus/UiGameEntityContextBus.h
    Include/LyShine/Bus/UiImageBus.h
    Include/LyShine/Bus/UiImageSequenceBus.h
    Include/LyShine/Bus/UiIndexableImageBus.h
    Include/LyShine/Bus/UiInitializationBus.h
    Include/LyShine/Bus/UiInteractableActionsBus.h
    Include/LyShine/Bus/UiInteractableBus.h
    Include/LyShine/Bus/UiInteractableStatesBus.h
    Include/LyShine/Bus/UiInteractionMaskBus.h
    Include/LyShine/Bus/UiLayoutBus.h
    Include/LyShine/Bus/UiLayoutCellBus.h
    Include/LyShine/Bus/UiLayoutCellDefaultBus.h
    Include/LyShine/Bus/UiLayoutColumnBus.h
    Include/LyShine/Bus/UiLayoutControllerBus.h
    Include/LyShine/Bus/UiLayoutFitterBus.h
    Include/LyShine/Bus/UiLayoutGridBus.h
    Include/LyShine/Bus/UiLayoutManagerBus.h
    Include/LyShine/Bus/UiLayoutRowBus.h
    Include/LyShine/Bus/UiMarkupButtonBus.h
    Include/LyShine/Bus/UiMaskBus.h
    Include/LyShine/Bus/UiNavigationBus.h
    Include/LyShine/Bus/UiParticleEmitterBus.h
    Include/LyShine/Bus/UiRadioButtonBus.h
    Include/LyShine/Bus/UiRadioButtonCommunicationBus.h
    Include/LyShine/Bus/UiRadioButtonGroupBus.h
    Include/LyShine/Bus/UiRadioButtonGroupCommunicationBus.h
    Include/LyShine/Bus/UiRenderBus.h
    Include/LyShine/Bus/UiRenderControlBus.h
    Include/LyShine/Bus/UiScrollableBus.h
    Include/LyShine/Bus/UiScrollBarBus.h
    Include/LyShine/Bus/UiScrollBoxBus.h
    Include/LyShine/Bus/UiScrollerBus.h
    Include/LyShine/Bus/UiSliderBus.h
    Include/LyShine/Bus/UiSpawnerBus.h
    Include/LyShine/Bus/UiSystemBus.h
    Include/LyShine/Bus/UiTextBus.h
    Include/LyShine/Bus/UiTextInputBus.h
    Include/LyShine/Bus/UiTooltipBus.h
    Include/LyShine/Bus/UiTooltipDataPopulatorBus.h
    Include/LyShine/Bus/UiTooltipDisplayBus.h
    Include/LyShine/Bus/UiTransform2dBus.h
    Include/LyShine/Bus/UiTransformBus.h
    Include/LyShine/Bus/UiVisualBus.h
    Include/LyShine/Bus/Sprite/UiSpriteBus.h
    Include/LyShine/Bus/World/UiCanvasOnMeshBus.h
    Include/LyShine/Bus/World/UiCanvasRefBus.h
    Include/LyShine/Bus/Tools/UiSystemToolsBus.h
    Source/Draw2d.cpp
    Source/Draw2d.h
    Source/LyShine.cpp
    Source/LyShine.h
    Source/LyShinePassDataBus.h
    Source/LyShineDebug.cpp
    Source/LyShineDebug.h
    Source/LyShinePass.cpp
    Source/LyShinePass.h
    Source/StringUtfUtils.h
    Source/UiImageComponent.cpp
    Source/UiImageComponent.h
    Source/UiImageSequenceComponent.cpp
    Source/UiImageSequenceComponent.h
    Source/UiRenderer.cpp
    Source/UiRenderer.h
    Include/LyShine/LyShineBus.h
    Source/EditorPropertyTypes.cpp
    Source/EditorPropertyTypes.h
    Source/LyShineLoadScreen.cpp
    Source/LyShineLoadScreen.h
    Source/RenderGraph.cpp
    Source/RenderGraph.h
    Source/RenderToTextureBus.h
    Source/TextMarkup.cpp
    Source/TextMarkup.h
    Source/UiButtonComponent.cpp
    Source/UiButtonComponent.h
    Source/UiCanvasComponent.cpp
    Source/UiCanvasComponent.h
    Source/UiCanvasFileObject.cpp
    Source/UiCanvasFileObject.h
    Source/UiCanvasManager.cpp
    Source/UiCanvasManager.h
    Source/UiCheckboxComponent.cpp
    Source/UiCheckboxComponent.h
    Source/UiDraggableComponent.cpp
    Source/UiDraggableComponent.h
    Source/UiDropTargetComponent.cpp
    Source/UiDropTargetComponent.h
    Source/UiDropdownComponent.cpp
    Source/UiDropdownComponent.h
    Source/UiDropdownOptionComponent.cpp
    Source/UiDropdownOptionComponent.h
    Source/UiDynamicLayoutComponent.cpp
    Source/UiDynamicLayoutComponent.h
    Source/UiDynamicScrollBoxComponent.cpp
    Source/UiDynamicScrollBoxComponent.h
    Source/UiElementComponent.cpp
    Source/UiElementComponent.h
    Source/UiFaderComponent.cpp
    Source/UiFaderComponent.h
    Source/UiFlipbookAnimationComponent.cpp
    Source/UiFlipbookAnimationComponent.h
    Source/UiGameEntityContext.cpp
    Source/UiGameEntityContext.h
    Source/UiInteractableComponent.cpp
    Source/UiInteractableComponent.h
    Source/UiInteractableState.cpp
    Source/UiInteractableState.h
    Source/UiLayoutCellComponent.cpp
    Source/UiLayoutCellComponent.h
    Source/UiLayoutColumnComponent.cpp
    Source/UiLayoutColumnComponent.h
    Source/UiLayoutFitterComponent.cpp
    Source/UiLayoutFitterComponent.h
    Source/UiLayoutGridComponent.cpp
    Source/UiLayoutGridComponent.h
    Source/UiLayoutHelpers.cpp
    Source/UiLayoutHelpers.h
    Source/UiLayoutManager.cpp
    Source/UiLayoutManager.h
    Source/UiLayoutRowComponent.cpp
    Source/UiLayoutRowComponent.h
    Source/UiMarkupButtonComponent.cpp
    Source/UiMarkupButtonComponent.h
    Source/UiMaskComponent.cpp
    Source/UiMaskComponent.h
    Source/UiNavigationHelpers.cpp
    Source/UiNavigationHelpers.h
    Source/UiNavigationSettings.cpp
    Source/UiNavigationSettings.h
    Source/UiParticleEmitterComponent.cpp
    Source/UiParticleEmitterComponent.h
    Source/UiRadioButtonComponent.cpp
    Source/UiRadioButtonComponent.h
    Source/UiRadioButtonGroupComponent.cpp
    Source/UiRadioButtonGroupComponent.h
    Source/UiScrollBarComponent.cpp
    Source/UiScrollBarComponent.h
    Source/UiScrollBoxComponent.cpp
    Source/UiScrollBoxComponent.h
    Source/UiSerialize.cpp
    Source/UiSerialize.h
    Source/UiSliderComponent.cpp
    Source/UiSliderComponent.h
    Source/UiStateActionManager.cpp
    Source/UiStateActionManager.h
    Source/UiSpawnerComponent.cpp
    Source/UiSpawnerComponent.h
    Source/UiTextComponent.cpp
    Source/UiTextComponent.h
    Source/UiTextComponentOffsetsSelector.cpp
    Source/UiTextComponentOffsetsSelector.h
    Source/UiTextInputComponent.cpp
    Source/UiTextInputComponent.h
    Source/UiTooltipComponent.cpp
    Source/UiTooltipComponent.h
    Source/UiTooltipDisplayComponent.cpp
    Source/UiTooltipDisplayComponent.h
    Source/UiTransform2dComponent.cpp
    Source/UiTransform2dComponent.h
    Source/UiClipboard.h
    Source/Sprite.cpp
    Source/Sprite.h
    Source/Animation/2DSpline.h
    Source/Animation/AnimNode.cpp
    Source/Animation/AnimNode.h
    Source/Animation/AnimSequence.cpp
    Source/Animation/AnimSequence.h
    Source/Animation/AnimSplineTrack.h
    Source/Animation/AnimSplineTrack_Vec2Specialization.h
    Source/Animation/AnimTrack.h
    Source/Animation/AzEntityNode.cpp
    Source/Animation/AzEntityNode.h
    Source/Animation/BoolTrack.cpp
    Source/Animation/BoolTrack.h
    Source/Animation/CompoundSplineTrack.cpp
    Source/Animation/CompoundSplineTrack.h
    Source/Animation/EventNode.cpp
    Source/Animation/EventNode.h
    Source/Animation/TrackEventTrack.cpp
    Source/Animation/TrackEventTrack.h
    Source/Animation/UiAnimationSystem.cpp
    Source/Animation/UiAnimationSystem.h
    Source/Animation/UiAnimSerialize.cpp
    Source/Animation/UiAnimSerialize.h
    Source/Particle/UiParticle.cpp
    Source/Particle/UiParticle.h
    Source/Script/UiCanvasNotificationLuaBus.h
    Source/Script/UiCanvasNotificationLuaBus.cpp
    Source/Script/UiCanvasLuaBus.h
    Source/Script/UiCanvasLuaBus.cpp
    Source/Script/UiElementLuaBus.h
    Source/Script/UiElementLuaBus.cpp
    Source/World/UiCanvasOnMeshComponent.cpp
    Source/World/UiCanvasOnMeshComponent.h
    Source/World/UiCanvasAssetRefComponent.cpp
    Source/World/UiCanvasAssetRefComponent.h
    Source/World/UiCanvasProxyRefComponent.cpp
    Source/World/UiCanvasProxyRefComponent.h
)

set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Source/UiImageSequenceComponent.h
    Source/UiImageSequenceComponent.cpp
    Source/UiTextComponent.cpp
    Source/UiTextComponent.h
)

