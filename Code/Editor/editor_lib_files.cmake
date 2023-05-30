#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Translations/editor_en-us.ts
    Translations/assetbrowser_en-us.ts
    DPIAware.xml
    res/AVI_Recorder.bmp
    res/ConsoleToolbar.bmp
    res/CryEdit.ico
    res/CryEdit.rc2
    res/CryEditDoc.ico
    res/Cursor_2.cur
    res/Mouse.cur
    res/Preferences.bmp
    res/Toolbar.bmp
    res/TreeView.bmp
    res/VisualLog_PlayerButtons.bmp
    res/ab_toolbar.bmp
    res/anim.bmp
    res/animatio.bmp
    res/animations_tree_soundevent.bmp
    res/animflag_inside_pak.bmp
    res/animflag_on_disk.bmp
    res/animtree.bmp
    res/arhitype_tree.bmp
    res/arr_addkey.cur
    res/arrow.cur
    res/arrow_down.cur
    res/arrow_down_black.ico
    res/arrow_downright.cur
    res/arrow_up.cur
    res/arrow_up_black.ico
    res/arrow_upright.cur
    res/arrowcop.cur
    res/assetIsDraggable.ico
    res/avi_reco.bmp
    res/ball_disabled.ico
    res/ball_offline.ico
    res/ball_online.ico
    res/ball_pending.ico
    res/bitmap5.bmp
    res/bitmap6.bmp
    res/bmp00001.bmp
    res/bmp00002.bmp
    res/bmp00003.bmp
    res/bmp00005.bmp
    res/bmp00006.bmp
    res/bmp00007.bmp
    res/bmp00008.bmp
    res/bmp00009.bmp
    res/bmp00010.bmp
    res/bmp00011.bmp
    res/bmp00012.bmp
    res/bmp00013.bmp
    res/bmp00014.bmp
    res/bmp00015.bmp
    res/bmp00016.bmp
    res/bmp00017.bmp
    res/bmp00019.bmp
    res/bmp00020.bmp
    res/bmp00024.bmp
    res/bmp00025.bmp
    res/bmp00026.bmp
    res/bmp00027.bmp
    res/bmp00028.bmp
    res/bmp00029.bmp
    res/bmp00030.bmp
    res/bmp00031.bmp
    res/brush24bpp.bmp
    res/ce_animations_toolbar.bmp
    res/character_parts_bar.bmp
    res/charedit.bmp
    res/clapperboard_cancel.bmp
    res/clapperboard_ready.bmp
    res/clone.ico
    res/cur00001.cur
    res/cur00002.cur
    res/cur00003.cur
    res/cur00004.cur
    res/cur00005.cur
    res/cursor1.cur
    res/cursor2.cur
    res/db_gametoken.bmp
    res/db_library_bar.bmp
    res/db_library_item_bar.bmp
    res/db_music.bmp
    res/db_music_logic.bmp
    res/db_standart.bmp
    res/down_arr.ico
    res/down_arrow.ico
    res/dynamichelp.bmp
    res/edit_mod.bmp
    res/editwithbutton.bmp
    res/editwithbutton_dark.bmp
    res/entitybar.bmp
    res/error_report.bmp
    res/expand1.ico
    res/expand2.ico
    res/faceit_playbar.bmp
    res/faceit_slidersbar.bmp
    res/faceit_spline_bar.bmp
    res/facejoystickbar.bmp
    res/facesequence_bar.bmp
    res/facialsequence_bar.bmp
    res/feedback.ico
    res/file_browse.ico
    res/filesimage.bmp
    res/filestatus.bmp
    res/folder.ico
    res/fopen_back.ico
    res/fopen_up.ico
    res/grid.ico
    res/handDrag.cur
    res/hit.cur
    res/ico00001.ico
    res/ico00002.ico
    res/icon1.ico
    res/icon_delete.ico
    res/icon_export.ico
    res/icon_import.ico
    res/icon_new.ico
    res/icon_pause.ico
    res/icon_play.ico
    res/icon_question.bmp
    res/icon_question.ico
    res/idb_.bmp
    res/layouts.bmp
    res/lc_connecting.ico
    res/lc_running.ico
    res/leftright.cur
    res/litebulb.bmp
    res/lock_sel.bmp
    res/locksele.bmp
    res/mainfram.bmp
    res/mann_tagdef_toolbar.bmp
    res/mann_tagdef_tree.bmp
    res/material.bmp
    res/material_browser.bmp
    res/materialbar.bmp
    res/maximize.ico
    res/minus.ico
    res/model_viewport_dock.bmp
    res/model_viewport_fullscreen.bmp
    res/musictree.bmp
    res/nodrop.cur
    res/object_move.cur
    res/object_rotate.cur
    res/object_scale.cur
    res/objectsbrowser.bmp
    res/panel_ve.bmp
    res/panel_veg.bmp
    res/particles_tree.bmp
    res/particlesbar.bmp
    res/pick.bmp
    res/pick_cursor.cur
    res/plus.ico
    res/pointerDragItem.cur
    res/pointerHit.cur
    res/pointer_.cur
    res/pointer_flatten.cur
    res/pointer_getheight.cur
    res/pointer_link.cur
    res/pointer_linknow.cur
    res/pointer_minus.cur
    res/pointer_plus.cur
    res/pointer_smooth.cur
    res/pointer_so_sel_plus.cur
    res/pointer_so_select.cur
    res/proceduralmaterial_toolbar.bmp
    res/progslider_end.bmp
    res/progslider_marker.bmp
    res/progslider_start.bmp
    res/progslider_thumb.bmp
    res/progslider_track.bmp
    res/properties.bmp
    res/psd.ico
    res/rename.ico
    res/replace.ico
    res/ribbon_system_button.png
    res/selectobj.bmp
    res/seq_1_colour_keys.bmp
    res/seq_2_colour_keys.bmp
    res/seq_3_colour_keys.bmp
    res/seq_4_colour_keys.bmp
    res/seq_5_colour_keys.bmp
    res/seq_6_colour_keys.bmp
    res/seq_7_colour_keys.bmp
    res/sequencer_keys.bmp
    res/sequencer_nodes.bmp
    res/soundfiles.bmp
    res/soundmood.bmp
    res/soundpre.bmp
    res/spline_edit_bar.bmp
    res/splineextrabar.bmp
    res/status_mem_low1.ico
    res/status_mem_low2.ico
    res/status_mem_ok.ico
    res/stdviews.bmp
    res/subobjseltype.bmp
    res/sw_mapbar.bmp
    res/sw_toolbar.bmp
    res/tiff.ico
    res/toolbar1.bmp
    res/toolbar2.bmp
    res/toolbox.bmp
    res/trackvie.bmp
    res/trackview.bmp
    res/trackview_key.bmp
    res/trackview_keys.bmp
    res/trackview_nodes.bmp
    res/trackview_play.bmp
    res/trackview_view.bmp
    res/tree_vie.bmp
    res/uieditor_main.bmp
    res/up_arrow.ico
    res/veed_tree.bmp
    res/vegetati.bmp
    res/vegtree.bmp
    res/video_record.ico
    res/warning16x16.ico
    res/water.bmp
    res/work_in_progress_icon.ico
    res/MannFileManagerImageList.bmp
    res/infobar/CameraCollision-default.svg
    res/infobar/GotoLocation-default.svg
    res/infobar/LockScale-default.svg
    res/infobar/LockSelection-default.svg
    res/infobar/Mute-default.svg
    res/infobar/NoPlayerSync-default.svg
    res/infobar/NoPlayerSync-selected.svg
    res/infobar/Pause-default.svg
    res/infobar/PausePlay-default.svg
    res/infobar/PhysicsCol-default.svg
    res/infobar/VR-default.svg
    res/infobar/XYZ-default.svg
    Plugin.cpp
    Plugin.h
    PluginManager.cpp
    PluginManager.h
    IEditor.h
    IEditorImpl.cpp
    IEditorImpl.h
    CryEditPy.cpp
    CryEdit.cpp
    CryEdit.h
    EditorToolsApplication.cpp
    EditorToolsApplication.h
    EditorToolsApplicationAPI.h
    Core/EditorMetricsPlainTextNameRegistration.cpp
    Core/EditorMetricsPlainTextNameRegistration.h
    MainWindow.cpp
    MainWindow.h
    MainWindow.qrc
    Core/EditorActionsHandler.cpp
    Core/EditorActionsHandler.h
    Core/QtEditorApplication.cpp
    Core/QtEditorApplication.h
    Core/Widgets/PrefabEditVisualModeWidget.h
    Core/Widgets/PrefabEditVisualModeWidget.cpp
    Core/Widgets/ViewportSettingsWidgets.h
    Core/Widgets/ViewportSettingsWidgets.cpp
    StartupTraceHandler.cpp
    StartupTraceHandler.h
    LevelTreeModel.cpp
    LevelTreeModel.h
    Include/Command.h
    Include/HitContext.h
    Include/ICommandManager.h
    Include/IDisplayViewport.h
    Include/IEditorClassFactory.h
    Include/ILogFile.h
    Include/IKeyTimeSet.h
    Include/IObjectManager.h
    Include/IPlugin.h
    Include/IPreferencesPage.h
    Include/ISourceControl.h
    Include/ObjectEvent.h
    Util/AffineParts.cpp
    Objects/BaseObject.cpp
    Objects/BaseObject.h
    AnimationContext.cpp
    AnimationContext.h
    AzAssetBrowser/AzAssetBrowserRequestHandler.cpp
    AzAssetBrowser/AzAssetBrowserRequestHandler.h
    AzAssetBrowser/AzAssetBrowserMultiWindow.cpp
    AzAssetBrowser/AzAssetBrowserMultiWindow.h
    AzAssetBrowser/AzAssetBrowserWindow.cpp
    AzAssetBrowser/AzAssetBrowserWindow.h
    AzAssetBrowser/AzAssetBrowserWindow.ui
    AssetDatabase/AssetDatabaseLocationListener.h
    AssetDatabase/AssetDatabaseLocationListener.cpp
    AssetImporter/AssetImporterManager/AssetImporterDragAndDropHandler.cpp
    AssetImporter/AssetImporterManager/AssetImporterDragAndDropHandler.h
    AssetImporter/AssetImporterManager/AssetImporterManager.cpp
    AssetImporter/AssetImporterManager/AssetImporterManager.h
    AssetImporter/UI/FilesAlreadyExistDialog.cpp
    AssetImporter/UI/FilesAlreadyExistDialog.h
    AssetImporter/UI/FilesAlreadyExistDialog.ui
    AssetImporter/UI/ProcessingAssetsDialog.cpp
    AssetImporter/UI/ProcessingAssetsDialog.h
    AssetImporter/UI/ProcessingAssetsDialog.ui
    AssetImporter/UI/SelectDestinationDialog.cpp
    AssetImporter/UI/SelectDestinationDialog.h
    AssetImporter/UI/SelectDestinationDialog.ui
    AssetEditor/AssetEditorRequestsHandler.cpp
    AssetEditor/AssetEditorRequestsHandler.h
    AssetEditor/AssetEditorWindow.cpp
    AssetEditor/AssetEditorWindow.h
    AssetEditor/AssetEditorWindow.ui
    Commands/CommandManager.cpp
    Commands/CommandManager.h
    Controls/ConsoleSCB.cpp
    Controls/ConsoleSCB.h
    Controls/ConsoleSCB.ui
    Controls/ConsoleSCB.qrc
    Controls/CvarDPE.cpp
    Controls/CvarDPE.h
    Controls/FolderTreeCtrl.cpp
    Controls/FolderTreeCtrl.h
    Controls/SplineCtrl.cpp
    Controls/SplineCtrl.h
    Controls/SplineCtrlEx.cpp
    Controls/SplineCtrlEx.h
    Controls/TimelineCtrl.cpp
    Controls/TimelineCtrl.h
    Controls/WndGridHelper.h
    Controls/ReflectedPropertyControl/PropertyGenericCtrl.cpp
    Controls/ReflectedPropertyControl/PropertyGenericCtrl.h
    Controls/ReflectedPropertyControl/PropertyMiscCtrl.cpp
    Controls/ReflectedPropertyControl/PropertyMiscCtrl.h
    Controls/ReflectedPropertyControl/PropertyMotionCtrl.cpp
    Controls/ReflectedPropertyControl/PropertyMotionCtrl.h
    Controls/ReflectedPropertyControl/PropertyCtrl.cpp
    Controls/ReflectedPropertyControl/PropertyCtrl.h
    MainStatusBar.cpp
    MainStatusBar.h
    MainStatusBarItems.h
    CryEdit.rc
    CryEditDoc.cpp
    CryEditDoc.h
    EditorDefs.h
    GameEngine.cpp
    GameEngine.h
    Include/SandboxAPI.h
    LogFile.cpp
    LogFile.h
    Resource.h
    ActionManager.cpp
    ActionManager.h
    ShortcutDispatcher.cpp
    ShortcutDispatcher.h
    CheckOutDialog.cpp
    CheckOutDialog.h
    CheckOutDialog.ui
    ConsoleDialog.cpp
    ConsoleDialog.h
    CustomAspectRatioDlg.cpp
    CustomAspectRatioDlg.ui
    CustomAspectRatioDlg.h
    CustomResolutionDlg.cpp
    CustomResolutionDlg.ui
    CustomResolutionDlg.h
    ErrorReportDialog.ui
    ErrorReportDialog.cpp
    ErrorReportDialog.h
    GenericSelectItemDialog.ui
    GenericSelectItemDialog.cpp
    GenericSelectItemDialog.h
    GotoPositionDlg.cpp
    GotoPositionDlg.h
    GotoPositionDlg.ui
    LayoutConfigDialog.cpp
    LayoutConfigDialog.h
    LayoutConfigDialog.ui
    LayoutConfigDialog.qrc
    LevelFileDialog.cpp
    LevelFileDialog.qrc
    LevelFileDialog.h
    LevelFileDialog.ui
    SelectLightAnimationDialog.cpp
    SelectLightAnimationDialog.h
    SelectSequenceDialog.cpp
    SelectSequenceDialog.h
    StartupLogoDialog.cpp
    StartupLogoDialog.h
    StartupLogoDialog.ui
    StartupLogoDialog.qrc
    ToolsConfigPage.cpp
    ToolsConfigPage.h
    ToolsConfigPage.ui
    IconListDialog.ui
    UndoDropDown.cpp
    UndoDropDown.h
    NewLevelDialog.cpp
    NewLevelDialog.h
    NewLevelDialog.ui
    Dialogs/PythonScriptsDialog.cpp
    Dialogs/PythonScriptsDialog.h
    Dialogs/PythonScriptsDialog.ui
    Dialogs/Generic/UserOptions.cpp
    Dialogs/Generic/UserOptions.h
    EditorFileMonitor.cpp
    EditorFileMonitor.h
    Include/IEditorFileMonitor.h
    GameExporter.cpp
    GameResourcesExporter.cpp
    GameExporter.h
    GameResourcesExporter.h
    DocMultiArchive.h
    FBXExporterDialog.h
    FileTypeUtils.h
    GridUtils.h
    IPostRenderer.h
    ToolBox.h
    TrackViewNewSequenceDialog.h
    Util/GeometryUtil.h
    LevelIndependentFileMan.cpp
    LevelIndependentFileMan.h
    LogFileImpl.cpp
    LogFileImpl.h
    Objects/ClassDesc.h
    Objects/DisplayContextShared.inl
    Objects/SelectionGroup.cpp
    Objects/SelectionGroup.h
    Objects/ObjectLoader.cpp
    Objects/ObjectLoader.h
    Objects/ObjectManager.cpp
    Objects/ObjectManager.h
    Objects/ObjectManagerLegacyUndo.cpp
    Objects/ObjectManagerLegacyUndo.h
    Objects/DisplayContext.cpp
    Objects/DisplayContext.h
    Objects/EntityObject.cpp
    Objects/EntityObject.h
    PythonEditorFuncs.cpp
    PythonEditorFuncs.h
    QtUI/WaitCursor.h
    QtUI/WaitCursor.cpp
    CustomizeKeyboardDialog.h
    CustomizeKeyboardDialog.cpp
    CustomizeKeyboardDialog.ui
    KeyboardCustomizationSettings.h
    KeyboardCustomizationSettings.cpp
    Settings.cpp
    SettingsManager.cpp
    SettingsManagerDialog.cpp
    PreferencesStdPages.h
    PreferencesStdPages.cpp
    EditorPreferencesPageGeneral.h
    EditorPreferencesPageGeneral.cpp
    EditorPreferencesPageFiles.h
    EditorPreferencesPageFiles.cpp
    EditorPreferencesPageViewportGeneral.h
    EditorPreferencesPageViewportGeneral.cpp
    EditorPreferencesPageViewportManipulator.h
    EditorPreferencesPageViewportManipulator.cpp
    EditorPreferencesPageViewportCamera.h
    EditorPreferencesPageViewportCamera.cpp
    EditorPreferencesPageViewportDebug.h
    EditorPreferencesPageViewportDebug.cpp
    EditorPreferencesPageExperimentalLighting.h
    EditorPreferencesPageExperimentalLighting.cpp
    EditorPreferencesPageAWS.h
    EditorPreferencesPageAWS.cpp
    EditorPreferencesDialog.h
    EditorPreferencesDialog.cpp
    EditorPreferencesDialog.ui
    EditorPreferencesTreeWidgetItem.h
    EditorPreferencesTreeWidgetItem.cpp
    EditorPreferencesTreeWidgetItemDelegate.h
    EditorPreferencesTreeWidgetItemDelegate.cpp
    Settings.h
    SettingsManager.h
    SettingsManagerDialog.h
    SettingsManagerDialog.ui
    ErrorReportTableModel.h
    ErrorReportTableModel.cpp
    FBXExporterDialog.cpp
    FBXExporterDialog.ui
    FileTypeUtils.cpp
    ToolBox.cpp
    TrackViewNewSequenceDialog.cpp
    TrackViewNewSequenceDialog.ui
    Dialogs/ErrorsDlg.cpp
    Dialogs/ErrorsDlg.h
    Dialogs/ErrorsDlg.ui
    DisplaySettings.cpp
    DisplaySettings.h
    DisplaySettingsPythonFuncs.h
    DisplaySettingsPythonFuncs.cpp
    ErrorReport.cpp
    ErrorReport.h
    ProcessInfo.h
    TrackView/AtomOutputFrameCapture.cpp
    TrackView/AtomOutputFrameCapture.h
    TrackView/TrackViewDialog.qrc
    TrackView/TrackViewDialog.cpp
    TrackView/TrackViewDialog.h
    TrackView/TrackViewDopeSheetBase.cpp
    TrackView/TrackViewDopeSheetBase.h
    TrackView/TrackViewDoubleSpinBox.cpp
    TrackView/TrackViewDoubleSpinBox.h
    TrackView/TrackViewPythonFuncs.h
    TrackView/TrackViewPythonFuncs.cpp
    TrackView/TrackViewSequenceManager.cpp
    TrackView/TrackViewSequenceManager.h
    TrackView/TrackViewUndo.cpp
    TrackView/TrackViewUndo.h
    TrackView/CommentNodeAnimator.cpp
    TrackView/DirectorNodeAnimator.cpp
    TrackView/CommentNodeAnimator.h
    TrackView/DirectorNodeAnimator.h
    TrackViewExportKeyTimeDlg.cpp
    TrackViewExportKeyTimeDlg.ui
    TrackView/SequenceBatchRenderDialog.cpp
    TrackView/TrackViewCurveEditor.cpp
    TrackView/TrackViewFindDlg.cpp
    TrackView/TrackViewKeyPropertiesDlg.cpp
    TrackView/TrackViewNodes.cpp
    TrackView/TrackViewNodes.ui
    TrackView/TrackViewSplineCtrl.cpp
    TrackView/TrackViewTimeline.cpp
    TrackView/TVCustomizeTrackColorsDialog.ui
    TrackViewFBXImportPreviewDialog.cpp
    TrackViewFBXImportPreviewDialog.ui
    TrackViewExportKeyTimeDlg.h
    TrackView/SequenceBatchRenderDialog.h
    TrackView/SequenceBatchRenderDialog.ui
    TrackView/TrackViewCurveEditor.h
    TrackView/TrackViewCurveEditor.ui
    TrackView/TrackViewFindDlg.h
    TrackView/TrackViewFindDlg.ui
    TrackView/TrackViewKeyPropertiesDlg.h
    TrackView/TrackViewNodes.h
    TrackView/TrackViewSplineCtrl.h
    TrackView/TrackViewTimeline.h
    TrackView/TVCustomizeTrackColorsDlg.cpp
    TrackView/TVCustomizeTrackColorsDlg.h
    TrackViewFBXImportPreviewDialog.h
    TrackView/KeyUIControls.h
    TrackView/2DBezierKeyUIControls.cpp
    TrackView/AssetBlendKeyUIControls.cpp
    TrackView/CaptureKeyUIControls.cpp
    TrackView/ConsoleKeyUIControls.cpp
    TrackView/EventKeyUIControls.cpp
    TrackView/GotoKeyUIControls.cpp
    TrackView/SelectKeyUIControls.cpp
    TrackView/SequenceKeyUIControls.cpp
    TrackView/SoundKeyUIControls.cpp
    TrackView/TrackEventKeyUIControls.cpp
    TrackView/TrackViewTrackPropsDlg.ui
    TrackView/TVEventsDialog.cpp
    TrackView/TVEventsDialog.ui
    TrackView/TVSequenceProps.cpp
    TrackView/CommentKeyUIControls.cpp
    TrackView/ScreenFaderKeyUIControls.cpp
    TrackView/TimeRangeKeyUIControls.cpp
    TrackView/TVEventsDialog.h
    TrackView/TVSequenceProps.h
    TrackView/TVSequenceProps.ui
    TrackView/TrackViewAnimNode.cpp
    TrackView/TrackViewTrack.cpp
    TrackView/TrackViewNode.cpp
    TrackView/TrackViewSequence.cpp
    TrackView/TrackViewNodeFactories.cpp
    TrackView/TrackViewEventNode.cpp
    TrackView/TrackViewAnimNode.h
    TrackView/TrackViewTrack.h
    TrackView/TrackViewNode.h
    TrackView/TrackViewSequence.h
    TrackView/TrackViewNodeFactories.h
    TrackView/TrackViewEventNode.h
    Util/AffineParts.h
    Util/AutoLogTime.cpp
    Util/AutoLogTime.h
    Util/AutoDirectoryRestoreFileDialog.h
    Util/AutoDirectoryRestoreFileDialog.cpp
    Util/CryMemFile.h
    Util/EditorAutoLevelLoadTest.cpp
    Util/EditorAutoLevelLoadTest.h
    Util/EditorUtils.cpp
    Util/EditorUtils.h
    Util/FileEnum.cpp
    Util/FileEnum.h
    Util/FileUtil.cpp
    Util/FileUtil.h
    Util/FileUtil_Common.h
    Util/FileUtil_Common.cpp
    Util/GeometryUtil.cpp
    Util/GuidUtil.cpp
    Util/GuidUtil.h
    Util/Mailer.h
    Util/NamedData.cpp
    Util/NamedData.h
    Util/PakFile.cpp
    Util/PakFile.h
    Util/PredefinedAspectRatios.cpp
    Util/PredefinedAspectRatios.h
    Util/StringHelpers.cpp
    Util/StringHelpers.h
    Util/Util.h
    Util/XmlArchive.cpp
    Util/XmlArchive.h
    Util/XmlTemplate.cpp
    Util/XmlTemplate.h
    Util/fastlib.h
    WaitProgress.cpp
    WaitProgress.h
    Util/FileUtil_impl.h
    Util/FileUtil_impl.cpp
    Util/ImageUtil_impl.h
    Util/ImageUtil_impl.cpp
    Util/ColumnSortProxyModel.h
    Util/ColumnSortProxyModel.cpp
    Util/AbstractGroupProxyModel.h
    Util/AbstractGroupProxyModel.cpp
    Util/AbstractSortModel.h
    Util/AbstractSortModel.cpp
    Util/ColumnGroupProxyModel.h
    Util/ColumnGroupProxyModel.cpp
    Util/ColumnGroupHeaderView.h
    Util/ColumnGroupHeaderView.cpp
    Util/ColumnGroupItemDelegate.h
    Util/ColumnGroupItemDelegate.cpp
    Util/ColumnGroupTreeView.h
    Util/ColumnGroupTreeView.cpp
    Util/ModalWindowDismisser.h
    Util/ModalWindowDismisser.cpp
    Util/3DConnexionDriver.cpp
    Util/3DConnexionDriver.h
    Util/FileChangeMonitor.cpp
    Util/FileChangeMonitor.h
    Util/ImageUtil.cpp
    Util/ImageUtil.h
    Util/ImageGif.cpp
    Util/ImageGif.h
    Util/ImageTIF.cpp
    Util/ImageTIF.h
    Util/Math.h
    WelcomeScreen/WelcomeScreenDialog.h
    WelcomeScreen/WelcomeScreenDialog.cpp
    WelcomeScreen/WelcomeScreenDialog.ui
    WelcomeScreen/WelcomeScreenDialog.qrc
    LayoutWnd.cpp
    LayoutWnd.h
    EditorViewportWidget.cpp
    EditorViewportWidget.h
    EditorViewportSettings.cpp
    EditorViewportSettings.h
    EditorViewportCamera.cpp
    EditorViewportCamera.h
    EditorModularViewportCameraComposer.cpp
    EditorModularViewportCameraComposer.h
    EditorModularViewportCameraComposerBus.h
    ViewportManipulatorController.cpp
    ViewportManipulatorController.h
    ViewManager.cpp
    ViewManager.h
    ViewPane.cpp
    ViewPane.h
    Viewport.cpp
    Viewport.h
    ViewportTitleDlg.cpp
    ViewportTitleDlg.ui
    ViewportTitleDlg.h
    EditorEnvironment.cpp
    EditorEnvironment.h
)


set(SKIP_UNITY_BUILD_INCLUSION_FILES
    TrackView/TrackViewPythonFuncs.cpp
    Util/AffineParts.cpp
)
