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
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#pragma once

#if !defined(Q_MOC_RUN)
#include "UiAnimViewNode.h"
#include "UiAnimViewSequence.h"
#include "Undo/IUndoManagerListener.h"

#include <LyShine/Animation/IUiAnimation.h>
#include <QMap>
#include <QTreeWidgetItem>
#endif

// forward declarations.
class CUiAnimViewNode;
class CUiAnimViewAnimNode;
class CUiAnimViewTrack;
class CUiAnimViewSequence;
class CUiAnimViewDopeSheetBase;
class CUiAnimViewDialog;

class QLineEdit;


namespace Ui {
    class CUiAnimViewNodesCtrl;
}

//////////////////////////////////////////////////////////////////////////
class CUiAnimViewNodesCtrl
    : public QWidget
    , public IUiAnimViewSequenceListener
    , public IUndoManagerListener
{
    Q_OBJECT
public:
    class CRecord
        : public QTreeWidgetItem
    {
        friend class CUiAnimViewNodesCtrl;

    public:
        enum Roles
        {
            EnableRole = Qt::UserRole + 1
        };

        CRecord(CUiAnimViewNode* pNode = nullptr);
        CUiAnimViewNode* GetNode() const { return m_pNode; }
        bool IsGroup() const { return m_pNode->GetChildCount() != 0; }
        const QString GetName() const { return m_pNode->GetName(); }

        // Workaround: CXTPReportRecord::IsVisible is
        // unreliable after the last visible element
        bool IsVisible() const { return m_bVisible; }

        QRect GetRect() const { return treeWidget()->visualItemRect(this);  }

    private:
        bool m_bVisible;
        CUiAnimViewNode* m_pNode;
    };

    CUiAnimViewNodesCtrl(QWidget* hParentWnd, CUiAnimViewDialog* parent = 0);
    ~CUiAnimViewNodesCtrl();

    void SetUiAnimViewDialog(CUiAnimViewDialog* dlg) { m_pUiAnimViewDialog = dlg; }
    void OnSequenceChanged();

    void SetDopeSheet(CUiAnimViewDopeSheetBase* keysCtrl);

    void SetEditLock(bool bLock) { m_bEditLock = bLock; }

    float SaveVerticalScrollPos() const;
    void RestoreVerticalScrollPos(float fScrollPos);

    CRecord* GetNodeRecord(const CUiAnimViewNode* pNode) const;

    virtual void Reload();
    virtual void OnFillItems();

    void UpdateAllNodesForElementChanges();

    // IUiAnimViewSequenceListener
    virtual void OnNodeChanged(CUiAnimViewNode* pNode, IUiAnimViewSequenceListener::ENodeChangeType type) override;
    virtual void OnNodeRenamed(CUiAnimViewNode* pNode, const char* pOldName) override;
    virtual void OnNodeSelectionChanged(CUiAnimViewSequence* pSequence) override;
    virtual void OnKeysChanged(CUiAnimViewSequence* pSequence) override;
    virtual void OnKeySelectionChanged(CUiAnimViewSequence* pSequence) override;

    // IUndoManagerListener
    virtual void BeginUndoTransaction() override;
    virtual void EndUndoTransaction() override;

    // Helper for dialog
    QIcon GetIconForTrack(const CUiAnimViewTrack* pTrack);
    void ShowNextResult();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* o, QEvent* e) override;

private slots:
    void OnNMRclick(QPoint pos);
    void OnItemExpanded(QTreeWidgetItem*);
    void OnSelectionChanged();
    void OnItemDblClick(QTreeWidgetItem* item, int);
    void OnFilterChange(const QString& text);

private:
    void AddMenuSeperatorConditional(QMenu& menu, bool& bAppended);
    void AddGroupNodeAddItems(struct UiAnimContextMenu& contextMenu, CUiAnimViewAnimNode* pAnimNode);
    int ShowPopupMenuSingleSelection(struct UiAnimContextMenu& contextMenu, CUiAnimViewSequence* pSequence, CUiAnimViewNode* pNode);
    int ShowPopupMenuMultiSelection(struct UiAnimContextMenu& contextMenu);
    int ShowPopupMenu(QPoint point, const CRecord* pItemInfo);
    void EditEvents();

    void SetPopupMenuLock(QMenu* menu);
    void CreateSetAnimationLayerPopupMenu(QMenu& menuSetLayer, CUiAnimViewTrack* pTrack) const;

    int GetIconIndexForTrack(const CUiAnimViewTrack* pTrack) const;
    int GetIconIndexForNode(EUiAnimNodeType type) const;

    void AddNodeRecord(CRecord* pParentRecord, CUiAnimViewNode* pNode);
    CRecord* AddTrackRecord(CRecord* pParentRecord, CUiAnimViewTrack* pTrack);
    CRecord* AddAnimNodeRecord(CRecord* pParentRecord, CUiAnimViewAnimNode* pNode);

    void FillNodesRec(CRecord* pRecord, CUiAnimViewNode* pCurrentNode);

    void EraseNodeRecordRec(CUiAnimViewNode* pNode);

    void UpdateNodeRecord(CRecord* pRecord);
    void UpdateTrackRecord(CRecord* pRecord, CUiAnimViewTrack* pTrack);
    void UpdateUiAnimNodeRecord(CRecord* pRecord, CUiAnimViewAnimNode* pAnimNode);

    void FillAutoCompletionListForFilter();

    // Utility function for handling material nodes
    // It'll return -1 if the found material isn't a multi-material.
    static int GetMatNameAndSubMtlIndexFromName(QString& matName, const char* nodeName);

    void CustomizeTrackColor(CUiAnimViewTrack* pTrack);
    void ClearCustomTrackColor(CUiAnimViewTrack* pTrack);

    // For drawing dope sheet
    void UpdateRecordVisibility();

    void UpdateDopeSheet();

    int GetInsertPosition(CRecord* pParentRecord, CUiAnimViewNode* pNode);

    void SelectRow(CUiAnimViewNode* pNode, const bool bEnsureVisible, const bool bDeselectOtherRows);
    void DeselectRow(CUiAnimViewNode* pNode);

    CUiAnimViewDopeSheetBase* m_pDopeSheet;
    CUiAnimViewDialog* m_pUiAnimViewDialog;

    typedef std::vector<CRecord*> ItemInfos;
    ItemInfos m_itemInfos;

    bool m_bSelectionChanging;
    bool m_bEditLock;

    QCursor m_arrowCursor;
    QCursor m_noIcon;

    UINT m_currentMatchIndex;
    UINT m_matchCount;

    bool m_bIgnoreNotifications;
    bool m_bNeedReload;
    float m_storedScrollPosition;

    // Drag and drop
    CUiAnimViewAnimNodeBundle m_draggedNodes;
    CUiAnimViewAnimNode* m_pDragTarget;

    std::unordered_map<const CUiAnimViewNode*, CRecord*> m_nodeToRecordMap;

    QMap<int, QIcon> m_imageList;
    QScopedPointer<Ui::CUiAnimViewNodesCtrl> ui;
};


typedef CUiAnimViewNode* CUiAnimViewNodePtr;
Q_DECLARE_METATYPE(CUiAnimViewNodePtr);
QDataStream& operator<<(QDataStream& out, const CUiAnimViewNodePtr& obj);
QDataStream& operator>>(QDataStream& in, CUiAnimViewNodePtr& obj);
