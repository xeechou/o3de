#
# Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.
# 
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    AzQtComponentsAPI.h
    Application/AzQtApplication.cpp
    Application/AzQtApplication.h
	Application/AzQtTraceLogger.cpp
	Application/AzQtTraceLogger.h
    Buses/DragAndDrop.h
    Buses/ShortcutDispatch.h
    DragAndDrop/MainWindowDragAndDrop.h
    DragAndDrop/ViewportDragAndDrop.h
    Components/AutoCustomWindowDecorations.cpp
    Components/AutoCustomWindowDecorations.h
    Components/ButtonDivider.cpp
    Components/ButtonDivider.h
    Components/ButtonStripe.cpp
    Components/ButtonStripe.h
    Components/ConfigHelpers.cpp
    Components/ConfigHelpers.h
    Components/DockBar.cpp
    Components/DockBar.h
    Components/DockBarButton.cpp
    Components/DockBarButton.h
    Components/DockBarButtonConfig.ini
    Components/DockMainWindow.cpp
    Components/DockMainWindow.h
    Components/DockTabBar.cpp
    Components/DockTabBar.h
    Components/DockTabWidget.cpp
    Components/DockTabWidget.h
    Components/FancyDocking.cpp
    Components/FancyDocking.h
    Components/FancyDockingGhostWidget.cpp
    Components/FancyDockingGhostWidget.h
    Components/FancyDockingDropZoneWidget.cpp
    Components/FancyDockingDropZoneWidget.h
    Components/FilteredSearchWidget.cpp
    Components/FilteredSearchWidget.h
    Components/FilteredSearchWidget.ui
    Components/GlobalEventFilter.h
    Components/GlobalEventFilter.cpp
    Components/O3DEStylesheet.h
    Components/Titlebar.cpp
    Components/Titlebar.h
    Components/TitleBarOverdrawHandler.cpp
    Components/TitleBarOverdrawHandler.h
    Components/ToolButtonComboBox.cpp
    Components/ToolButtonComboBox.h
    Components/ToolButtonLineEdit.cpp
    Components/ToolButtonLineEdit.h
    Components/ToolButtonWithWidget.cpp
    Components/ToolButtonWithWidget.h
    Components/StyledDockWidget.cpp
    Components/StyledDockWidget.h
    Components/StyledBusyLabel.cpp
    Components/StyledBusyLabel.h
    Components/StyledDetailsTableModel.cpp
    Components/StyledDetailsTableModel.h
    Components/StyledDetailsTableView.cpp
    Components/StyledDetailsTableView.h
    Components/HelpButton.cpp
    Components/HelpButton.h
    Components/InteractiveWindowGeometryChanger.cpp
    Components/InteractiveWindowGeometryChanger.h
    Components/SearchLineEdit.cpp
    Components/SearchLineEdit.h
    Components/StyledDialog.cpp
    Components/StyledDialog.h
    Components/StyledLineEdit.cpp
    Components/StyledLineEdit.h
    Components/StyledSpinBox.cpp
    Components/StyledSpinBox.h
    Components/Style.cpp
    Components/Style.h
    Components/StyleHelpers.h
    Components/StyleManager.cpp
    Components/StyleManager.h
    Components/StyleSheetCache.cpp
    Components/StyleSheetCache.h
    Components/StylesheetPreprocessor.cpp
    Components/StylesheetPreprocessor.h
    Components/TagSelector.cpp
    Components/TagSelector.h
    Components/ToolBarArea.cpp
    Components/ToolBarArea.h
    Components/VectorEdit.cpp
    Components/VectorEdit.h
    Components/WindowDecorationWrapper.cpp
    Components/WindowDecorationWrapper.h
    Components/ExtendedLabel.cpp
    Components/ExtendedLabel.h
    Components/RepolishMinimizer.h
    Components/FlowLayout.h
    Components/FlowLayout.cpp
    Components/resources.qrc
    Components/Widgets/BaseStyleSheet.qss
    Components/Widgets/AssetFolderListView.cpp
    Components/Widgets/AssetFolderListView.h
    Components/Widgets/AssetFolderListView.qss
    Components/Widgets/AssetFolderThumbnailView.cpp
    Components/Widgets/AssetFolderThumbnailView.h
    Components/Widgets/AssetFolderThumbnailViewConfig.ini
    Components/Widgets/BreadCrumbs.cpp
    Components/Widgets/BreadCrumbs.h
    Components/Widgets/BreadCrumbsConfig.ini
    Components/Widgets/BreadCrumbs.qss
    Components/Widgets/BrowseEdit.cpp
    Components/Widgets/BrowseEdit.h
    Components/Widgets/BrowseEdit.qss
    Components/Widgets/BrowseEditConfig.ini
    Components/Widgets/Card.cpp
    Components/Widgets/Card.h
    Components/Widgets/CardConfig.ini
    Components/Widgets/Card.qss
    Components/Widgets/CardHeader.cpp
    Components/Widgets/CardHeader.h
    Components/Widgets/CardNotification.cpp
    Components/Widgets/CardNotification.h
    Components/Widgets/CheckBox.cpp
    Components/Widgets/CheckBox.h
    Components/Widgets/CheckBox.qss
    Components/Widgets/CheckBoxConfig.ini
    Components/Widgets/ColorLabel.cpp
    Components/Widgets/ColorLabel.h
    Components/Widgets/ColorLabel.qss
    Components/Widgets/ColorPicker.cpp
    Components/Widgets/ColorPicker.h
    Components/Widgets/ColorPicker.qss
    Components/Widgets/ColorPickerConfig.ini
    Components/Widgets/ComboBox.cpp
    Components/Widgets/ComboBox.h
    Components/Widgets/ComboBox.qss
    Components/Widgets/ComboBoxConfig.ini
    Components/Widgets/DialogButtonBox.cpp
    Components/Widgets/DialogButtonBox.h
    Components/Widgets/DragAndDrop.cpp
    Components/Widgets/DragAndDrop.h
    Components/Widgets/DragAndDropConfig.ini
    Components/Widgets/ElidingLabel.cpp
    Components/Widgets/ElidingLabel.h
    Components/Widgets/Eyedropper.h
    Components/Widgets/Eyedropper.cpp
    Components/Widgets/EyedropperConfig.ini
    Components/Widgets/FilteredSearchWidget.qss
    Components/Widgets/FilteredSearchWidgetConfig.ini
    Components/Widgets/GradientSlider.cpp
    Components/Widgets/GradientSlider.h
    Components/Widgets/LineEdit.cpp
    Components/Widgets/LineEdit.h
    Components/Widgets/LineEdit.qss
    Components/Widgets/LineEditConfig.ini
    Components/Widgets/LogicalTabOrderingWidget.cpp
    Components/Widgets/LogicalTabOrderingWidget.h
    Components/Widgets/Menu.cpp
    Components/Widgets/Menu.h
    Components/Widgets/Menu.qss
    Components/Widgets/MenuConfig.ini
    Components/Widgets/MessageBox.cpp
    Components/Widgets/MessageBox.h
    Components/Widgets/OverlayWidget.cpp
    Components/Widgets/OverlayWidget.h
    Components/Widgets/Internal/OverlayWidgetLayer.cpp
    Components/Widgets/Internal/OverlayWidgetLayer.h
    Components/Widgets/Internal/OverlayWidgetLayer.ui
    Components/Widgets/ProgressBar.cpp
    Components/Widgets/ProgressBar.h
    Components/Widgets/ProgressBarConfig.ini
    Components/Widgets/ProgressBar.qss
    Components/Widgets/PushButton.cpp
    Components/Widgets/PushButton.h
    Components/Widgets/PushButtonConfig.ini
    Components/Widgets/PushButton.qss
    Components/Widgets/QDockWidget.qss
    Components/Widgets/RadioButton.cpp
    Components/Widgets/RadioButton.h
    Components/Widgets/RadioButton.qss
    Components/Widgets/RadioButtonConfig.ini
    Components/Widgets/ReflectedPropertyEditor.qss
    Components/Widgets/ScrollBar.cpp
    Components/Widgets/ScrollBar.h
    Components/Widgets/ScrollBar.qss
    Components/Widgets/ScrollBarConfig.ini
    Components/Widgets/SegmentBar.h
    Components/Widgets/SegmentBar.cpp
    Components/Widgets/SegmentControl.h
    Components/Widgets/SegmentControl.cpp
    Components/Widgets/SegmentControl.qss
    Components/Widgets/Slider.cpp
    Components/Widgets/Slider.h
    Components/Widgets/Slider.qss
    Components/Widgets/SliderConfig.ini
    Components/Widgets/SliderCombo.cpp
    Components/Widgets/SliderCombo.h
    Components/Widgets/SpinBox.cpp
    Components/Widgets/SpinBox.h
    Components/Widgets/SpinBox.qss
    Components/Widgets/SpinBoxConfig.ini
    Components/Widgets/StatusBar.cpp
    Components/Widgets/StatusBar.h
    Components/Widgets/StatusBarConfig.ini
    Components/Widgets/StyledDockWidget.qss
    Components/Widgets/TableView.cpp
    Components/Widgets/TableView.h
    Components/Widgets/TableView.qss
    Components/Widgets/TabWidget.cpp
    Components/Widgets/TabWidget.h
    Components/Widgets/TabWidget.qss
    Components/Widgets/TabWidgetConfig.ini
    Components/Widgets/TabWidgetActionToolBar.cpp
    Components/Widgets/TabWidgetActionToolBar.h
    Components/Widgets/TabWidgetActionToolBar.qss
    Components/Widgets/Text.cpp
    Components/Widgets/Text.h
    Components/Widgets/Text.qss
    Components/Widgets/TitleBar.qss
    Components/Widgets/TitleBarConfig.ini
    Components/Widgets/ToolBar.cpp
    Components/Widgets/ToolBar.h
    Components/Widgets/ToolBar.qss
    Components/Widgets/ToolBarConfig.ini
    Components/Widgets/ToolButton.cpp
    Components/Widgets/ToolButton.h
    Components/Widgets/ToolButtonConfig.ini
    Components/Widgets/ToolTip.qss
    Components/Widgets/TreeView.h
    Components/Widgets/TreeView.cpp
    Components/Widgets/TreeViewConfig.ini
    Components/Widgets/VectorInput.cpp
    Components/Widgets/VectorInput.h
    Components/Widgets/VectorInput.qss
    Components/Widgets/WindowDecorationWrapper.qss
    Components/Widgets/ColorPicker/ColorController.cpp
    Components/Widgets/ColorPicker/ColorController.h
    Components/Widgets/ColorPicker/ColorComponentSliders.cpp
    Components/Widgets/ColorPicker/ColorComponentSliders.h
    Components/Widgets/ColorPicker/ColorPreview.cpp
    Components/Widgets/ColorPicker/ColorPreview.h
    Components/Widgets/ColorPicker/ColorRGBAEdit.cpp
    Components/Widgets/ColorPicker/ColorRGBAEdit.h
    Components/Widgets/ColorPicker/ColorHexEdit.cpp
    Components/Widgets/ColorPicker/ColorHexEdit.h
    Components/Widgets/ColorPicker/ColorGrid.cpp
    Components/Widgets/ColorPicker/ColorGrid.h
    Components/Widgets/ColorPicker/ColorValidator.cpp
    Components/Widgets/ColorPicker/ColorValidator.h
    Components/Widgets/ColorPicker/PaletteView.cpp
    Components/Widgets/ColorPicker/PaletteView.h
    Components/Widgets/ColorPicker/PaletteViewConfig.ini
    Components/Widgets/ColorPicker/Palette.cpp
    Components/Widgets/ColorPicker/Palette.h
    Components/Widgets/ColorPicker/GammaEdit.cpp
    Components/Widgets/ColorPicker/GammaEdit.h
    Components/Widgets/ColorPicker/PaletteCard.cpp
    Components/Widgets/ColorPicker/PaletteCard.h
    Components/Widgets/ColorPicker/PaletteCardCollection.cpp
    Components/Widgets/ColorPicker/PaletteCardCollection.h
    Components/Widgets/ColorPicker/Swatch.h
    Components/Widgets/ColorPicker/Swatch.cpp
    Components/Widgets/ColorPicker/ColorWarning.h
    Components/Widgets/ColorPicker/ColorWarning.cpp
    Images/resources.qrc
    Utilities/AutoSettingsGroup.h
    Utilities/ColorUtilities.cpp
    Utilities/ColorUtilities.h
    Utilities/Conversions.h
    Utilities/Conversions.cpp
    Utilities/DesktopUtilities.cpp
    Utilities/DesktopUtilities.h
    Utilities/HandleDpiAwareness.cpp
    Utilities/HandleDpiAwareness.h
    Utilities/MouseHider.h
    Utilities/QtPluginPaths.cpp
    Utilities/QtPluginPaths.h
    Utilities/QtWindowUtilities.cpp
    Utilities/QtWindowUtilities.h
    Utilities/QtViewPaneEffects.h
    Utilities/QtViewPaneEffects.cpp
    Utilities/RandomNumberGenerator.cpp
    Utilities/RandomNumberGenerator.h
    Utilities/ScreenUtilities.h
    Utilities/ScreenUtilities.cpp
    Utilities/ScreenGrabber.h
    Utilities/ScopedCleanup.h
    Utilities/SelectionProxyModel.cpp
    Utilities/SelectionProxyModel.h
    Utilities/TextUtilities.cpp
    Utilities/TextUtilities.h
)
