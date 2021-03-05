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

#include <ReflectionProbe/EditorReflectionProbeComponent.h>
#include <AzFramework/StringFunc/StringFunc.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <AzToolsFramework/Entity/EditorEntityInfoBus.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzCore/IO/SystemFile.h>
#include <Atom/RPI.Reflect/Image/StreamingImagePoolAsset.h>
#include <Atom/Utils/DdsFile.h>
#include <AzCore/Component/Entity.h>

AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option") // disable warnings spawned by QT
#include <QApplication>
#include <QMessageBox>
#include <QProgressDialog>
AZ_POP_DISABLE_WARNING

namespace AZ
{
    namespace Render
    {
        void EditorReflectionProbeComponent::Reflect(AZ::ReflectContext* context)
        {
            BaseClass::Reflect(context);

            if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<EditorReflectionProbeComponent, BaseClass>()
                    ->Version(2, ConvertToEditorRenderComponentAdapter<1>)
                    ->Field("useBakedCubemap", &EditorReflectionProbeComponent::m_useBakedCubemap)
                    ->Field("bakedCubeMapRelativePath", &EditorReflectionProbeComponent::m_bakedCubeMapRelativePath)
                    ->Field("authoredCubeMapAsset", &EditorReflectionProbeComponent::m_authoredCubeMapAsset)
                ;

                if (AZ::EditContext* editContext = serializeContext->GetEditContext())
                {
                    editContext->Class<EditorReflectionProbeComponent>(
                        "Reflection Probe", "The ReflectionProbe component captures an IBL specular reflection at a specific position in the level")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                            ->Attribute(AZ::Edit::Attributes::Category, "Atom")
                            ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/Component_Placeholder.svg")
                            ->Attribute(AZ::Edit::Attributes::ViewportIcon, "editor/icons/components/viewport/component_placeholder.png")
                            ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                            ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                            ->Attribute(AZ::Edit::Attributes::PrimaryAssetType, AZ::AzTypeInfo<RPI::ModelAsset>::Uuid())
                        ->ClassElement(AZ::Edit::ClassElements::Group, "Cubemap")
                            ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                            ->DataElement(AZ::Edit::UIHandlers::Default, &EditorReflectionProbeComponent::m_useBakedCubemap, "Use Baked Cubemap", "Selects between a cubemap that captures the environment at location in the scene or a preauthored cubemap")
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorReflectionProbeComponent::OnUseBakedCubemapChanged)
                            ->UIElement(AZ::Edit::UIHandlers::Button, "Bake Reflection Probe", "Bake Reflection Probe")
                                ->Attribute(AZ::Edit::Attributes::NameLabelOverride, "")
                                ->Attribute(AZ::Edit::Attributes::ButtonText, "Bake Reflection Probe")
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorReflectionProbeComponent::BakeReflectionProbe)
                                ->Attribute(AZ::Edit::Attributes::Visibility, &EditorReflectionProbeComponent::GetBakedCubemapVisibilitySetting)
                            ->DataElement(AZ::Edit::UIHandlers::MultiLineEdit, &EditorReflectionProbeComponent::m_bakedCubeMapRelativePath, "Baked Cubemap Path", "Baked Cubemap Path")
                                ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                                ->Attribute(AZ::Edit::Attributes::Visibility, &EditorReflectionProbeComponent::GetBakedCubemapVisibilitySetting)
                            ->DataElement(AZ::Edit::UIHandlers::Default, &EditorReflectionProbeComponent::m_authoredCubeMapAsset, "Cubemap file", "Authored Cubemap file")
                                ->Attribute(AZ::Edit::Attributes::Visibility, &EditorReflectionProbeComponent::GetAuthoredCubemapVisibilitySetting)
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorReflectionProbeComponent::OnAuthoredCubemapChanged)
                        ;

                    editContext->Class<ReflectionProbeComponentController>(
                        "ReflectionProbeComponentController", "")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                            ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Default, &ReflectionProbeComponentController::m_configuration, "Configuration", "")
                            ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ;

                    editContext->Class<ReflectionProbeComponentConfig>(
                        "ReflectionProbeComponentConfig", "")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                            ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->ClassElement(AZ::Edit::ClassElements::Group, "Inner Extents")
                            ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                            ->DataElement(AZ::Edit::UIHandlers::Default, &ReflectionProbeComponentConfig::m_innerHeight, "Height", "Height of the reflection probe inner volume")
                                ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, Edit::PropertyRefreshLevels::ValuesOnly)
                            ->DataElement(AZ::Edit::UIHandlers::Default, &ReflectionProbeComponentConfig::m_innerLength, "Length", "Length of the reflection probe inner volume")
                                ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, Edit::PropertyRefreshLevels::ValuesOnly)
                            ->DataElement(AZ::Edit::UIHandlers::Default, &ReflectionProbeComponentConfig::m_innerWidth, "Width", "Width of the reflection probe inner volume")
                                ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, Edit::PropertyRefreshLevels::ValuesOnly)
                        ->ClassElement(AZ::Edit::ClassElements::Group, "Settings")
                            ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                            ->DataElement(AZ::Edit::UIHandlers::CheckBox, &ReflectionProbeComponentConfig::m_useParallaxCorrection, "Parallax Correction", "Correct the reflection to adjust for the offset from the capture position")
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, Edit::PropertyRefreshLevels::ValuesOnly)
                            ->DataElement(AZ::Edit::UIHandlers::CheckBox, &ReflectionProbeComponentConfig::m_showVisualization, "Show Visualization", "Show the reflection probe visualization sphere")
                                ->Attribute(AZ::Edit::Attributes::ChangeNotify, Edit::PropertyRefreshLevels::ValuesOnly)
                        ;
                }
            }

            if (auto behaviorContext = azrtti_cast<BehaviorContext*>(context))
            {
                behaviorContext->EBus<EditorReflectionProbeBus>("EditorReflectionProbeBus")
                    ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                    ->Attribute(AZ::Script::Attributes::Module, "render")
                    ->Event("BakeReflectionProbe", &EditorReflectionProbeInterface::BakeReflectionProbe)
                    ;
                
                behaviorContext->ConstantProperty("EditorReflectionProbeComponentTypeId", BehaviorConstant(Uuid(EditorReflectionProbeComponentTypeId)))
                    ->Attribute(AZ::Script::Attributes::Module, "render")
                    ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation);
            }
        }

        EditorReflectionProbeComponent::EditorReflectionProbeComponent(const ReflectionProbeComponentConfig& config)
            : BaseClass(config)
        {
        }

        void EditorReflectionProbeComponent::Activate()
        {
            BaseClass::Activate();
            AzFramework::EntityDebugDisplayEventBus::Handler::BusConnect(GetEntityId());
            AzToolsFramework::EditorComponentSelectionRequestsBus::Handler::BusConnect(GetEntityId());
            EditorReflectionProbeBus::Handler::BusConnect(GetEntityId());

            ReflectionProbeComponentConfig& configuration = m_controller.m_configuration;

            // special handling is required if this component is being cloned in the editor:
            // if the entityId in the configuration does not match this component's entityId it is being cloned
            AZ::u64 entityId = (AZ::u64)GetEntityId();
            if (configuration.m_entityId != EntityId::InvalidEntityId
                && configuration.m_entityId != entityId)
            {
                // clear the cubeMapRelativePath to prevent the newly cloned reflection probe
                // from using the same cubemap path as the original reflection probe
                configuration.m_bakedCubeMapRelativePath = "";
            }

            // update UI cubemap path display
            m_bakedCubeMapRelativePath = configuration.m_bakedCubeMapRelativePath;

            configuration.m_entityId = entityId;
        }

        void EditorReflectionProbeComponent::Deactivate()
        {
            EditorReflectionProbeBus::Handler::BusDisconnect(GetEntityId());
            AzToolsFramework::EditorComponentSelectionRequestsBus::Handler::BusDisconnect();
            AzFramework::EntityDebugDisplayEventBus::Handler::BusDisconnect();
            BaseClass::Deactivate();
        }

        void EditorReflectionProbeComponent::DisplayEntityViewport([[maybe_unused]] const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay)
        {
            // only draw the bounds if selected
            if (!IsSelected())
            {
                return;
            }

            AZ::Vector3 position = AZ::Vector3::CreateZero();
            AZ::TransformBus::EventResult(position, GetEntityId(), &AZ::TransformBus::Events::GetWorldTranslation);

            // draw AABB at probe position using the inner dimensions
            Color color(0.0f, 0.0f, 1.0f, 1.0f);
            debugDisplay.SetColor(color);

            ReflectionProbeComponentConfig& configuration = m_controller.m_configuration;
            AZ::Vector3 innerMin(position.GetX() - configuration.m_innerWidth / 2, position.GetY() - configuration.m_innerLength / 2, position.GetZ() - configuration.m_innerHeight / 2);
            AZ::Vector3 innerMax(position.GetX() + configuration.m_innerWidth / 2, position.GetY() + configuration.m_innerLength / 2, position.GetZ() + configuration.m_innerHeight / 2);
            debugDisplay.DrawWireBox(innerMin, innerMax);
        }

        AZ::Aabb EditorReflectionProbeComponent::GetEditorSelectionBoundsViewport([[maybe_unused]] const AzFramework::ViewportInfo& viewportInfo)
        {
            return m_controller.GetWorldBounds();
        }

        bool EditorReflectionProbeComponent::SupportsEditorRayIntersect()
        {
            return false;
        }

        AZ::u32 EditorReflectionProbeComponent::OnUseBakedCubemapChanged()
        {
            // save setting to the configuration
            m_controller.m_configuration.m_useBakedCubemap = m_useBakedCubemap;

            // refresh currently displayed cubemap
            m_controller.UpdateCubeMap();

            return AZ::Edit::PropertyRefreshLevels::EntireTree;
        }

        AZ::u32 EditorReflectionProbeComponent::OnAuthoredCubemapChanged()
        {
            // save the selected authored asset to the configuration
            m_controller.m_configuration.m_authoredCubeMapAsset = m_authoredCubeMapAsset;

            // refresh currently displayed cubemap
            m_controller.UpdateCubeMap();

            return AZ::Edit::PropertyRefreshLevels::None;
        }

        AZ::u32 EditorReflectionProbeComponent::GetBakedCubemapVisibilitySetting()
        {
            // controls specific to baked cubemaps call this to determine their visibility
            // they are visible when the mode is set to baked, otherwise hidden
            return m_useBakedCubemap ? AZ::Edit::PropertyVisibility::Show : AZ::Edit::PropertyVisibility::Hide;
        }

        AZ::u32 EditorReflectionProbeComponent::GetAuthoredCubemapVisibilitySetting()
        {
            // controls specific to authored cubemaps call this to determine their visibility
            // they are hidden when the mode is set to baked, otherwise visible
            return m_useBakedCubemap ? AZ::Edit::PropertyVisibility::Hide : AZ::Edit::PropertyVisibility::Show;
        }

        AZ::u32 EditorReflectionProbeComponent::BakeReflectionProbe()
        {
            if (!m_useBakedCubemap)
            {
                AZ_Assert(false, "BakeReflectionProbe() called on a Reflection Probe set to use an authored cubemap");
                return AZ::Edit::PropertyRefreshLevels::None;
            }

            if (m_bakeInProgress)
            {
                return AZ::Edit::PropertyRefreshLevels::None;
            }

            // retrieve entity visibility
            bool isHidden = false;
            AzToolsFramework::EditorEntityInfoRequestBus::EventResult(
                isHidden,
                GetEntityId(),
                &AzToolsFramework::EditorEntityInfoRequestBus::Events::IsHidden);

            // the entity must be visible in order to bake
            if (isHidden)
            {
                QMessageBox::information(
                    QApplication::activeWindow(),
                    "Reflection Probe",
                    "This Reflection Probe entity is hidden, it must be visible in order to bake the cubemap.",
                    QMessageBox::Ok);

                return AZ::Edit::PropertyRefreshLevels::None;
            }

            // callback from the EnvironmentCubeMapPass when the cubemap render is complete
            BuildCubeMapCallback buildCubeMapCallback = [this](uint8_t* const* cubeMapFaceTextureData, const RHI::Format cubeMapTextureFormat)
            {
                char projectPath[AZ_MAX_PATH_LEN];
                AZ::IO::FileIOBase::GetInstance()->ResolvePath("@devassets@", projectPath, AZ_MAX_PATH_LEN);

                // retrieve the source cubemap path from the configuration
                // we need to make sure to use the same source cubemap for each bake
                AZStd::string cubeMapRelativePath = m_controller.m_configuration.m_bakedCubeMapRelativePath;
                AZStd::string cubeMapFullPath;

                if (!cubeMapRelativePath.empty())
                {
                    // test to see if the cubemap file is actually there, if it was removed we need to
                    // generate a new filename, otherwise it will cause an error in the asset system
                    AzFramework::StringFunc::Path::Join(projectPath, cubeMapRelativePath.c_str(), cubeMapFullPath, true, true);

                    if (!AZ::IO::FileIOBase::GetInstance()->Exists(cubeMapFullPath.c_str()))
                    {
                        // clear it to force the generation of a new filename
                        cubeMapRelativePath.clear();
                    }
                }

                // build a new cubemap path if necessary
                if (cubeMapRelativePath.empty())
                {
                    // the file name is a combination of the entity name, a UUID, and the filemask
                    Entity* entity = GetEntity();
                    AZ_Assert(entity, "ReflectionProbe entity is null");
                   
                    AZ::Uuid uuid = AZ::Uuid::CreateRandom();
                    AZStd::string uuidString;
                    uuid.ToString(uuidString);

                    cubeMapRelativePath = "ReflectionProbes/" + entity->GetName() + "_" + uuidString + "_iblspecularcm.dds";

                    // replace any invalid filename characters
                    auto invalidCharacters = [](char letter)
                    {
                        return
                            letter == ':' || letter == '"' || letter == '\'' ||
                            letter == '{' || letter == '}' ||
                            letter == '<' || letter == '>';
                    };
                    AZStd::replace_if(cubeMapRelativePath.begin(), cubeMapRelativePath.end(), invalidCharacters, '_');

                    // build the full source path
                    AzFramework::StringFunc::Path::Join(projectPath, cubeMapRelativePath.c_str(), cubeMapFullPath, true, true);
                }

                // make sure the folder is created
                AZStd::string reflectionProbeFolder;
                AzFramework::StringFunc::Path::GetFolderPath(cubeMapFullPath.data(), reflectionProbeFolder);
                AZ::IO::SystemFile::CreateDir(reflectionProbeFolder.c_str());

                // check out the file in source control                
                bool checkedOutSuccessfully = false;
                using ApplicationBus = AzToolsFramework::ToolsApplicationRequestBus;
                ApplicationBus::BroadcastResult(
                    checkedOutSuccessfully,
                    &ApplicationBus::Events::RequestEditForFileBlocking,
                    cubeMapFullPath.c_str(),
                    "Checking out for edit...",
                    ApplicationBus::Events::RequestEditProgressCallback());
                
                if (!checkedOutSuccessfully)
                {
                    AZ_Error("ReflectionProbe", false, "Failed to write \"%s\", source control checkout failed", cubeMapFullPath.c_str());
                }

                // write the cubemap data to the .dds file
                WriteOutputFile(cubeMapFullPath.c_str(), cubeMapFaceTextureData, cubeMapTextureFormat);

                // save the relative source path in the configuration
                AzToolsFramework::ScopedUndoBatch undoBatch("Cubemap path changed.");
                m_controller.m_configuration.m_bakedCubeMapRelativePath = cubeMapRelativePath;
                SetDirty();

                // update UI cubemap path display
                m_bakedCubeMapRelativePath = cubeMapRelativePath;

                // call the feature processor to notify when the asset is created and ready
                NotifyCubeMapAssetReadyCallback notifyCubeMapAssetReadyCallback = [this](const Data::Asset<RPI::StreamingImageAsset>& cubeMapAsset)
                {
                    // update configuration with the new baked cubemap asset
                    m_controller.m_configuration.m_bakedCubeMapAsset = { cubeMapAsset.GetAs<RPI::StreamingImageAsset>(), AZ::Data::AssetLoadBehavior::PreLoad };

                    // refresh the currently rendered cubemap
                    m_controller.UpdateCubeMap();

                    // update the UI
                    AzToolsFramework::PropertyEditorGUIMessages::Bus::Broadcast(&AzToolsFramework::PropertyEditorGUIMessages::RequestRefresh, AzToolsFramework::PropertyModificationRefreshLevel::Refresh_AttributesAndValues);

                    // signal completion
                    m_bakeInProgress = false;
                };

                AZStd::string cubeMapRelativeAssetPath = cubeMapRelativePath + ".streamingimage";
                m_controller.m_featureProcessor->NotifyCubeMapAssetReady(cubeMapRelativeAssetPath, notifyCubeMapAssetReadyCallback);
            };

            // initiate the cubemap bake
            m_bakeInProgress = true;
            m_controller.BakeReflectionProbe(buildCubeMapCallback);

            // show a dialog box letting the user know the probe is baking
            QProgressDialog bakeDialog;
            bakeDialog.setWindowFlags(bakeDialog.windowFlags() & ~Qt::WindowCloseButtonHint);
            bakeDialog.setLabelText("Baking Reflection Probe...");
            bakeDialog.setWindowModality(Qt::WindowModal);
            bakeDialog.setMaximumSize(QSize(256, 96));
            bakeDialog.setMinimum(0);
            bakeDialog.setMaximum(0);
            bakeDialog.setMinimumDuration(0);
            bakeDialog.setAutoClose(false);
            bakeDialog.setCancelButton(nullptr);
            bakeDialog.show();

            // display until finished or canceled
            while (m_bakeInProgress && !bakeDialog.wasCanceled())
            {
                QApplication::processEvents();
                AZStd::this_thread::sleep_for(AZStd::chrono::milliseconds(100));
            }

            return AZ::Edit::PropertyRefreshLevels::ValuesOnly;
        }

        void EditorReflectionProbeComponent::WriteOutputFile(AZStd::string filePath, uint8_t* const* cubeMapTextureData, const RHI::Format cubeMapTextureFormat)
        {
            static const u32 CubeMapFaceSize = 1024;
            static const u32 NumCubeMapFaces = 6;

            u32 bytesPerTexel = RHI::GetFormatSize(cubeMapTextureFormat);
            u32 bytesPerCubeMapFace = CubeMapFaceSize * CubeMapFaceSize * bytesPerTexel;

            AZStd::vector<uint8_t> buffer;
            buffer.resize_no_construct(bytesPerCubeMapFace * NumCubeMapFaces);
            for (AZ::u32 i = 0; i < NumCubeMapFaces; ++i)
            {
                memcpy(buffer.data() + (i * bytesPerCubeMapFace), cubeMapTextureData[i], bytesPerCubeMapFace);
            }

            DdsFile::DdsFileData ddsFileData;
            ddsFileData.m_size.m_width = CubeMapFaceSize;
            ddsFileData.m_size.m_height = CubeMapFaceSize;
            ddsFileData.m_format = cubeMapTextureFormat;
            ddsFileData.m_isCubemap = true;
            ddsFileData.m_buffer = &buffer;

            auto outcome = AZ::DdsFile::WriteFile(filePath, ddsFileData);
            if (!outcome)
            {
                AZ_Warning("WriteDds", false, outcome.GetError().m_message.c_str());
            }
        }
    } // namespace Render
} // namespace AZ
