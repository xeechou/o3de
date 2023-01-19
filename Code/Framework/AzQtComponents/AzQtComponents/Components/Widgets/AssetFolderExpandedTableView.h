/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#if !defined(Q_MOC_RUN)
#include <AzQtComponents/AzQtComponentsAPI.h>
#include <AzQtComponents/Components/Widgets/ScrollBar.h>

AZ_PUSH_DISABLE_WARNING(4244, "-Wunknown-warning-option")
#include <QAbstractItemView>
AZ_POP_DISABLE_WARNING
#endif

class QSettings;

namespace AzQtComponents
{
    class Style;
    class AssetFolderExpandedTableViewDelegate;

    class AZ_QT_COMPONENTS_API AssetFolderExpandedTableView
        : public QAbstractItemView
    {
        Q_OBJECT
        Q_PROPERTY(ThumbnailSize thumbnailSize READ thumbnailSize WRITE setThumbnailSize)

    public:
        struct Config
        {
            struct Thumbnail
            {
                int width;
                int height;
                qreal borderRadius;
                int padding;
                QColor backgroundColor;
                qreal borderThickness;
                qreal selectedBorderThickness;
                QColor borderColor;
                QColor selectedBorderColor;
            };

            struct ExpandButton
            {
                int width;
                qreal borderRadius;
                qreal caretWidth;
                qreal caretHeight;
                QColor backgroundColor;
                QColor caretColor;
            };

            struct ChildFrame
            {
                int padding;
                qreal borderRadius;
                QColor borderColor;
                QColor backgroundColor;
                int closeButtonWidth;
            };

            int viewportPadding;
            int topItemsHorizontalSpacing;
            int topItemsVerticalSpacing;
            int childrenItemsHorizontalSpacing;
            Thumbnail rootThumbnail;
            Thumbnail childThumbnail;
            ExpandButton expandButton;
            ChildFrame childFrame;
        };

        static Config loadConfig(QSettings& settings);

        static Config defaultConfig();

        explicit AssetFolderExpandedTableView(QWidget* parent = nullptr);
        ~AssetFolderExpandedTableView() override;

        enum class ThumbnailSize
        {
            Small,
            Medium,
            Large
        };
        Q_ENUM(ThumbnailSize)

        void setThumbnailSize(ThumbnailSize size);
        ThumbnailSize thumbnailSize() const;

        void updateGeometries() override;
        QModelIndex indexAt(const QPoint& point) const override;
        void scrollTo(const QModelIndex& index, QAbstractItemView::ScrollHint hint) override;
        QRect visualRect(const QModelIndex& index) const override;

        Q_SIGNAL void IndexClicked(const QModelIndex& idx);
        Q_SIGNAL void IndexDoubleClicked(const QModelIndex& idx);

    protected:
        friend class Style;

        static bool polish(Style* style, QWidget* widget, const ScrollBar::Config& scrollBarConfig, const Config& config);
        void polish(const Config& config);

        QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
        int horizontalOffset() const override;
        int verticalOffset() const override;
        bool isIndexHidden(const QModelIndex& index) const override;
        void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags flags) override;
        QRegion visualRegionForSelection(const QItemSelection& selection) const override;

        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;

    private:
        void paintChildFrames(QPainter* painter) const;
        void paintItems(QPainter* painter) const;

        bool isExpandable(const QModelIndex& index) const;

        int rootThumbnailSizeInPixels() const;
        int childThumbnailSizeInPixels() const;

        AssetFolderExpandedTableViewDelegate* m_delegate;   
        AZ_PUSH_DISABLE_WARNING(4251, "-Wunknown-warning-option") // 'AzQtComponents::AssetFolderExpandedTableView::m_itemGeometry': class 'QHash<QPersistentModelIndex,QRect>' needs to have dll-interface to be used by clients of class 'AzQtComponents::AssetFolderExpandedTableView'
        QHash<QPersistentModelIndex, QRect> m_itemGeometry;
        struct ChildFrame
        {
            QPersistentModelIndex index;
            QVector<QRect> rects;
        };
        QVector<ChildFrame> m_childFrames;
        QSet<int> m_expandedRows;
        AZ_POP_DISABLE_WARNING
        ThumbnailSize m_thumbnailSize;
        Config m_config;
    };
}
