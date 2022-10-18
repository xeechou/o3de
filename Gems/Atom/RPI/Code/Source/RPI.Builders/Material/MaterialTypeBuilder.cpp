/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MaterialTypeBuilder.h"
#include <Material/MaterialBuilderUtils.h>

#include <Atom/RPI.Edit/Material/MaterialTypeSourceData.h>
#include <Atom/RPI.Edit/Material/MaterialSourceData.h>
#include <Atom/RPI.Edit/Material/MaterialUtils.h>
#include <Atom/RPI.Edit/Common/JsonUtils.h>
#include <Atom/RPI.Edit/Common/AssetUtils.h>
//#include <Atom/RPI.Edit/Shader/ShaderUtils.h>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/IO/LocalFileIO.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <AssetBuilderSDK/SerializationDependencies.h>

namespace AZ
{
    namespace RPI
    {
        namespace
        {
            [[maybe_unused]] static constexpr char const MaterialTypeBuilderName[] = "MaterialTypeBuilder";

            //! Some shaders are used by multiple pipelines, so this name will be used in place of the pipeline name for the final shader filename.
            static constexpr char const PipelineNameForCommonShaders[] = "Common";
        }

        const char* MaterialTypeBuilder::MaterialPipelineJobKey = "Material Type and Pipeline Builder";
        const char* MaterialTypeBuilder::FinalMaterialTypeJobKey = "Material Type Builder";

        AZStd::string MaterialTypeBuilder::GetBuilderSettingsFingerprint() const
        {
            return AZStd::string::format("[ShouldOutputAllPropertiesMaterial=%d]", ShouldOutputAllPropertiesMaterial());
        }

        void MaterialTypeBuilder::RegisterBuilder()
        {
            AssetBuilderSDK::AssetBuilderDesc materialBuilderDescriptor;
            materialBuilderDescriptor.m_name = "Material Type Builder";
            materialBuilderDescriptor.m_version = 2; // Added material pipeline concept
            materialBuilderDescriptor.m_patterns.push_back(AssetBuilderSDK::AssetBuilderPattern("*.materialtype", AssetBuilderSDK::AssetBuilderPattern::PatternType::Wildcard));
            materialBuilderDescriptor.m_busId = azrtti_typeid<MaterialTypeBuilder>();
            materialBuilderDescriptor.m_createJobFunction = AZStd::bind(&MaterialTypeBuilder::CreateJobs, this, AZStd::placeholders::_1, AZStd::placeholders::_2);
            materialBuilderDescriptor.m_processJobFunction = AZStd::bind(&MaterialTypeBuilder::ProcessJob, this, AZStd::placeholders::_1, AZStd::placeholders::_2);

            materialBuilderDescriptor.m_analysisFingerprint = GetBuilderSettingsFingerprint();

            BusConnect(materialBuilderDescriptor.m_busId);

            AssetBuilderSDK::AssetBuilderBus::Broadcast(&AssetBuilderSDK::AssetBuilderBus::Handler::RegisterBuilderInformation, materialBuilderDescriptor);

            LoadMaterialPipelines();
        }

        MaterialTypeBuilder::~MaterialTypeBuilder()
        {
            BusDisconnect();
        }

        bool MaterialTypeBuilder::ShouldOutputAllPropertiesMaterial() const
        {
            // Enable this setting to generate a default source material file containing an explicit list of all properties and their
            // default values. This is primarily used by artists and developers scraping data from the materials and should only be enabled
            // as needed by those users.
            bool value = false;
            if (auto settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry != nullptr)
            {
                settingsRegistry->Get(value, "/O3DE/Atom/RPI/MaterialTypeBuilder/CreateAllPropertiesMaterial");
            }
            return value;
        }

        void MaterialTypeBuilder::LoadMaterialPipelines()
        {
            if (auto settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry != nullptr)
            {
                AZStd::vector<AZStd::string> materialPipelineFiles;
                settingsRegistry->GetObject(materialPipelineFiles, "/O3DE/Atom/RPI/MaterialPipelineFiles");

                for (const AZStd::string& file : materialPipelineFiles)
                {
                    //AZStd::string materialPipelineName = GetMaterialPipelineName(file);
                    //
                    //// We prevent redundant pipeline names just to simplify the code in ProcessPipelineJob where generated shader filenames could otherwise collide. 
                    //
                    //if (materialPipelineName == PipelineNameForCommonShaders)
                    //{
                    //    AZ_Error(MaterialTypeBuilderName, false, "Material pipeline '%s' cannot be used because the name conflicts with the reserved name '%s'.", file.c_str(), PipelineNameForCommonShaders);
                    //    continue;
                    //}
                    //
                    //for (const auto& [otherMaterialPipelineFilePath, materialPipeline] : m_materialPipelines)
                    //{
                    //    AZ_UNUSED(materialPipeline);
                    //    if (materialPipelineName == GetMaterialPipelineName(otherMaterialPipelineFilePath))
                    //    {
                    //        AZ_Error(MaterialTypeBuilderName, false, "Material pipeline '%s' cannot be used because the name conflicts with the reserved name '%s'.", file.c_str(), PipelineNameForCommonShaders);
                    //        continue;
                    //    }
                    //}

                    auto loadResult = MaterialUtils::LoadMaterialPipelineSourceData(file.c_str());
                    if (!loadResult.IsSuccess())
                    {
                        AZ_Error(MaterialTypeBuilderName, false, "Failed to load '%s'.", file.c_str());
                        continue;
                    }

                    m_materialPipelines.emplace(file, loadResult.TakeValue());
                }
            }
        }

        AZStd::string MaterialTypeBuilder::GetMaterialPipelineName(const AZStd::string& materialPipelineFile) const
        {
            return AZ::IO::Path{materialPipelineFile}.Stem().Native();
        }

        void MaterialTypeBuilder::CreateJobs(const AssetBuilderSDK::CreateJobsRequest& request, AssetBuilderSDK::CreateJobsResponse& response) const
        {
            if (m_isShuttingDown)
            {
                response.m_result = AssetBuilderSDK::CreateJobsResultCode::ShuttingDown;
                return;
            }

            AZStd::string fullSourcePath;
            AzFramework::StringFunc::Path::ConstructFull(request.m_watchFolder.data(), request.m_sourceFile.data(), fullSourcePath, true);

            MaterialUtils::ImportedJsonFiles importedJsonFiles;
            auto materialTypeSourceData = MaterialUtils::LoadMaterialTypeSourceData(fullSourcePath, nullptr, &importedJsonFiles);

            if (!materialTypeSourceData.IsSuccess())
            {
                return;
            }

            // There should only be imported files when IsAbstractFormat() is false, but it doesn't hurt anything to check the imported file list in either case.
            for (auto& importedJsonFile : importedJsonFiles)
            {
                AssetBuilderSDK::SourceFileDependency sourceDependency;
                sourceDependency.m_sourceFileDependencyPath = importedJsonFile;
                response.m_sourceFileDependencyList.push_back(sourceDependency);
            }

            if (materialTypeSourceData.GetValue().IsAbstractFormat())
            {
                CreatePipelineJobs(request, response, materialTypeSourceData.GetValue());
            }
            else
            {
                CreateFinalJobs(request, response, materialTypeSourceData.GetValue());
            }
        }

        void MaterialTypeBuilder::CreatePipelineJobs(const AssetBuilderSDK::CreateJobsRequest& /*request*/, AssetBuilderSDK::CreateJobsResponse& response, const MaterialTypeSourceData& /*materialTypeSourceData*/) const
        {
            AssetBuilderSDK::JobDescriptor outputJobDescriptor;
            outputJobDescriptor.m_jobKey = MaterialPipelineJobKey;
            outputJobDescriptor.SetPlatformIdentifier(AssetBuilderSDK::CommonPlatformName);
            response.m_createJobOutputs.push_back(outputJobDescriptor);

            // Note that we don't need to add the .materialtype's materialShaderCode as a source dependency because it's just going to be #included into the final azsl file.

            // Add dependencies on each material pipeline, since the output of this builder is a combination of the .materialtype data and the .materialpipeline data.
            for (const auto& [materialPipelineFilePath, materialPipeline] : m_materialPipelines)
            {
                // This comes from a central registry setting, so it must be a full path already.
                response.m_sourceFileDependencyList.push_back({});
                response.m_sourceFileDependencyList.back().m_sourceFileDependencyPath = materialPipelineFilePath;

                for (const MaterialPipelineSourceData::ShaderTemplate& shaderTemplate : materialPipeline.m_shaderTemplates)
                {
                    AZStd::vector<AZStd::string> possibleDependencies = RPI::AssetUtils::GetPossibleDepenencyPaths(materialPipelineFilePath, shaderTemplate.m_shader);
                    for (const AZStd::string& path : possibleDependencies)
                    {
                        response.m_sourceFileDependencyList.push_back({});
                        response.m_sourceFileDependencyList.back().m_sourceFileDependencyPath = path;
                    }

                    // We don't need to add m_azsli as a dependency because that will be #included into the final azsl file, so the shader asset builder
                    // will account for that dependency.
                }
            }

            response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
        }

        void MaterialTypeBuilder::CreateFinalJobs(const AssetBuilderSDK::CreateJobsRequest& request, AssetBuilderSDK::CreateJobsResponse& response, const MaterialTypeSourceData& materialTypeSourceData) const
        {
            // We'll build up this one JobDescriptor and reuse it to register each of the platforms
            AssetBuilderSDK::JobDescriptor outputJobDescriptor;
            outputJobDescriptor.m_jobKey = FinalMaterialTypeJobKey;
            outputJobDescriptor.m_additionalFingerprintInfo = GetBuilderSettingsFingerprint();

            for (auto& shader : materialTypeSourceData.m_shaderCollection)
            {
                MaterialBuilderUtils::AddPossibleDependencies(request.m_sourceFile,
                    shader.m_shaderFilePath,
                    "Shader Asset",
                    outputJobDescriptor.m_jobDependencyList,
                    response.m_sourceFileDependencyList,
                    false,
                    0);
            }

            auto addFunctorDependencies = [&outputJobDescriptor, &request, &response](const AZStd::vector<Ptr<MaterialFunctorSourceDataHolder>>& functors)
            {
                for (auto& functor : functors)
                {
                    const auto& dependencies = functor->GetActualSourceData()->GetAssetDependencies();

                    for (const MaterialFunctorSourceData::AssetDependency& dependency : dependencies)
                    {
                        MaterialBuilderUtils::AddPossibleDependencies(request.m_sourceFile,
                            dependency.m_sourceFilePath,
                            dependency.m_jobKey.c_str(),
                            outputJobDescriptor.m_jobDependencyList,
                            response.m_sourceFileDependencyList);
                    }
                }
            };

            addFunctorDependencies(materialTypeSourceData.m_materialFunctorSourceData);

            materialTypeSourceData.EnumeratePropertyGroups([addFunctorDependencies](const MaterialTypeSourceData::PropertyGroupStack& propertyGroupStack)
                {
                    addFunctorDependencies(propertyGroupStack.back()->GetFunctors());
                    return true;
                });

            materialTypeSourceData.EnumerateProperties(
                [&request, &response, &outputJobDescriptor](const MaterialTypeSourceData::PropertyDefinition* property, const MaterialNameContext&)
                {
                    if (property->m_dataType == MaterialPropertyDataType::Image && MaterialUtils::LooksLikeImageFileReference(property->m_value))
                    {
                        MaterialBuilderUtils::AddPossibleImageDependencies(
                            request.m_sourceFile,
                            property->m_value.GetValue<AZStd::string>(),
                            outputJobDescriptor.m_jobDependencyList,
                            response.m_sourceFileDependencyList);
                    }
                    return true;
                });

            // Create the output jobs for each platform
            for (const AssetBuilderSDK::PlatformInfo& platformInfo : request.m_enabledPlatforms)
            {
                outputJobDescriptor.SetPlatformIdentifier(platformInfo.m_identifier.c_str());

                for (auto& jobDependency : outputJobDescriptor.m_jobDependencyList)
                {
                    jobDependency.m_platformIdentifier = platformInfo.m_identifier;
                }

                response.m_createJobOutputs.push_back(outputJobDescriptor);
            }

            response.m_result = AssetBuilderSDK::CreateJobsResultCode::Success;
        }

        AZ::Data::Asset<MaterialTypeAsset> CreateMaterialTypeAsset(AZStd::string_view materialTypeSourceFilePath, rapidjson::Document& json)
        {
            auto materialType = MaterialUtils::LoadMaterialTypeSourceData(materialTypeSourceFilePath, &json);

            if (!materialType.IsSuccess())
            {
                return  {};
            }

            auto materialTypeAssetOutcome = materialType.GetValue().CreateMaterialTypeAsset(Uuid::CreateRandom(), materialTypeSourceFilePath, true);
            if (!materialTypeAssetOutcome.IsSuccess())
            {
                return  {};
            }

            return materialTypeAssetOutcome.GetValue();
        }
        
        void MaterialTypeBuilder::ProcessJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response) const
        {
            AssetBuilderSDK::JobCancelListener jobCancelListener(request.m_jobId);

            if (jobCancelListener.IsCancelled())
            {
                response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Cancelled;
                return;
            }
            if (m_isShuttingDown)
            {
                response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Cancelled;
                return;
            }

            if (request.m_jobDescription.m_jobKey == MaterialPipelineJobKey)
            {
                ProcessPipelineJob(request, response);
            }
            else if (request.m_jobDescription.m_jobKey == FinalMaterialTypeJobKey)
            {
                ProcessFinalJob(request, response);
            }
            else
            {
                AZ_Error(MaterialTypeBuilderName, false, "Invalid material type job key.");
            }
        }

        void MaterialTypeBuilder::ProcessPipelineJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response) const
        {
            AZStd::string materialTypeName = AZ::IO::Path{request.m_sourceFile}.Stem().Native();

            AZ::u32 nextProductSubID = 0;

            auto materialTypeLoadResult = MaterialUtils::LoadMaterialTypeSourceData(request.m_fullPath);
            if (!materialTypeLoadResult.IsSuccess())
            {
                AZ_Error(MaterialTypeBuilderName, false, "Failed to load material type file.");
                return;
            }
            MaterialTypeSourceData materialType = materialTypeLoadResult.TakeValue();
            materialType.m_shaderCollection.clear();

            // Some shader templates may be reused by multiple pipelines, so first collect a full picture of all the dependencies
            AZStd::unordered_map<MaterialPipelineSourceData::ShaderTemplate, AZStd::vector<AZStd::string/*materialPipielineName*/>> shaderTemplateReferences;
            {
                bool missingSomeFiles = false;

                for (const auto& [materialPipelineFilePath, materialPipeline] : m_materialPipelines)
                {
                    AZStd::string materialPipelineName = GetMaterialPipelineName(materialPipelineFilePath);

                    // TODO: Eventually we'll use a script and inputs from the material type to decide which shader templates need to be used
                    for (const MaterialPipelineSourceData::ShaderTemplate& shaderTemplate : materialPipeline.m_shaderTemplates)
                    {
                        // We need to normalize the content of the ShaderTemplate structure since it will be used as the key in the map.
                        // We also check for missing files now, where the original relative path is available for use in the error message.

                        MaterialPipelineSourceData::ShaderTemplate normalizedShaderTemplate = shaderTemplate;

                        auto resolveTemplateFilePathReference = [&missingSomeFiles](const AZStd::string& materialPipelineFilePath, AZStd::string& templateFilePath)
                        {
                            AZStd::string resolvedFilePath = AssetUtils::ResolvePathReference(materialPipelineFilePath, templateFilePath);

                            if (!AZ::IO::LocalFileIO::GetInstance()->Exists(resolvedFilePath.c_str()))
                            {
                                AZ_Error(MaterialTypeBuilderName, false, "File is missing: '%s'", templateFilePath.c_str());
                                missingSomeFiles = true;
                            }

                            templateFilePath = resolvedFilePath;
                        };

                        resolveTemplateFilePathReference(materialPipelineFilePath, normalizedShaderTemplate.m_shader);
                        resolveTemplateFilePathReference(materialPipelineFilePath, normalizedShaderTemplate.m_azsli);

                        shaderTemplateReferences[normalizedShaderTemplate].push_back(materialPipelineName.c_str());
                    }
                }

                if (missingSomeFiles)
                {
                    return;
                }
            }

            // The new material type will no longer be abstract, we remove the reference to the partial
            // material shader code and will replace it below with a concrete shader asset list.
            const AZStd::string materialShaderCode = materialType.m_materialShaderCode;
            materialType.m_materialShaderCode.clear();

            // Generate the required shaders
            for (const auto& [shaderTemplate, materialPipelineList] : shaderTemplateReferences)
            {
                AZStd::string materialPipelineName;

                if (materialPipelineList.size() == 1)
                {
                    materialPipelineName = *materialPipelineList.begin();
                }
                else if(materialPipelineList.size() > 1)
                {
                    // Multiple material pipelines reference the same shader, so it should have a generic common name.
                    materialPipelineName = PipelineNameForCommonShaders;
                }
                else
                {
                    AZ_Assert(false, "There should be at least one material pipeline referencing the shader");
                    return;
                }

                //auto shaderTemplateOutcome = ShaderUtils::LoadShaderDataJson(shaderTemplatePath.c_str());
                //if (!shaderTemplateOutcome.IsSuccess())
                //{
                //    AZ_Error(MaterialTypeBuilderName, false, "Failed to parse shader template '%s': %s",
                //        shaderTemplatePath.c_str(), shaderTemplateOutcome.GetError().c_str());
                //    return;
                //}
                //ShaderSourceData shaderSourceData = shaderTemplateOutcome.TakeValue();

                auto shaderFile = AZ::Utils::ReadFile<AZStd::string>(shaderTemplate.m_shader);
                if (!shaderFile.IsSuccess())
                {
                    AZ_Error(MaterialTypeBuilderName, false, "Failed to load shader template file '%s'. %s", shaderTemplate.m_shader.c_str(), shaderFile.GetError().c_str());
                    return;
                }

                // At this point shaderTemplate.m_azsli should be an absolute path due to ResolvePathReference() being called above.
                // It might be better for the include path to be relative to the generated .shader file path in the intermediate cache,
                // so the project could be renamed or moved without having to rebuild the cache. But there's a good chance that moving
                // the project would require a rebuild of the cache anyway.
                AZStd::string includeAzslFilePath = shaderTemplate.m_azsli;

                // Intermediate azsl file

                AZStd::string generatedAzsl = AZStd::string::format(
                    "// This code was generated by %s. Do not modify.\n"
                    "#include <%s>\n",
                    MaterialTypeBuilderName,
                    includeAzslFilePath.c_str());

                if (!materialShaderCode.empty())
                {
                    generatedAzsl += AZStd::string::format("#include <%s>\n", materialShaderCode.c_str());
                }

                generatedAzsl +=
                    " \n"
                    "#if !MaterialFunction_AdjustLocalPosition_DEFINED                                       \n"
                    "    void MaterialFunction_AdjustLocalPosition(inout float3 localPosition) {}            \n"
                    "    #define MaterialFunction_AdjustLocalPosition_DEFINED 1                              \n"
                    "#endif                                                                                  \n"
                    "                                                                                        \n"
                    "#if !MaterialFunction_AdjustWorldPosition_DEFINED                                       \n"
                    "    void MaterialFunction_AdjustWorldPosition(inout float3 worldPosition) {}            \n"
                    "    #define MaterialFunction_AdjustWorldPosition_DEFINED 1                              \n"
                    "#endif                                                                                  \n"
                    "                                                                                        \n"
                    "#if !MaterialFunction_AdjustSurface_DEFINED && MATERIALPIPELINE_SHADER_HAS_PIXEL_STAGE  \n"
                    "    void MaterialFunction_AdjustSurface(inout Surface outSurface) {}                    \n"
                    "    #define MaterialFunction_AdjustSurface_DEFINED 1                                    \n"
                    "#endif                                                                                  \n"
                    ;

                AZ::IO::Path shaderName = shaderTemplate.m_shader;
                shaderName = shaderName.Filename(); // Removes the folder path
                shaderName = shaderName.ReplaceExtension(""); // This will remove the ".template" extension
                shaderName = shaderName.ReplaceExtension(""); // This will remove the ".shader" extension

                AZ::IO::Path outputAzslFilePath = request.m_tempDirPath;
                outputAzslFilePath /= AZStd::string::format("%s_%s_%s.azsl", materialTypeName.c_str(), materialPipelineName.c_str(), shaderName.c_str());

                if (AZ::Utils::WriteFile(generatedAzsl, outputAzslFilePath.c_str()).IsSuccess())
                {
                    AssetBuilderSDK::JobProduct product;
                    product.m_outputFlags = AssetBuilderSDK::ProductOutputFlags::IntermediateAsset;
                    product.m_dependenciesHandled = true;
                    product.m_productFileName = outputAzslFilePath.String();
                    product.m_productSubID = nextProductSubID++;
                    response.m_outputProducts.push_back(AZStd::move(product));
                }
                else
                {
                    AZ_Error(MaterialTypeBuilderName, false, "Failed to write intermediate azsl file.");
                    return;
                }

                // Intermediate shader file

                AZStd::string azslFileReference = AZ::IO::Path{outputAzslFilePath.Filename()}.c_str();
                AZStd::to_lower(azslFileReference.begin(), azslFileReference.end());
                AzFramework::StringFunc::Replace(shaderFile.GetValue(), "INSERT_AZSL_HERE", azslFileReference.c_str());

                AZ::IO::Path outputShaderFilePath = request.m_tempDirPath;
                outputShaderFilePath /= AZStd::string::format("%s_%s_%s.shader", materialTypeName.c_str(), materialPipelineName.c_str(), shaderName.c_str());

                if (AZ::Utils::WriteFile(shaderFile.GetValue(), outputShaderFilePath.c_str()).IsSuccess())
                {
                    AssetBuilderSDK::JobProduct product;
                    product.m_outputFlags = AssetBuilderSDK::ProductOutputFlags::IntermediateAsset;
                    product.m_productFileName = outputShaderFilePath.String();
                    product.m_productSubID = nextProductSubID++;
                    response.m_outputProducts.push_back(AZStd::move(product));
                }
                else
                {
                    AZ_Error(MaterialTypeBuilderName, false, "Failed to write intermediate shader file.");
                    return;
                }

                // Add shader to intermediate material type

                materialType.m_shaderCollection.push_back({});
                materialType.m_shaderCollection.back().m_shaderFilePath = AZ::IO::Path{outputShaderFilePath.Filename()}.c_str();
            }

            AZ::IO::Path outputMaterialTypeFilePath = request.m_tempDirPath;
            // The "_final" postfix is necessary to prevent an infinite loop in the AP that would occur when the intermediate
            // file has the same name as the original file.
            outputMaterialTypeFilePath /= AZStd::string::format("%s_final.materialtype", materialTypeName.c_str());

            AZ_Assert(!materialType.IsAbstractFormat(),
                "The output material type must not use the abstract format, this will likely causing the '%s' job to run in an infinite loop.", MaterialPipelineJobKey);

            if (JsonUtils::SaveObjectToFile(outputMaterialTypeFilePath.String(), materialType))
            {
                AssetBuilderSDK::JobProduct product;
                product.m_outputFlags = AssetBuilderSDK::ProductOutputFlags::IntermediateAsset;
                product.m_productFileName = outputMaterialTypeFilePath.String();
                product.m_productSubID = nextProductSubID++;
                response.m_outputProducts.push_back(AZStd::move(product));
            }
            else
            {
                AZ_Error(MaterialTypeBuilderName, false, "Failed to write intermediate material type file.");
                return;
            }

            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
        }

        void MaterialTypeBuilder::ProcessFinalJob(const AssetBuilderSDK::ProcessJobRequest& request, AssetBuilderSDK::ProcessJobResponse& response) const
        {
            AZStd::string fullSourcePath;
            AzFramework::StringFunc::Path::ConstructFull(request.m_watchFolder.data(), request.m_sourceFile.data(), fullSourcePath, true);

            auto loadOutcome = JsonSerializationUtils::ReadJsonFile(fullSourcePath, AZ::RPI::JsonUtils::DefaultMaxFileSize);
            if (!loadOutcome.IsSuccess())
            {
                AZ_Error(MaterialTypeBuilderName, false, "Failed to load material file: %s", loadOutcome.GetError().c_str());
                return;
            }

            rapidjson::Document& document = loadOutcome.GetValue();

            AZStd::string materialProductPath;
            AZStd::string fileName;
            AzFramework::StringFunc::Path::GetFileName(request.m_sourceFile.c_str(), fileName);
            AzFramework::StringFunc::Path::ReplaceExtension(fileName, MaterialTypeAsset::Extension);

            AzFramework::StringFunc::Path::ConstructFull(request.m_tempDirPath.c_str(), fileName.c_str(), materialProductPath, true);

            AZ::Data::Asset<MaterialTypeAsset> materialTypeAsset;

            {
                AZ_TraceContext("Product", fileName);
                AZ_TracePrintf("MaterialTypeBuilder", AZStd::string::format("Producing %s...", fileName.c_str()).c_str());

                // Load the material type file and create the MaterialTypeAsset object
                materialTypeAsset = CreateMaterialTypeAsset(fullSourcePath, document);

                if (!materialTypeAsset)
                {
                    // Errors will have been reported above
                    return;
                }

                // [ATOM-13190] Change this back to ST_BINARY. It's ST_XML temporarily for debugging.
                if (!AZ::Utils::SaveObjectToFile(materialProductPath, AZ::DataStream::ST_XML, materialTypeAsset.Get()))
                {
                    AZ_Error(MaterialTypeBuilderName, false, "Failed to save material type to file '%s'!", materialProductPath.c_str());
                    return;
                }

                AssetBuilderSDK::JobProduct jobProduct;
                if (!AssetBuilderSDK::OutputObject(
                    materialTypeAsset.Get(),
                    materialProductPath,
                    azrtti_typeid<RPI::MaterialTypeAsset>(),
                    (u32)MaterialTypeProductSubId::MaterialTypeAsset,
                    jobProduct))
                {
                    AZ_Error(MaterialTypeBuilderName, false, "Failed to output product dependencies.");
                    return;
                }

                response.m_outputProducts.push_back(AZStd::move(jobProduct));
            }

            if (ShouldOutputAllPropertiesMaterial())
            {
                AZStd::string defaultMaterialFileName;
                AzFramework::StringFunc::Path::GetFileName(request.m_sourceFile.c_str(), defaultMaterialFileName);
                defaultMaterialFileName += "_AllProperties.material";

                AZStd::string defaultMaterialFilePath;
                AzFramework::StringFunc::Path::ConstructFull(request.m_tempDirPath.c_str(), defaultMaterialFileName.c_str(), defaultMaterialFilePath, true);

                AZ_TraceContext("Product", defaultMaterialFileName);
                AZ_TracePrintf("MaterialTypeBuilder", AZStd::string::format("Producing %s...", defaultMaterialFileName.c_str()).c_str());

                MaterialSourceData allPropertyDefaultsMaterial = MaterialSourceData::CreateAllPropertyDefaultsMaterial(materialTypeAsset, request.m_sourceFile);

                if (!JsonUtils::SaveObjectToFile(defaultMaterialFilePath, allPropertyDefaultsMaterial))
                {
                    AZ_Warning(MaterialTypeBuilderName, false, "Failed to save material reference file '%s'!", defaultMaterialFilePath.c_str());
                }
                else
                {
                    AssetBuilderSDK::JobProduct defaultMaterialFileProduct;
                    defaultMaterialFileProduct.m_dependenciesHandled = true; // This product is only for reference, not used at runtime
                    defaultMaterialFileProduct.m_productFileName = defaultMaterialFilePath;
                    defaultMaterialFileProduct.m_productSubID = (u32)MaterialTypeProductSubId::AllPropertiesMaterialSourceFile;
                    response.m_outputProducts.push_back(AZStd::move(defaultMaterialFileProduct));
                }
            }

            response.m_resultCode = AssetBuilderSDK::ProcessJobResult_Success;
        }

        void MaterialTypeBuilder::ShutDown()
        {
            m_isShuttingDown = true;
        }
    } // namespace RPI
} // namespace AZ
