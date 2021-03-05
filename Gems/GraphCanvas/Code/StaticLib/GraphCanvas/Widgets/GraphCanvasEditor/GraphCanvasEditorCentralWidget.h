/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

#if !defined(Q_MOC_RUN)
#include <QWidget>
#include <qdockwidget.h>
#include <qmimedata.h>

#include <AzCore/Component/EntityId.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/string/string_view.h>
#include <AzQtComponents/Components/DockMainWindow.h>

#include <GraphCanvas/Editor/EditorTypes.h>
#include <GraphCanvas/Editor/AssetEditorBus.h>

#include <GraphCanvas/Styling/StyleManager.h>
#endif

namespace Ui
{
    class GraphCanvasEditorCentralWidget;
}

namespace AzQtComponents
{
    class FancyDocking;
}

namespace GraphCanvas
{
    class EditorDockWidget;

    class GraphCanvasEditorEmptyDockWidget
        : public QDockWidget
    {
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(GraphCanvasEditorEmptyDockWidget, AZ::SystemAllocator,0);
        GraphCanvasEditorEmptyDockWidget(QWidget* parent);
        ~GraphCanvasEditorEmptyDockWidget();
        
        void SetDragTargetText(const AZStd::string& dragTargetString);

        void RegisterAcceptedMimeType(const QString& mimeType);
        
        void SetEditorId(const EditorId& editorId);
        const EditorId& GetEditorId() const;
        
        // QWidget
        void dragEnterEvent(QDragEnterEvent* enterEvent) override;
        void dragMoveEvent(QDragMoveEvent* moveEvent) override;
        void dropEvent(QDropEvent* dropEvent) override;
        ////
        
    protected:        
    private:

        bool AcceptsMimeData(const QMimeData* mimeData) const;

        QMimeData   m_initialDropMimeData;
    
        EditorId m_editorId;
        AZStd::unique_ptr<Ui::GraphCanvasEditorCentralWidget> m_ui;

        bool m_allowDrop;
        AZStd::vector< QString > m_mimeTypes;
    };

    class AssetEditorCentralDockWindow
        : public AzQtComponents::DockMainWindow
    {
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(AssetEditorCentralDockWindow, AZ::SystemAllocator, 0);

        AssetEditorCentralDockWindow(const EditorId& editorId, const char* saveIdentifier);
        ~AssetEditorCentralDockWindow();

        GraphCanvasEditorEmptyDockWidget* GetEmptyDockWidget() const;

        void OnEditorOpened(EditorDockWidget* dockWidget);
        void OnEditorClosed(EditorDockWidget* dockWidget);
        bool CloseAllEditors();
        EditorDockWidget* GetEditorDockWidgetByGraphId(const GraphId& graphId) const;
        const AZStd::unordered_set<EditorDockWidget*>& GetEditorDockWidgets() const;

        void OnFocusChanged(QWidget* oldWidget, QWidget* nowFocus);

    signals:
        void OnEditorClosing(EditorDockWidget* dockWidget);

    protected:

        void UpdateCentralWidget();
        void ActiveGraphChanged(EditorDockWidget* dockWidget);
        bool IsDockedInMainWindow(QDockWidget* dockWidget);

    protected Q_SLOTS:
        void HandleTabWidgetCurrentChanged(int index);

    private:

        EditorId m_editorId;

        GraphCanvasEditorEmptyDockWidget*    m_emptyDockWidget;
        AZStd::unordered_set< EditorDockWidget* > m_editorDockWidgets;

        AzQtComponents::FancyDocking* m_fancyDockingManager;

        void CreateTabifiedDockWidget(QDockWidget* dockWidget);
    };
}
