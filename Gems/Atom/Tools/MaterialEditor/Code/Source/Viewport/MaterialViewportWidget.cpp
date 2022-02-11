/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#undef RC_INVOKED

#include <Atom/Component/DebugCamera/CameraComponent.h>
#include <Atom/Component/DebugCamera/NoClipControllerComponent.h>
#include <Atom/Feature/ACES/AcesDisplayMapperFeatureProcessor.h>
#include <Atom/Feature/ImageBasedLights/ImageBasedLightFeatureProcessorInterface.h>
#include <Atom/Feature/PostProcess/PostProcessFeatureProcessorInterface.h>
#include <Atom/Feature/PostProcessing/PostProcessingConstants.h>
#include <Atom/Feature/Utils/LightingPreset.h>
#include <Atom/Feature/Utils/ModelPreset.h>
#include <Atom/RHI/Device.h>
#include <Atom/RHI/RHISystemInterface.h>
#include <Atom/RPI.Public/Image/StreamingImage.h>
#include <Atom/RPI.Public/Material/Material.h>
#include <Atom/RPI.Public/Pass/Specific/SwapChainPass.h>
#include <Atom/RPI.Public/RPISystemInterface.h>
#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/RPI.Public/ViewportContext.h>
#include <Atom/RPI.Public/ViewportContextBus.h>
#include <Atom/RPI.Public/WindowContext.h>
#include <Atom/RPI.Reflect/Asset/AssetUtils.h>
#include <AtomCore/Instance/InstanceDatabase.h>
#include <AtomLyIntegration/CommonFeatures/Grid/GridComponentConfig.h>
#include <AtomLyIntegration/CommonFeatures/Grid/GridComponentConstants.h>
#include <AtomLyIntegration/CommonFeatures/ImageBasedLights/ImageBasedLightComponentBus.h>
#include <AtomLyIntegration/CommonFeatures/ImageBasedLights/ImageBasedLightComponentConstants.h>
#include <AtomLyIntegration/CommonFeatures/Material/MaterialComponentBus.h>
#include <AtomLyIntegration/CommonFeatures/Material/MaterialComponentConstants.h>
#include <AtomLyIntegration/CommonFeatures/Mesh/MeshComponentBus.h>
#include <AtomLyIntegration/CommonFeatures/Mesh/MeshComponentConstants.h>
#include <AtomLyIntegration/CommonFeatures/PostProcess/ExposureControl/ExposureControlBus.h>
#include <AtomLyIntegration/CommonFeatures/PostProcess/ExposureControl/ExposureControlComponentConstants.h>
#include <AtomLyIntegration/CommonFeatures/PostProcess/PostFxLayerComponentConstants.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzFramework/Components/NonUniformScaleComponent.h>
#include <AzFramework/Components/TransformComponent.h>
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzFramework/Viewport/ViewportControllerList.h>
#include <Document/MaterialDocumentRequestBus.h>
#include <Viewport/MaterialViewportRequestBus.h>
#include <Viewport/MaterialViewportSettings.h>
#include <Viewport/MaterialViewportWidget.h>
#include <Viewport/ui_MaterialViewportWidget.h>

AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option") // disable warnings spawned by QT
#include <QWindow>
AZ_POP_DISABLE_WARNING

namespace MaterialEditor
{
    static constexpr float DepthNear = 0.01f;

    MaterialViewportWidget::MaterialViewportWidget(const AZ::Crc32& toolId, QWidget* parent)
        : AtomToolsFramework::RenderViewportWidget(parent)
        , m_ui(new Ui::MaterialViewportWidget)
        , m_toolId(toolId)
        , m_viewportController(AZStd::make_shared<MaterialEditorViewportInputController>())
    {
        m_ui->setupUi(this);

        // The viewport context created by AtomToolsFramework::RenderViewportWidget has no name.
        // Systems like frame capturing and post FX expect there to be a context with DefaultViewportContextName
        auto viewportContextManager = AZ::Interface<AZ::RPI::ViewportContextRequestsInterface>::Get();
        const AZ::Name defaultContextName = viewportContextManager->GetDefaultViewportContextName();
        viewportContextManager->RenameViewportContext(GetViewportContext(), defaultContextName);

        // Create and register a scene with all available feature processors
        AZ::RPI::SceneDescriptor sceneDesc;
        sceneDesc.m_nameId = AZ::Name("MaterialViewport");
        m_scene = AZ::RPI::Scene::CreateScene(sceneDesc);
        m_scene->EnableAllFeatureProcessors();

        // Bind m_defaultScene to the GameEntityContext's AzFramework::Scene
        auto sceneSystem = AzFramework::SceneSystemInterface::Get();
        AZ_Assert(sceneSystem, "MaterialViewportWidget was unable to get the scene system during construction.");
        AZStd::shared_ptr<AzFramework::Scene> mainScene = sceneSystem->GetScene(AzFramework::Scene::MainSceneName);

        // This should never happen unless scene creation has changed.
        AZ_Assert(mainScene, "Main scenes missing during system component initialization");
        mainScene->SetSubsystem(m_scene);

        // Create a render pipeline from the specified asset for the window context and add the pipeline to the scene
        AZ::Data::Asset<AZ::RPI::AnyAsset> pipelineAsset = AZ::RPI::AssetUtils::LoadAssetByProductPath<AZ::RPI::AnyAsset>(
            m_defaultPipelineAssetPath.c_str(), AZ::RPI::AssetUtils::TraceLevel::Error);
        m_renderPipeline = AZ::RPI::RenderPipeline::CreateRenderPipelineForWindow(pipelineAsset, *GetViewportContext()->GetWindowContext().get());
        pipelineAsset.Release();
        m_scene->AddRenderPipeline(m_renderPipeline);

        // As part of our initialization we need to create the BRDF texture generation pipeline
        AZ::RPI::RenderPipelineDescriptor pipelineDesc;
        pipelineDesc.m_mainViewTagName = "MainCamera";
        pipelineDesc.m_name = "BRDFTexturePipeline";
        pipelineDesc.m_rootPassTemplate = "BRDFTexturePipeline";
        pipelineDesc.m_executeOnce = true;

        AZ::RPI::RenderPipelinePtr brdfTexturePipeline = AZ::RPI::RenderPipeline::CreateRenderPipeline(pipelineDesc);
        m_scene->AddRenderPipeline(brdfTexturePipeline);

        // Currently the scene has to be activated after render pipeline was added so some feature processors (i.e. imgui) can be
        // initialized properly with pipeline's pass information.
        m_scene->Activate();

        AZ::RPI::RPISystemInterface::Get()->RegisterScene(m_scene);

        AzFramework::EntityContextId entityContextId;
        AzFramework::GameEntityContextRequestBus::BroadcastResult(
            entityContextId, &AzFramework::GameEntityContextRequestBus::Events::GetGameEntityContextId);

        // Configure camera
        AzFramework::EntityContextRequestBus::EventResult(
            m_cameraEntity, entityContextId, &AzFramework::EntityContextRequestBus::Events::CreateEntity, "Cameraentity");
        AZ_Assert(m_cameraEntity != nullptr, "Failed to create camera entity.");

        // Add debug camera and controller components
        AZ::Debug::CameraComponentConfig cameraConfig(GetViewportContext()->GetWindowContext());
        cameraConfig.m_fovY = AZ::Constants::HalfPi;
        cameraConfig.m_depthNear = DepthNear;
        m_cameraComponent = m_cameraEntity->CreateComponent(azrtti_typeid<AZ::Debug::CameraComponent>());
        m_cameraComponent->SetConfiguration(cameraConfig);
        m_cameraEntity->CreateComponent(azrtti_typeid<AzFramework::TransformComponent>());
        m_cameraEntity->Activate();

        // Connect camera to pipeline's default view after camera entity activated
        m_renderPipeline->SetDefaultViewFromEntity(m_cameraEntity->GetId());

        // Configure tone mapper
        AzFramework::EntityContextRequestBus::EventResult(
            m_postProcessEntity, entityContextId, &AzFramework::EntityContextRequestBus::Events::CreateEntity, "postProcessEntity");
        AZ_Assert(m_postProcessEntity != nullptr, "Failed to create post process entity.");

        m_postProcessEntity->CreateComponent(AZ::Render::PostFxLayerComponentTypeId);
        m_postProcessEntity->CreateComponent(AZ::Render::ExposureControlComponentTypeId);
        m_postProcessEntity->CreateComponent(azrtti_typeid<AzFramework::TransformComponent>());
        m_postProcessEntity->Activate();

        // Init directional light processor
        m_directionalLightFeatureProcessor = m_scene->GetFeatureProcessor<AZ::Render::DirectionalLightFeatureProcessorInterface>();

        // Init display mapper processor
        m_displayMapperFeatureProcessor = m_scene->GetFeatureProcessor<AZ::Render::DisplayMapperFeatureProcessorInterface>();

        // Init Skybox
        m_skyboxFeatureProcessor = m_scene->GetFeatureProcessor<AZ::Render::SkyBoxFeatureProcessorInterface>();
        m_skyboxFeatureProcessor->Enable(true);
        m_skyboxFeatureProcessor->SetSkyboxMode(AZ::Render::SkyBoxMode::Cubemap);

        // Create IBL
        AzFramework::EntityContextRequestBus::EventResult(
            m_iblEntity, entityContextId, &AzFramework::EntityContextRequestBus::Events::CreateEntity, "IblEntity");
        AZ_Assert(m_iblEntity != nullptr, "Failed to create ibl entity.");

        m_iblEntity->CreateComponent(AZ::Render::ImageBasedLightComponentTypeId);
        m_iblEntity->CreateComponent(azrtti_typeid<AzFramework::TransformComponent>());
        m_iblEntity->Activate();

        // Create model
        AzFramework::EntityContextRequestBus::EventResult(
            m_modelEntity, entityContextId, &AzFramework::EntityContextRequestBus::Events::CreateEntity, "ViewportModel");
        AZ_Assert(m_modelEntity != nullptr, "Failed to create model entity.");

        m_modelEntity->CreateComponent(AZ::Render::MeshComponentTypeId);
        m_modelEntity->CreateComponent(AZ::Render::MaterialComponentTypeId);
        m_modelEntity->CreateComponent(azrtti_typeid<AzFramework::TransformComponent>());
        m_modelEntity->Activate();

        // Create shadow catcher
        AzFramework::EntityContextRequestBus::EventResult(
            m_shadowCatcherEntity, entityContextId, &AzFramework::EntityContextRequestBus::Events::CreateEntity, "ViewportShadowCatcher");
        AZ_Assert(m_shadowCatcherEntity != nullptr, "Failed to create shadow catcher entity.");
        m_shadowCatcherEntity->CreateComponent(AZ::Render::MeshComponentTypeId);
        m_shadowCatcherEntity->CreateComponent(AZ::Render::MaterialComponentTypeId);
        m_shadowCatcherEntity->CreateComponent(azrtti_typeid<AzFramework::TransformComponent>());
        m_shadowCatcherEntity->CreateComponent(azrtti_typeid<AzFramework::NonUniformScaleComponent>());
        m_shadowCatcherEntity->Activate();

        AZ::NonUniformScaleRequestBus::Event(
            m_shadowCatcherEntity->GetId(), &AZ::NonUniformScaleRequests::SetScale, AZ::Vector3{ 100, 100, 1.0 });

        AZ::Data::AssetId shadowCatcherModelAssetId = AZ::RPI::AssetUtils::GetAssetIdForProductPath(
            "materialeditor/viewportmodels/plane_1x1.azmodel", AZ::RPI::AssetUtils::TraceLevel::Error);
        AZ::Render::MeshComponentRequestBus::Event(
            m_shadowCatcherEntity->GetId(), &AZ::Render::MeshComponentRequestBus::Events::SetModelAssetId, shadowCatcherModelAssetId);

        auto shadowCatcherMaterialAsset = AZ::RPI::AssetUtils::LoadAssetByProductPath<AZ::RPI::MaterialAsset>(
            "materials/special/shadowcatcher.azmaterial", AZ::RPI::AssetUtils::TraceLevel::Error);
        if (shadowCatcherMaterialAsset)
        {
            m_shadowCatcherOpacityPropertyIndex =
                shadowCatcherMaterialAsset->GetMaterialTypeAsset()->GetMaterialPropertiesLayout()->FindPropertyIndex(
                    AZ::Name{ "settings.opacity" });
            AZ_Error("MaterialViewportWidget", m_shadowCatcherOpacityPropertyIndex.IsValid(), "Could not find opacity property");

            m_shadowCatcherMaterial = AZ::RPI::Material::Create(shadowCatcherMaterialAsset);
            AZ_Error("MaterialViewportWidget", m_shadowCatcherMaterial != nullptr, "Could not create shadow catcher material.");

            AZ::Render::MaterialAssignmentMap shadowCatcherMaterials;
            auto& shadowCatcherMaterialAssignment = shadowCatcherMaterials[AZ::Render::DefaultMaterialAssignmentId];
            shadowCatcherMaterialAssignment.m_materialInstance = m_shadowCatcherMaterial;
            shadowCatcherMaterialAssignment.m_materialInstancePreCreated = true;

            AZ::Render::MaterialComponentRequestBus::Event(
                m_shadowCatcherEntity->GetId(), &AZ::Render::MaterialComponentRequestBus::Events::SetMaterialOverrides,
                shadowCatcherMaterials);
        }

        // Create grid
        AzFramework::EntityContextRequestBus::EventResult(
            m_gridEntity, entityContextId, &AzFramework::EntityContextRequestBus::Events::CreateEntity, "ViewportGrid");
        AZ_Assert(m_gridEntity != nullptr, "Failed to create grid entity.");

        AZ::Render::GridComponentConfig gridConfig;
        gridConfig.m_gridSize = 4.0f;
        gridConfig.m_axisColor = AZ::Color(0.1f, 0.1f, 0.1f, 1.0f);
        gridConfig.m_primaryColor = AZ::Color(0.1f, 0.1f, 0.1f, 1.0f);
        gridConfig.m_secondaryColor = AZ::Color(0.1f, 0.1f, 0.1f, 1.0f);
        auto gridComponent = m_gridEntity->CreateComponent(AZ::Render::GridComponentTypeId);
        gridComponent->SetConfiguration(gridConfig);

        m_gridEntity->CreateComponent(azrtti_typeid<AzFramework::TransformComponent>());
        m_gridEntity->Activate();

        OnDocumentOpened(AZ::Uuid::CreateNull());

        // Attempt to apply the default lighting preset
        AZ::Render::LightingPresetPtr lightingPreset;
        MaterialViewportRequestBus::BroadcastResult(lightingPreset, &MaterialViewportRequestBus::Events::GetLightingPresetSelection);
        OnLightingPresetSelected(lightingPreset);

        // Attempt to apply the default model preset
        AZ::Render::ModelPresetPtr modelPreset;
        MaterialViewportRequestBus::BroadcastResult(modelPreset, &MaterialViewportRequestBus::Events::GetModelPresetSelection);
        OnModelPresetSelected(modelPreset);

        m_viewportController->Init(m_cameraEntity->GetId(), m_modelEntity->GetId(), m_iblEntity->GetId());

        // Apply user settinngs restored since last run
        AZStd::intrusive_ptr<MaterialViewportSettings> viewportSettings =
            AZ::UserSettings::CreateFind<MaterialViewportSettings>(AZ::Crc32("MaterialViewportSettings"), AZ::UserSettings::CT_GLOBAL);

        OnGridEnabledChanged(viewportSettings->m_enableGrid);
        OnShadowCatcherEnabledChanged(viewportSettings->m_enableShadowCatcher);
        OnAlternateSkyboxEnabledChanged(viewportSettings->m_enableAlternateSkybox);
        OnFieldOfViewChanged(viewportSettings->m_fieldOfView);
        OnDisplayMapperOperationTypeChanged(viewportSettings->m_displayMapperOperationType);

        AtomToolsFramework::AtomToolsDocumentNotificationBus::Handler::BusConnect(m_toolId);
        MaterialViewportNotificationBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
        AZ::TransformNotificationBus::MultiHandler::BusConnect(m_cameraEntity->GetId());

        GetControllerList()->Add(m_viewportController);
    }

    MaterialViewportWidget::~MaterialViewportWidget()
    {
        AZ::TransformNotificationBus::MultiHandler::BusDisconnect();
        AZ::TickBus::Handler::BusDisconnect();
        AtomToolsFramework::AtomToolsDocumentNotificationBus::Handler::BusDisconnect();
        MaterialViewportNotificationBus::Handler::BusDisconnect();
        AZ::Data::AssetBus::Handler::BusDisconnect();

        AzFramework::EntityContextId entityContextId;
        AzFramework::GameEntityContextRequestBus::BroadcastResult(
            entityContextId, &AzFramework::GameEntityContextRequestBus::Events::GetGameEntityContextId);

        AzFramework::EntityContextRequestBus::Event(
            entityContextId, &AzFramework::EntityContextRequestBus::Events::DestroyEntity, m_iblEntity);
        m_iblEntity = nullptr;

        AzFramework::EntityContextRequestBus::Event(
            entityContextId, &AzFramework::EntityContextRequestBus::Events::DestroyEntity, m_modelEntity);
        m_modelEntity = nullptr;

        AzFramework::EntityContextRequestBus::Event(
            entityContextId, &AzFramework::EntityContextRequestBus::Events::DestroyEntity, m_shadowCatcherEntity);
        m_shadowCatcherEntity = nullptr;

        AzFramework::EntityContextRequestBus::Event(
            entityContextId, &AzFramework::EntityContextRequestBus::Events::DestroyEntity, m_gridEntity);
        m_gridEntity = nullptr;

        AzFramework::EntityContextRequestBus::Event(
            entityContextId, &AzFramework::EntityContextRequestBus::Events::DestroyEntity, m_cameraEntity);
        m_cameraEntity = nullptr;

        AzFramework::EntityContextRequestBus::Event(
            entityContextId, &AzFramework::EntityContextRequestBus::Events::DestroyEntity, m_postProcessEntity);
        m_postProcessEntity = nullptr;

        for (DirectionalLightHandle& handle : m_lightHandles)
        {
            m_directionalLightFeatureProcessor->ReleaseLight(handle);
        }
        m_lightHandles.clear();

        auto sceneSystem = AzFramework::SceneSystemInterface::Get();
        AZ_Assert(sceneSystem, "MaterialViewportWidget was unable to get the scene system during destruction.");
        AZStd::shared_ptr<AzFramework::Scene> mainScene = sceneSystem->GetScene(AzFramework::Scene::MainSceneName);
        // This should never happen unless scene creation has changed.
        AZ_Assert(mainScene, "Main scenes missing during system component destruction");
        mainScene->UnsetSubsystem(m_scene);

        m_swapChainPass = nullptr;
        AZ::RPI::RPISystemInterface::Get()->UnregisterScene(m_scene);
        m_scene = nullptr;
    }

    void MaterialViewportWidget::OnDocumentOpened(const AZ::Uuid& documentId)
    {
        AZ::Data::Instance<AZ::RPI::Material> materialInstance;
        MaterialDocumentRequestBus::EventResult(materialInstance, documentId, &MaterialDocumentRequestBus::Events::GetInstance);

        AZ::Render::MaterialAssignmentMap materials;
        auto& materialAssignment = materials[AZ::Render::DefaultMaterialAssignmentId];
        materialAssignment.m_materialInstance = materialInstance;
        materialAssignment.m_materialInstancePreCreated = true;

        AZ::Render::MaterialComponentRequestBus::Event(
            m_modelEntity->GetId(), &AZ::Render::MaterialComponentRequestBus::Events::SetMaterialOverrides, materials);
    }

    void MaterialViewportWidget::OnLightingPresetSelected(AZ::Render::LightingPresetPtr preset)
    {
        if (!preset)
        {
            return;
        }

        AZ::Render::ImageBasedLightFeatureProcessorInterface* iblFeatureProcessor =
            m_scene->GetFeatureProcessor<AZ::Render::ImageBasedLightFeatureProcessorInterface>();
        AZ::Render::PostProcessFeatureProcessorInterface* postProcessFeatureProcessor =
            m_scene->GetFeatureProcessor<AZ::Render::PostProcessFeatureProcessorInterface>();

        AZ::Render::ExposureControlSettingsInterface* exposureControlSettingInterface =
            postProcessFeatureProcessor->GetOrCreateSettingsInterface(m_postProcessEntity->GetId())
                ->GetOrCreateExposureControlSettingsInterface();

        Camera::Configuration cameraConfig;
        Camera::CameraRequestBus::EventResult(
            cameraConfig, m_cameraEntity->GetId(), &Camera::CameraRequestBus::Events::GetCameraConfiguration);

        bool enableAlternateSkybox = false;
        MaterialViewportRequestBus::BroadcastResult(enableAlternateSkybox, &MaterialViewportRequestBus::Events::GetAlternateSkyboxEnabled);

        preset->ApplyLightingPreset(
            iblFeatureProcessor, m_skyboxFeatureProcessor, exposureControlSettingInterface, m_directionalLightFeatureProcessor,
            cameraConfig, m_lightHandles, m_shadowCatcherMaterial, m_shadowCatcherOpacityPropertyIndex, enableAlternateSkybox);
    }

    void MaterialViewportWidget::OnLightingPresetChanged(AZ::Render::LightingPresetPtr preset)
    {
        AZ::Render::LightingPresetPtr selectedPreset;
        MaterialViewportRequestBus::BroadcastResult(selectedPreset, &MaterialViewportRequestBus::Events::GetLightingPresetSelection);
        if (selectedPreset == preset)
        {
            OnLightingPresetSelected(preset);
        }
    }

    void MaterialViewportWidget::OnModelPresetSelected(AZ::Render::ModelPresetPtr preset)
    {
        if (!preset)
        {
            return;
        }

        if (!preset->m_modelAsset.GetId().IsValid())
        {
            AZ_Warning(
                "MaterialViewportWidget", false, "Attempting to set invalid model for preset: '%s'\n.", preset->m_displayName.c_str());
            return;
        }

        if (preset->m_modelAsset.GetId() == m_modelAssetId)
        {
            return;
        }

        AZ::Render::MeshComponentRequestBus::Event(
            m_modelEntity->GetId(), &AZ::Render::MeshComponentRequestBus::Events::SetModelAsset, preset->m_modelAsset);

        m_modelAssetId = preset->m_modelAsset.GetId();

        AZ::Data::AssetBus::Handler::BusDisconnect();
        AZ::Data::AssetBus::Handler::BusConnect(m_modelAssetId);
    }

    void MaterialViewportWidget::OnModelPresetChanged(AZ::Render::ModelPresetPtr preset)
    {
        AZ::Render::ModelPresetPtr selectedPreset;
        MaterialViewportRequestBus::BroadcastResult(selectedPreset, &MaterialViewportRequestBus::Events::GetModelPresetSelection);
        if (selectedPreset == preset)
        {
            OnModelPresetSelected(preset);
        }
    }

    void MaterialViewportWidget::OnShadowCatcherEnabledChanged(bool enable)
    {
        AZ::Render::MeshComponentRequestBus::Event(
            m_shadowCatcherEntity->GetId(), &AZ::Render::MeshComponentRequestBus::Events::SetVisibility, enable);
    }

    void MaterialViewportWidget::OnGridEnabledChanged(bool enable)
    {
        if (m_gridEntity)
        {
            if (enable && m_gridEntity->GetState() == AZ::Entity::State::Init)
            {
                m_gridEntity->Activate();
            }
            else if (!enable && m_gridEntity->GetState() == AZ::Entity::State::Active)
            {
                m_gridEntity->Deactivate();
            }
        }
    }

    void MaterialViewportWidget::OnAlternateSkyboxEnabledChanged(bool enable)
    {
        AZ_UNUSED(enable);
        AZ::Render::LightingPresetPtr selectedPreset;
        MaterialViewportRequestBus::BroadcastResult(selectedPreset, &MaterialViewportRequestBus::Events::GetLightingPresetSelection);
        OnLightingPresetSelected(selectedPreset);
    }

    void MaterialViewportWidget::OnFieldOfViewChanged(float fieldOfView)
    {
        MaterialEditorViewportInputControllerRequestBus::Broadcast(
            &MaterialEditorViewportInputControllerRequestBus::Handler::SetFieldOfView, fieldOfView);
    }

    void MaterialViewportWidget::OnDisplayMapperOperationTypeChanged(AZ::Render::DisplayMapperOperationType operationType)
    {
        AZ::Render::DisplayMapperConfigurationDescriptor desc;
        desc.m_operationType = operationType;
        m_displayMapperFeatureProcessor->RegisterDisplayMapperConfiguration(desc);
    }

    void MaterialViewportWidget::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        if (m_modelAssetId == asset.GetId())
        {
            MaterialEditorViewportInputControllerRequestBus::Broadcast(&MaterialEditorViewportInputControllerRequestBus::Handler::Reset);
            AZ::Data::AssetBus::Handler::BusDisconnect(asset.GetId());
        }
    }

    void MaterialViewportWidget::OnTick(float deltaTime, AZ::ScriptTimePoint time)
    {
        AtomToolsFramework::RenderViewportWidget::OnTick(deltaTime, time);

        m_renderPipeline->AddToRenderTickOnce();

        if (m_shadowCatcherMaterial)
        {
            // Compile the m_shadowCatcherMaterial in OnTick because changes can only be compiled once per frame.
            // This is ignored when a compile isn't needed.
            m_shadowCatcherMaterial->Compile();
        }
    }

    void MaterialViewportWidget::OnTransformChanged(const AZ::Transform&, const AZ::Transform&)
    {
        const AZ::EntityId* currentBusId = AZ::TransformNotificationBus::GetCurrentBusId();
        if (m_cameraEntity && currentBusId && *currentBusId == m_cameraEntity->GetId() && m_directionalLightFeatureProcessor)
        {
            auto transform = AZ::Transform::CreateIdentity();
            AZ::TransformBus::EventResult(transform, m_cameraEntity->GetId(), &AZ::TransformBus::Events::GetWorldTM);
            for (const DirectionalLightHandle& id : m_lightHandles)
            {
                m_directionalLightFeatureProcessor->SetCameraTransform(id, transform);
            }
        }
    }
} // namespace MaterialEditor
