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

#include <AzToolsFramework/UI/Prefab/LevelRootUiHandler.h>

#include <AzToolsFramework/UI/Prefab/PrefabEditInterface.h>
#include <AzToolsFramework/Prefab/PrefabPublicInterface.h>
#include <AzToolsFramework/UI/Outliner/EntityOutlinerListModel.hxx>

#include <QAbstractItemModel>
#include <QPainter>
#include <QPainterPath>
#include <QTreeView>

namespace AzToolsFramework
{
    const QColor LevelRootUiHandler::m_levelRootBorderColor = QColor("#656565");
    const QString LevelRootUiHandler::m_levelRootIconPath = QString(":/Level/level.svg");

    LevelRootUiHandler::LevelRootUiHandler()
    {
        m_prefabEditInterface = AZ::Interface<Prefab::PrefabEditInterface>::Get();

        if (m_prefabEditInterface == nullptr)
        {
            AZ_Assert(false, "LevelRootUiHandler - could not get PrefabEditInterface on LevelRootUiHandler construction.");
            return;
        }

        m_prefabPublicInterface = AZ::Interface<Prefab::PrefabPublicInterface>::Get();

        if (m_prefabPublicInterface == nullptr)
        {
            AZ_Assert(false, "LevelRootUiHandler - could not get PrefabPublicInterface on LevelRootUiHandler construction.");
            return;
        }
    }

    QPixmap LevelRootUiHandler::GenerateItemIcon(AZ::EntityId /*entityId*/) const
    {
        return QPixmap(m_levelRootIconPath);
    }

    bool LevelRootUiHandler::CanToggleLockVisibility(AZ::EntityId /*entityId*/) const
    {
        return false;
    }

    void LevelRootUiHandler::PaintItemBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
    {
        if (!painter)
        {
            AZ_Warning("LevelRootUiHandler", false, "LevelRootUiHandler - painter is nullptr, can't draw Prefab outliner background.");
            return;
        }

        QPen borderLinePen(m_levelRootBorderColor, m_levelRootBorderThickness);

        QRect rect = option.rect;
        rect.setLeft(rect.left() + (m_levelRootBorderThickness / 2));

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(borderLinePen);
        
        // Draw border at the bottom
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    }
}
