/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Atom/RHI/FactoryManagerBus.h>
#include <AtomToolsFramework/Document/AtomToolsDocumentApplication.h>
#include <AtomToolsFramework/EntityPreviewViewport/EntityPreviewViewportSettingsSystem.h>
#include <AtomToolsFramework/Graph/DynamicNode/DynamicNodeManager.h>
#include <AzToolsFramework/API/EditorWindowRequestBus.h>
#include <Document/MaterialGraphCompiler.h>
#include <GraphModel/Model/GraphContext.h>
#include <Window/MaterialCanvasMainWindow.h>

namespace MaterialCanvas
{
    //! The main application class for Material Canvas, setting up top level systems, document types, and the main window. 
    class MaterialCanvasApplication
        : public AtomToolsFramework::AtomToolsDocumentApplication
        , private AzToolsFramework::EditorWindowRequestBus::Handler
        , private AZ::RHI::FactoryManagerNotificationBus::Handler
        , private AtomToolsFramework::AtomToolsDocumentNotificationBus::Handler
    {
    public:
        AZ_TYPE_INFO(MaterialCanvas::MaterialCanvasApplication, "{30F90CA5-1253-49B5-8143-19CEE37E22BB}");

        using Base = AtomToolsFramework::AtomToolsDocumentApplication;

        MaterialCanvasApplication(int* argc, char*** argv);
        ~MaterialCanvasApplication();

        // AzFramework::Application overrides...
        void Reflect(AZ::ReflectContext* context) override;
        const char* GetCurrentConfigurationName() const override;
        void StartCommon(AZ::Entity* systemEntity) override;
        void Destroy() override;

    private:

        // AtomToolsFramework::AtomToolsApplication overrides...
        AZStd::vector<AZStd::string> GetCriticalAssetFilters() const override;

        // AzToolsFramework::EditorWindowRequests::Bus::Handler
        QWidget* GetAppMainWindow() override;

        // AZ::RHI::FactoryManagerNotificationBus::Handler overrides...
        void FactoryRegistered() override;

        // AtomToolsFramework::AtomToolsDocumentNotificationBus::Handler overrides...
        void OnDocumentOpened(const AZ::Uuid& documentId) override;
        void OnDocumentSaved(const AZ::Uuid& documentId) override;
        void OnDocumentUndoStateChanged(const AZ::Uuid& documentId) override;
        void OnDocumentClosed(const AZ::Uuid& documentId) override;
        void OnDocumentDestroyed(const AZ::Uuid& documentId) override;

        void InitDynamicNodeManager();
        void InitDynamicNodeEditData();
        void InitSharedGraphContext();
        void InitGraphViewSettings();
        void InitMaterialGraphDocumentType();
        void InitMaterialGraphNodeDocumentType();
        void InitShaderSourceDataDocumentType();
        void ApplyShaderBuildSettings();

        AZStd::unique_ptr<MaterialCanvasMainWindow> m_window;
        AZStd::unique_ptr<AtomToolsFramework::EntityPreviewViewportSettingsSystem> m_viewportSettingsSystem;
        AZStd::unique_ptr<AtomToolsFramework::DynamicNodeManager> m_dynamicNodeManager;
        AZStd::shared_ptr<GraphModel::GraphContext> m_graphContext;
        AtomToolsFramework::GraphViewSettingsPtr m_graphViewSettingsPtr;
        AZStd::unordered_map<AZ::Uuid, AZStd::unique_ptr<MaterialGraphCompiler>> m_graphCompilerMap;
    };
} // namespace MaterialCanvas
