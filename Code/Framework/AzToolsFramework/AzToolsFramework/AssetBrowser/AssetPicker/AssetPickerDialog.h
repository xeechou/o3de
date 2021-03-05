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
#include <AzToolsFramework/AssetBrowser/Search/Filter.h>
#include <AzToolsFramework/UI/UICore/QTreeViewStateSaver.hxx>
#include <AzCore/std/smart_ptr/intrusive_ptr.h>

#include <QDialog>
#include <QScopedPointer>
#endif

class QKeyEvent;
class QModelIndex;
class QItemSelection;

namespace Ui
{
    class AssetPickerDialogClass;
}

namespace AzToolsFramework
{
    class QWidgetSavedState;

    namespace AssetBrowser
    {
        class ProductAssetBrowserEntry;
        class AssetBrowserFilterModel;
        class AssetBrowserModel;
        class AssetSelectionModel;

        class AssetPickerDialog
            : public QDialog
        {
            Q_OBJECT
        public:
            AZ_CLASS_ALLOCATOR(AssetPickerDialog, AZ::SystemAllocator, 0);
            explicit AssetPickerDialog(AssetSelectionModel& selection, QWidget* parent = nullptr);
            virtual ~AssetPickerDialog();

        protected:
            //////////////////////////////////////////////////////////////////////////
            // QDialog
            //////////////////////////////////////////////////////////////////////////
            void accept() override;
            void reject() override;
            void keyPressEvent(QKeyEvent* e) override;

        private Q_SLOTS:
            void DoubleClickedSlot(const QModelIndex& index);
            void SelectionChangedSlot();
            void RestoreState();
            void OnFilterUpdated();

        private:
            //! Evaluate whether current selection is valid.
            //! Valid selection requires exactly one item to be selected, must be source or product type, and must match the wildcard filter
            bool EvaluateSelection() const;
            void UpdatePreview() const;
            void SaveState();

            QScopedPointer<Ui::AssetPickerDialogClass> m_ui;
            AssetBrowserModel* m_assetBrowserModel = nullptr;
            QScopedPointer<AssetBrowserFilterModel> m_filterModel;
            AssetSelectionModel& m_selection;
            bool m_hasFilter;
            AZStd::unique_ptr<TreeViewState> m_filterStateSaver;
            AZStd::intrusive_ptr<QWidgetSavedState> m_persistentState;
        };
    } // AssetBrowser
} // AzToolsFramework
