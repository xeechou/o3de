/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GemCatalog/GemCatalogHeaderWidget.h>
#include <CreateAGemScreen.h>
#include <ProjectUtils.h>
#include <PythonBindingsInterface.h>
#include <ScreenHeaderWidget.h>

#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>


namespace O3DE::ProjectManager
{

    CreateGem::CreateGem(QWidget* parent)
        : ScreenWidget(parent)
    {
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setSpacing(0);
        vLayout->setContentsMargins(0, 0, 0, 0);

        ScreenHeader* m_header = new ScreenHeader(this);
        m_header->setSubTitle(tr("Create a new gem"));
        connect(
            m_header->backButton(),
            &QPushButton::clicked,
            this,
            [&]()
            {
                emit GoToPreviousScreenRequest();
            });
        vLayout->addWidget(m_header);

        m_stackWidget = new QStackedWidget(this);
        m_stackWidget->setContentsMargins(0, 0, 0, 0);
        
        m_stackWidget->setObjectName("createAGemRHS");
        m_stackWidget->addWidget(CreateGemSetupScrollArea());  //tr("1. Gem Setup");
        m_stackWidget->addWidget(CreateGemDetailsScrollArea());//tr("2. Gem Details");
        m_stackWidget->addWidget(CreateGemCreatorScrollArea());//tr("3. Creator Details");
        vLayout->addWidget(m_stackWidget);

        QFrame* footerFrame = new QFrame(this);
        footerFrame->setObjectName("createAGemFooter");
        m_backNextButtons = new QDialogButtonBox(this);
        m_backNextButtons->setObjectName("footer");
        QVBoxLayout* footerLayout = new QVBoxLayout();
        footerLayout->setContentsMargins(0, 0, 0, 0);
        footerFrame->setLayout(footerLayout);
        footerLayout->addWidget(m_backNextButtons);
        vLayout->addWidget(footerFrame);

        m_backButton = m_backNextButtons->addButton(tr("Back"), QDialogButtonBox::RejectRole);
        m_backButton->setProperty("secondary", true);
        m_nextButton = m_backNextButtons->addButton(tr("Next"), QDialogButtonBox::ApplyRole);

        connect(m_backButton, &QPushButton::clicked, this, &CreateGem::HandleBackButton);
        connect(m_nextButton, &QPushButton::clicked, this, &CreateGem::HandleNextButton);

        setObjectName("createAGemBody");
        setLayout(vLayout);
    }



    void CreateGem::LoadButtonsFromGemTemplatePaths(QVBoxLayout* gemSetupLayout)
    {
        m_radioButtonGroup = new QButtonGroup();

        auto templatesResult = PythonBindingsInterface::Get()->GetGemTemplates();
        if (templatesResult.IsSuccess() && !templatesResult.GetValue().isEmpty())
        {
            m_gemTemplates = templatesResult.GetValue();
            for (int index = 0; index < m_gemTemplates.size(); ++index)
            {
                const ProjectTemplateInfo& gemTemplate = m_gemTemplates.at(index);

                QRadioButton* button = new QRadioButton(gemTemplate.m_displayName);
                button->setObjectName("createAGem");
                m_radioButtonGroup->addButton(button, index);

                QLabel* buttonSubtext = new QLabel(gemTemplate.m_summary);
                buttonSubtext->setObjectName("createAGemRadioButtonSubtext");

                gemSetupLayout->addWidget(button);
                gemSetupLayout->addWidget(buttonSubtext);
            }
            QAbstractButton* firstButton = m_radioButtonGroup->button(0);
            firstButton->setChecked(true);
        }
    }

    QScrollArea* CreateGem::CreateGemSetupScrollArea()
    {
        QScrollArea* gemSetupScrollArea = new QScrollArea();
        gemSetupScrollArea->setWidgetResizable(true);
        gemSetupScrollArea->setObjectName("createAGemRightPane");
        QFrame* gemSetupFrame = new QFrame();

        QWidget* gemSetupScrollWidget = new QWidget();
        gemSetupScrollArea->setWidget(gemSetupScrollWidget);

        QVBoxLayout* gemSetupLayout = new QVBoxLayout();
        gemSetupLayout->setAlignment(Qt::AlignTop);
        gemSetupLayout->addWidget(gemSetupFrame);
        gemSetupScrollWidget->setLayout(gemSetupLayout);
        QLabel* rightPaneHeader = new QLabel(tr("Please Choose a Gem Template"));
        rightPaneHeader->setObjectName("rightPaneHeader");
        QLabel* rightPaneSubheader = new QLabel(tr("Gems can contain assets new functionality and such as scripts, animations, meshes, textures, and more."));
        rightPaneSubheader->setObjectName("rightPaneSubheader");
        gemSetupLayout->addWidget(rightPaneHeader);
        gemSetupLayout->addWidget(rightPaneSubheader);

        LoadButtonsFromGemTemplatePaths(gemSetupLayout);
        m_formFolderRadioButton = new QRadioButton("Choose existing template");
        m_formFolderRadioButton->setObjectName("createAGem");
        m_radioButtonGroup->addButton(m_formFolderRadioButton);

        m_gemTemplateLocation = new FormFolderBrowseEditWidget(tr("Gem Template Location*"), "", "", tr("A path must be provided."));
        m_gemTemplateLocation->setObjectName("createAGemRadioButtonSubFormField");
        gemSetupLayout->addWidget(m_formFolderRadioButton);
        gemSetupLayout->addWidget(m_gemTemplateLocation);

        return gemSetupScrollArea;
    }

    QScrollArea* CreateGem::CreateGemDetailsScrollArea()
    {
        QScrollArea* gemDetailsScrollArea = new QScrollArea();
        gemDetailsScrollArea->setWidgetResizable(true);
        gemDetailsScrollArea->setObjectName("createAGemRightPane");
        QFrame* gemDetailsFrame = new QFrame();

        QWidget* gemDetailsScrollWidget = new QWidget();
        gemDetailsScrollArea->setWidget(gemDetailsScrollWidget);

        QVBoxLayout* gemDetailsLayout = new QVBoxLayout(this);
        gemDetailsLayout->setAlignment(Qt::AlignTop);
        gemDetailsLayout->addWidget(gemDetailsFrame);
        gemDetailsScrollWidget->setLayout(gemDetailsLayout);

        QLabel* secondRightPaneHeader = new QLabel(tr("Enter Gem Details"));
        secondRightPaneHeader->setObjectName("rightPaneDetailsHeader");
        gemDetailsLayout->addWidget(secondRightPaneHeader);

        m_gemName = new FormLineEditWidget(
            tr("Gem name*"),
            "",
            tr("The unique name for your gem consisting of only alphanumeric characters, '-' and '_'."),
            tr("A gem system name is required."));
        m_gemName->lineEdit()->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z]+[a-zA-Z0-9\\-\\_]*"), this));
        m_gemName->setObjectName("createAGemFormLineEdit");
        gemDetailsLayout->addWidget(m_gemName);

        m_gemDisplayName = new FormLineEditWidget(
            tr("Gem Display name*"), "", tr("The name displayed in the Gem Catalog"), tr("A gem display name is required."));
        m_gemDisplayName->lineEdit()->setValidator(new QRegularExpressionValidator(QRegularExpression("( |\\w)+"), this));
        m_gemDisplayName->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_gemDisplayName);

        m_gemSummary = new FormLineEditWidget(tr("Gem Summary"), "", tr("A short description of your Gem"), "");
        m_gemSummary->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_gemSummary);

        m_requirements = new FormLineEditWidget(tr("Requirements"), "", tr("Notice of any requirements your Gem. i.e. This requires X other gem"), "");
        m_requirements->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_requirements);

        m_license = new FormLineEditWidget(
            tr("License*"), "", tr("License uses goes here: i.e. Apache-2.0 or MIT"), tr("License details are required."));
        m_license->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_license);

        m_licenseURL = new FormLineEditWidget(tr("License URL"), "", tr("Link to the license web site i.e. https://opensource.org/licenses/Apache-2.0"), "");
        m_licenseURL->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_licenseURL);

        m_userDefinedGemTags = new FormLineEditWidget(tr("User-defined Gem Tags (Comma separated list)"), "");
        m_userDefinedGemTags->lineEdit()->setValidator(new QRegularExpressionValidator(QRegularExpression("(\\w+)(,\\s?\\w*)*"), this));
        m_userDefinedGemTags->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_userDefinedGemTags);

        m_gemLocation = new FormFolderBrowseEditWidget(tr("Gem Location"), "", tr("The path that the gem will be created at."), tr("The chosen directory must either not exist or be empty."));
        m_gemLocation->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_gemLocation);
        
        m_gemIconPath = new FormLineEditWidget(tr("Gem Icon Path"), "default.png", tr("Select Gem icon path"), "");
        m_gemIconPath->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_gemIconPath);

        m_documentationURL = new FormLineEditWidget(
            tr("Documentation URL"), "", tr("Link to any documentation of your Gem i.e. https://o3de.org/docs/user-guide/gems/..."), "");
        m_documentationURL->setObjectName("createAGem");
        gemDetailsLayout->addWidget(m_documentationURL);

        return gemDetailsScrollArea;
    }

    QScrollArea* CreateGem::CreateGemCreatorScrollArea()
    {
        QScrollArea* gemCreatorScrollArea = new QScrollArea();
        gemCreatorScrollArea->setWidgetResizable(true);
        gemCreatorScrollArea->setObjectName("createAGemRightPane");
        QFrame* gemCreatorFrame = new QFrame();

        QWidget* gemCreatorScrollWidget = new QWidget();
        gemCreatorScrollArea->setWidget(gemCreatorScrollWidget);

        QVBoxLayout* gemCreatorLayout = new QVBoxLayout();
        gemCreatorLayout->setAlignment(Qt::AlignTop);
        gemCreatorLayout->addWidget(gemCreatorFrame);
        gemCreatorScrollWidget->setLayout(gemCreatorLayout);

        QLabel* thirdRightPaneHeader = new QLabel(tr("Enter your Details"));
        thirdRightPaneHeader->setObjectName("rightPaneDetailsHeader");
        gemCreatorLayout->addWidget(thirdRightPaneHeader);

        m_origin =
            new FormLineEditWidget(tr("Creator Name*"), "", tr("The name of the gem creator or originator goes here. i.e. O3DE"), tr("You must provide a creator name."));
        m_origin->setObjectName("createAGem");
        gemCreatorLayout->addWidget(m_origin);

        m_originURL = new FormLineEditWidget(tr("Origin URL"), "", tr("The primary website for your Gem. i.e. http://o3de.org"), "");
        m_originURL->setObjectName("createAGem");
        gemCreatorLayout->addWidget(m_originURL);

        m_repositoryURL = new FormLineEditWidget(tr("Repository URL"), "", tr("Optional URL of the repository for this gem."), "");
        m_repositoryURL->setObjectName("createAGem");
        gemCreatorLayout->addWidget(m_repositoryURL);

        return gemCreatorScrollArea;
    }

    bool CreateGem::ValidateGemTemplateLocation()
    {
        bool gemTemplateLocationFilled = true;
        if (m_formFolderRadioButton->isChecked() && m_gemTemplateLocation->lineEdit()->text().isEmpty())
        {
            gemTemplateLocationFilled = false;
        }
        m_gemTemplateLocation->setErrorLabelVisible(!gemTemplateLocationFilled);
        return gemTemplateLocationFilled;
    }

    bool CreateGem::ValidateGemDisplayName()
    {
        bool gemScreenNameIsValid = true;
        if (m_gemDisplayName->lineEdit()->text().isEmpty() || !m_gemDisplayName->lineEdit()->hasAcceptableInput())
        {
            gemScreenNameIsValid = false;
        }
        m_gemDisplayName->setErrorLabelVisible(!gemScreenNameIsValid);
        return gemScreenNameIsValid;
    }

    bool CreateGem::ValidateGemName()
    {
        bool gemSystemNameIsValid = true;
        if (m_gemName->lineEdit()->text().isEmpty() || !m_gemName->lineEdit()->hasAcceptableInput())
        {
            gemSystemNameIsValid = false;
        }
        m_gemName->setErrorLabelVisible(!gemSystemNameIsValid);
        return gemSystemNameIsValid;
    }

    bool CreateGem::ValidateGemPath()
    {
        // This first isEmpty check is to check that the input field is not empty. If it is QDir will use the current
        // directory as the gem location, so that if that folder is also empty it will pass the chosenGemLocation.isEmpty()
        // check and place the created gem there, which is likely unintended behavior for a GUI app such as Project Manager.
        if (m_gemLocation->lineEdit()->text().isEmpty())
        {
            return false;
        }

        QDir chosenGemLocation = QDir(m_gemLocation->lineEdit()->text());

        bool locationValid = !chosenGemLocation.exists() || chosenGemLocation.isEmpty();
        m_gemLocation->setErrorLabelVisible(!locationValid);
        return locationValid;
    }

    bool CreateGem::ValidateFormNotEmpty(FormLineEditWidget* form)
    {
        bool formIsValid = !form->lineEdit()->text().isEmpty();
        form->setErrorLabelVisible(!formIsValid);
        return formIsValid;
    }

    bool CreateGem::ValidateRepositoryURL()
    {
        bool repositoryURLIsValid = true;
        if (!m_repositoryURL->lineEdit()->text().isEmpty())
        {
            QUrl checkURL = QUrl(m_repositoryURL->lineEdit()->text(), QUrl::StrictMode);
            repositoryURLIsValid = checkURL.isValid();
            m_repositoryURL->setErrorLabelText(tr("Repository URL is not valid."));
        }
        m_repositoryURL->setErrorLabelVisible(!repositoryURLIsValid);
        return repositoryURLIsValid;
    }

    void CreateGem::HandleBackButton()
    {
        if (m_stackWidget->currentIndex() > 0)
        {
            m_stackWidget->setCurrentIndex(m_stackWidget->currentIndex() - 1);
        }

        if (m_stackWidget->currentIndex() == 0)
        {
            m_backButton->setVisible(false);
        }

        m_nextButton->setText(tr("Next"));
    }

    void CreateGem::HandleNextButton()
    {
        if (m_stackWidget->currentIndex() == gemTemplateSelectionScreen && ValidateGemTemplateLocation())
        {
            m_backButton->setVisible(true);
            m_stackWidget->setCurrentIndex(gemDetailsScreen);
        }
        else if (m_stackWidget->currentIndex() == gemDetailsScreen)
        {
            bool gemNameValid = ValidateGemName();
            bool gemDisplayNameValid = ValidateGemDisplayName();
            bool licenseValid = ValidateFormNotEmpty(m_license);
            bool gemPathValid = ValidateGemPath();
            if (gemNameValid && gemDisplayNameValid && licenseValid && gemPathValid)
            {
                m_createGemInfo.m_displayName = m_gemDisplayName->lineEdit()->text();
                m_createGemInfo.m_name = m_gemName->lineEdit()->text();
                m_createGemInfo.m_summary = m_gemSummary->lineEdit()->text();
                m_createGemInfo.m_requirement = m_requirements->lineEdit()->text();
                m_createGemInfo.m_licenseText = m_license->lineEdit()->text();
                m_createGemInfo.m_licenseLink = m_licenseURL->lineEdit()->text();
                m_createGemInfo.m_documentationLink = m_documentationURL->lineEdit()->text();
                m_createGemInfo.m_path = m_gemLocation->lineEdit()->text();
                m_createGemInfo.m_features = m_userDefinedGemTags->lineEdit()->text().split(',');

                m_stackWidget->setCurrentIndex(gemCreatorDetailsScreen);
                m_nextButton->setText(tr("Create"));
            }
        }
        else if (m_stackWidget->currentIndex() == gemCreatorDetailsScreen)
        {
            bool originIsValid = ValidateFormNotEmpty(m_origin);
            bool repoURLIsValid = ValidateRepositoryURL();
            if (originIsValid && repoURLIsValid)
            {
                m_createGemInfo.m_origin = m_origin->lineEdit()->text();
                m_createGemInfo.m_originURL = m_originURL->lineEdit()->text();
                m_createGemInfo.m_repoUri = m_repositoryURL->lineEdit()->text();
                QString templateLocation;
                if (m_formFolderRadioButton->isChecked())
                {
                    templateLocation = m_gemTemplateLocation->lineEdit()->text();
                }
                else
                {
                    int templateID = m_radioButtonGroup->checkedId();
                    templateLocation = m_gemTemplates[templateID].m_path;
                }
                auto result = PythonBindingsInterface::Get()->CreateGem(templateLocation, m_createGemInfo);
                if (result.IsSuccess())
                {
                    emit GemCreated(result.GetValue<GemInfo>());
                    emit GoToPreviousScreenRequest();
                }
                else
                {
                    QMessageBox::critical(
                        this,
                        tr("Failed to create gem"),
                        tr("The gem failed to be created"));
                }
            }
        }
    }

    void CreateGem::UpdateNextButtonToCreate()
    {
        if (m_stackWidget->currentIndex() == gemCreatorDetailsScreen)
        {
            m_nextButton->setText(tr("Create"));
        }
        else
        {
            m_nextButton->setText(tr("Next"));
        }
    }

} // namespace O3DE::ProjectManager
