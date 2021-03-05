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

#include <AzCore/Component/EntityId.h>

// Disable warnings generated by QT headers:
// 4251: needs to have dll-interface to be used by clients
// 4800: 'uint': forcing value to bool 'true' or 'false'
AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option")
#include <QBoxLayout>
#include <QToolButton>
#include <QComboBox>
#include <QEvent>
AZ_POP_DISABLE_WARNING

namespace NvCloth
{
    namespace Editor
    {
        //! Wrapper widget for a combo box with a button at the end.
        class ComboBoxEditButtonPair
            : public QWidget
        {
        public:
            explicit ComboBoxEditButtonPair(QWidget* parent);

            QComboBox* GetComboBox();
            QToolButton* GetEditButton();

            void SetEntityId(AZ::EntityId entityId);
            AZ::EntityId GetEntityId() const;

        private:
            bool eventFilter(QObject *object, QEvent *event) override;

            QComboBox* m_comboBox = nullptr;
            QToolButton* m_editButton = nullptr;

            AZ::EntityId m_entityId;
        };
    } // namespace Editor
} // namespace NvCloth
