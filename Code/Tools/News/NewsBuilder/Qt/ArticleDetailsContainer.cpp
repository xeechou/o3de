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

#include "ArticleDetailsContainer.h"
#include "ResourceManagement/BuilderResourceManifest.h"
#include "ArticleDetails.h"
#include "Qt/ui_ArticleDetailsContainer.h"

namespace News
{

    ArticleDetailsContainer::ArticleDetailsContainer(
        QWidget* parent,
        BuilderResourceManifest& manifest)
        : QWidget(parent)
        , m_ui(new Ui::ArticleDetailsContainerWidget)
        , m_manifest(manifest)
        , m_articleDetails(nullptr)
    {
        m_ui->setupUi(this);
    }

    ArticleDetailsContainer::~ArticleDetailsContainer() {}

    //! When article is selected, old articleDetails control is removed, new one is created
    void ArticleDetailsContainer::SelectArticle(const QString& id)
    {
        closeArticleSlot();

        if (!id.isEmpty())
        {
            auto article = m_manifest.FindById(id);
            if (article)
            {
                m_articleDetails = new ArticleDetails(
                    m_ui->scrollAreaWidgetContents,
                    *article,
                    m_manifest);
                m_ui->scrollAreaWidgetContents->layout()->addWidget(m_articleDetails);
                connect(m_articleDetails, &ArticleDetails::updateArticleSignal,
                    this, &ArticleDetailsContainer::updateArticleSlot);
                connect(m_articleDetails, &ArticleDetails::deleteArticleSignal,
                    this, &ArticleDetailsContainer::deleteArticleSlot);
                connect(m_articleDetails, &ArticleDetails::closeArticleSignal,
                    this, &ArticleDetailsContainer::closeArticleSlot);
                connect(m_articleDetails, SIGNAL(logSignal(QString, LogType)),
                    this, SIGNAL(logSignal(QString, LogType)));
                connect(m_articleDetails, SIGNAL(orderChangedSignal(QString, bool)),
                    this, SIGNAL(orderChangedSignal(QString, bool)));
                m_selectedId = id;
            }
        }
    }

    void ArticleDetailsContainer::Reset()
    {
        SelectArticle(m_selectedId);
    }

    void ArticleDetailsContainer::updateArticleSlot()
    {
        emit updateArticleSignal(m_articleDetails->GetId());
    }

    void ArticleDetailsContainer::deleteArticleSlot()
    {
        emit deleteArticleSignal(m_articleDetails->GetId());
        closeArticleSlot();
    }

    void ArticleDetailsContainer::closeArticleSlot()
    {
        if (m_articleDetails)
        {
            emit closeArticleSignal(m_selectedId);
            delete m_articleDetails;
            m_articleDetails = nullptr;
            m_selectedId = "";
        }
    }

#include "Qt/moc_ArticleDetailsContainer.cpp"

}
