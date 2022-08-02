/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <API/ToolsApplicationAPI.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/Settings/SettingsRegistryMergeUtils.h>
#include <AzCore/StringFunc/StringFunc.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/Viewport/CameraInput.h>
#include <AzFramework/Viewport/CameraState.h>
#include <AzToolsFramework/API/EditorCameraBus.h>
#include <AzToolsFramework/Entity/PrefabEditorEntityOwnershipInterface.h>
#include <AzToolsFramework/Viewport/ViewportMessages.h>
#include <Entity/PrefabEditorEntityOwnershipInterface.h>
#include <Prefab/PrefabSystemComponentInterface.h>
#include <Viewport/LocalViewBookmarkComponent.h>
#include <Viewport/LocalViewBookmarkLoader.h>

namespace AzToolsFramework
{
    static constexpr const char* ViewBookmarksRegistryPath = "/O3DE/ViewBookmarks";
    static constexpr const char* LocalBookmarksKey = "LocalBookmarks";
    static constexpr const char* LastKnownLocationKey = "LastKnownLocation";

    // temporary value until there is UI to expose the fields
    static constexpr int DefaultViewBookmarkCount = 12;

    struct ViewBookmarkVisitor : AZ::SettingsRegistryInterface::Visitor
    {
        ViewBookmarkVisitor()
            : m_viewBookmarksKey{ ViewBookmarksRegistryPath } {};

        AZ::SettingsRegistryInterface::VisitResponse Traverse(
            AZStd::string_view path,
            [[maybe_unused]] AZStd::string_view valueName,
            AZ::SettingsRegistryInterface::VisitAction action,
            [[maybe_unused]] AZ::SettingsRegistryInterface::Type type) override
        {
            if (action == AZ::SettingsRegistryInterface::VisitAction::Begin)
            {
                // strip off the last json pointer key from the path and if it matches the view bookmark key then add an entry
                // to the view bookmark map
                AZStd::optional<AZStd::string_view> localBookmarksId = AZ::StringFunc::TokenizeLast(path, "/");
                if (path == m_viewBookmarksKey && localBookmarksId.has_value() && !localBookmarksId->empty())
                {
                    if (const auto existingBookmarkEntry = m_bookmarkMap.find(localBookmarksId.value());
                        existingBookmarkEntry == m_bookmarkMap.end())
                    {
                        m_bookmarkMap.insert(AZStd::make_pair(localBookmarksId.value(), AZStd::vector<ViewBookmark>{}));
                    }
                }
            }

            return AZ::SettingsRegistryInterface::VisitResponse::Continue;
        }

        using AZ::SettingsRegistryInterface::Visitor::Visit;
        void Visit(AZStd::string_view path, AZStd::string_view valueIndex, AZ::SettingsRegistryInterface::Type, double value) override
        {
            AZ::StringFunc::TokenizeLast(path, "/");
            AZStd::optional<AZStd::string_view> dataType = AZ::StringFunc::TokenizeLast(path, "/");
            AZStd::optional<AZStd::string_view> bookmarkIndexStr = AZ::StringFunc::TokenizeLast(path, "/");
            AZStd::optional<AZStd::string_view> bookmarkType;
            if (bookmarkIndexStr == LastKnownLocationKey)
            {
                bookmarkType = bookmarkIndexStr;
            }
            else
            {
                // differentiate between local Bookmarks and LastKnownLocation
                bookmarkType = AZ::StringFunc::TokenizeLast(path, "/");
            }

            if (AZStd::optional<AZStd::string_view> localBookmarksId = AZ::StringFunc::TokenizeLast(path, "/");
                localBookmarksId.has_value() && path == m_viewBookmarksKey && !localBookmarksId->empty())
            {
                auto setVec3Fn = [value](AZ::Vector3& inout, int currentIndex)
                {
                    switch (currentIndex)
                    {
                    case 0:
                        inout.SetX(aznumeric_cast<float>(value));
                        break;
                    case 1:
                        inout.SetY(aznumeric_cast<float>(value));
                        break;
                    case 2:
                        inout.SetZ(aznumeric_cast<float>(value));
                        break;
                    default:
                        AZ_Error(
                            "LocalViewBookmarkLoader", false, "Trying to set an invalid index in a Vector3, index = %d", currentIndex);
                        break;
                    }
                };

                if (bookmarkType == LastKnownLocationKey)
                {
                    if (!m_lastKnownLocation.has_value())
                    {
                        m_lastKnownLocation.emplace(ViewBookmark{});
                    }

                    const int currentIndex = AZStd::stoi(AZStd::string(valueIndex));
                    if (dataType == "Position")
                    {
                        setVec3Fn(m_lastKnownLocation->m_position, currentIndex);
                    }
                    else if (dataType == "Rotation")
                    {
                        setVec3Fn(m_lastKnownLocation->m_rotation, currentIndex);
                    }
                }
                else if (bookmarkType == LocalBookmarksKey)
                {
                    if (auto existingBookmarkEntry = m_bookmarkMap.find(localBookmarksId.value());
                        existingBookmarkEntry != m_bookmarkMap.end())
                    {
                        AZStd::vector<ViewBookmark>& bookmarks = existingBookmarkEntry->second;
                        // if it is the first bookmark and it is the Position data it means it is the first one
                        // and we have to create the Bookmark.
                        if (valueIndex == "0" && dataType == "Position")
                        {
                            ViewBookmark bookmark;
                            bookmark.m_position.SetX(aznumeric_cast<float>(value));
                            bookmarks.push_back(bookmark);
                        }
                        else
                        {
                            const int bookmarkIndex = AZStd::stoi(AZStd::string(bookmarkIndexStr->data()));
                            AZ_Assert(bookmarkIndex >= 0 && bookmarkIndex < bookmarks.size(), "Bookmark index is out of bounds");
                            ViewBookmark& bookmark = bookmarks[bookmarkIndex];
                            const int currentIndex = AZStd::stoi(AZStd::string(valueIndex));

                            if (dataType == "Position")
                            {
                                setVec3Fn(bookmark.m_position, currentIndex);
                            }
                            else if (dataType == "Rotation")
                            {
                                setVec3Fn(bookmark.m_rotation, currentIndex);
                            }
                        }
                    }
                }
            }
        };

        AZ::SettingsRegistryInterface::FixedValueString m_viewBookmarksKey;
        AZStd::unordered_map<AZStd::string, AZStd::vector<ViewBookmark>> m_bookmarkMap;
        AZStd::optional<ViewBookmark> m_lastKnownLocation;
    };

    static AZ::IO::FixedMaxPath LocalBookmarkFilePath(const AZStd::string& bookmarkFileName)
    {
        return AZ::IO::FixedMaxPath(AZ::Utils::GetProjectPath()) / "user/Registry/ViewBookmarks/" / bookmarkFileName;
    }

    // returns true if the file already existed, false otherwise
    static bool EnsureLocalBookmarkFileExists(const AZStd::string& bookmarkFileName)
    {
        const auto localBookmarkFilePath = LocalBookmarkFilePath(bookmarkFileName);

        AZ::IO::SystemFile outputFile;
        // if the file doesn't exist but we got a valid filename from source control then let's create the file
        const auto configurationMode = !outputFile.Exists(localBookmarkFilePath.c_str())
            ? AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_CREATE_PATH | AZ::IO::SystemFile::SF_OPEN_READ_WRITE
            : AZ::IO::SystemFile::SF_OPEN_READ_ONLY;

        outputFile.Open(localBookmarkFilePath.c_str(), configurationMode);

        return outputFile.Length() > 0;
    }

    static AZStd::string LocalViewBookmarkSetRegPath(const AZStd::string& bookmarkFileName)
    {
        return AZStd::string::format("/%s/LocalBookmarks", bookmarkFileName.c_str());
    }

    static AZStd::string LocalViewBookmarkSetRegPathAtIndex(const AZStd::string& bookmarkFileName, const int index)
    {
        return AZStd::string::format("%s/%i", LocalViewBookmarkSetRegPath(bookmarkFileName).c_str(), index);
    }

    static AZStd::string LastKnownLocationSetRegPath(const AZStd::string& bookmarkFileName)
    {
        return AZStd::string::format("/%s/LastKnownLocation", bookmarkFileName.c_str());
    }

    static AZStd::string GenerateBookmarkFileName()
    {
        auto* prefabEditorEntityOwnershipInterface = AZ::Interface<PrefabEditorEntityOwnershipInterface>::Get();
        AZ_Assert(prefabEditorEntityOwnershipInterface != nullptr, "PrefabEditorEntityOwnershipInterface is not found.");
        Prefab::TemplateId rootPrefabTemplateId = prefabEditorEntityOwnershipInterface->GetRootPrefabTemplateId();

        if (rootPrefabTemplateId == Prefab::InvalidTemplateId)
        {
            return AZStd::string();
        }

        auto* prefabSystemComponent = AZ::Interface<Prefab::PrefabSystemComponentInterface>::Get();
        AZ_Assert(
            prefabSystemComponent != nullptr,
            "Prefab System Component Interface could not be found. "
            "It is a requirement for the LocalViewBookmarkLoader class. "
            "Check that it is being correctly initialized.");

        const auto prefabTemplate = prefabSystemComponent->FindTemplate(rootPrefabTemplateId);
        const AZStd::string prefabTemplateName = prefabTemplate->get().GetFilePath().Filename().Stem().Native();

        // to generate the file name in which we will store the view Bookmarks we use the name of the prefab + the timestamp
        // e.g. LevelName_1639763579377.setreg
        const AZStd::chrono::system_clock::time_point now = AZStd::chrono::system_clock::now();
        return AZStd::string::format("%s_%llu.setreg", prefabTemplateName.c_str(), now.time_since_epoch().count());
    }

    static AZ::SettingsRegistryMergeUtils::DumperSettings CreateDumperSettings(const AZStd::string& bookmarkKey)
    {
        AZ::SettingsRegistryMergeUtils::DumperSettings dumperSettings;
        dumperSettings.m_prettifyOutput = true;
        dumperSettings.m_includeFilter = [bookmarkKey](AZStd::string_view path)
        {
            AZStd::string_view o3dePrefixPath(bookmarkKey);
            return o3dePrefixPath.starts_with(path.substr(0, o3dePrefixPath.size()));
        };
        return dumperSettings;
    }

    void LocalViewBookmarkLoader::RegisterViewBookmarkInterface()
    {
        AZ::Interface<ViewBookmarkInterface>::Register(this);
        AZ::Interface<ViewBookmarkPersistInterface>::Register(this);

        m_streamWriteFn = [](const AZStd::string& localBookmarksFileName, const AZStd::string stringBuffer,
                             AZStd::function<bool(AZ::IO::GenericStream&, const AZStd::string&)> writeFn)
        {
            constexpr auto configurationMode =
                AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_CREATE_PATH | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY;

            // resolve path to user project folder
            const auto localBookmarkFilePath = LocalBookmarkFilePath(localBookmarksFileName);

            AZ::IO::SystemFile outputFile;
            outputFile.Open(localBookmarkFilePath.c_str(), configurationMode);

            AZ::IO::SystemFileStream systemFileStream(&outputFile, false);
            const bool saved = writeFn(systemFileStream, stringBuffer);

            AZ_Warning(
                "LocalViewBookmarkLoader", saved, R"(Unable to save Local View Bookmark file to path "%s"\n)",
                localBookmarkFilePath.c_str());

            return saved;
        };

        m_streamReadFn = [](const AZStd::string localBookmarksFileName)
        {
            const auto localBookmarkFilePath = LocalBookmarkFilePath(localBookmarksFileName);
            const auto fileSize = AZ::IO::SystemFile::Length(localBookmarkFilePath.c_str());
            AZStd::vector<char> buffer(fileSize + 1);
            buffer[fileSize] = '\0';
            if (!AZ::IO::SystemFile::Read(localBookmarkFilePath.c_str(), buffer.data()))
            {
                return AZStd::vector<char>{};
            }
            return buffer;
        };

        m_fileExistsFn = [](const AZStd::string& localBookmarksFileName)
        {
            return EnsureLocalBookmarkFileExists(localBookmarksFileName);
        };
    }

    void LocalViewBookmarkLoader::UnregisterViewBookmarkInterface()
    {
        AZ::Interface<ViewBookmarkPersistInterface>::Unregister(this);
        AZ::Interface<ViewBookmarkInterface>::Unregister(this);
    }

    void LocalViewBookmarkLoader::WriteViewBookmarksSettingsRegistryToFile(const AZStd::string& localBookmarksFileName)
    {
        auto registry = AZ::SettingsRegistry::Get();
        if (!registry)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "Unable to access global settings registry. Editor Preferences cannot be saved");
            return;
        }

        const AZStd::string bookmarkKey = "/" + localBookmarksFileName;

        AZStd::string stringBuffer;
        AZ::IO::ByteContainerStream stringStream(&stringBuffer);
        if (!AZ::SettingsRegistryMergeUtils::DumpSettingsRegistryToStream(*registry, "", stringStream, CreateDumperSettings(bookmarkKey)))
        {
            AZ_Error(
                "LocalViewBookmarkLoader", false, R"(Unable to dump SettingsRegistry to stream "%s"\n)", localBookmarksFileName.c_str());
            return;
        }

        [[maybe_unused]] const bool saved = m_streamWriteFn(
            localBookmarksFileName, stringBuffer,
            [this](AZ::IO::GenericStream& genericStream, const AZStd::string& stringBuffer)
            {
                return Write(genericStream, stringBuffer);
            });

        AZ_Warning(
            "LocalViewBookmarkLoader", saved, R"(Unable to save Local View Bookmark file to path "%s"\n)", localBookmarksFileName.c_str());

        // once written to the desired file, remove the key from the settings registry
        registry->Remove(bookmarkKey + "/");
    }

    bool LocalViewBookmarkLoader::Write(AZ::IO::GenericStream& genericStream, const AZStd::string& stringBuffer)
    {
        return genericStream.Write(stringBuffer.size(), stringBuffer.data()) == stringBuffer.size();
    }

    bool LocalViewBookmarkLoader::SaveBookmarkAtIndex(const ViewBookmark& bookmark, const int index)
    {
        if (index < 0 || index >= DefaultViewBookmarkCount)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "Attempting to save bookmark at invalid index");
            return false;
        }

        auto registry = AZ::SettingsRegistry::Get();
        if (!registry)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "Unable to access global settings registry. Editor Preferences cannot be saved");
            return false;
        }

        const LocalViewBookmarkComponent* bookmarkComponent = FindOrCreateLocalViewBookmarkComponent();
        if (!bookmarkComponent)
        {
            AZ_Error("LocalViewBookmarkLoader", bookmarkComponent, "Couldn't find or create LocalViewBookmarkComponent.");
            return false;
        }

        const auto& localBookmarksFileName = bookmarkComponent->GetLocalBookmarksFileName();
        ReadViewBookmarksSettingsRegistryFromFile(localBookmarksFileName);

        // write to the settings registry
        const bool success = registry->SetObject(LocalViewBookmarkSetRegPathAtIndex(localBookmarksFileName, index), bookmark);

        if (!success)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "couldn't remove View Bookmark at index %d", index);
            return false;
        }

        // if we managed to add the bookmark
        WriteViewBookmarksSettingsRegistryToFile(localBookmarksFileName);

        return success;
    }

    bool LocalViewBookmarkLoader::SaveLastKnownLocation(const ViewBookmark& bookmark)
    {
        const LocalViewBookmarkComponent* bookmarkComponent = FindOrCreateLocalViewBookmarkComponent();
        if (!bookmarkComponent)
        {
            AZ_Error("LocalViewBookmarkLoader", bookmarkComponent, "Couldn't find or create LocalViewBookmarkComponent.");
            return false;
        }

        const auto localBookmarksFileName = bookmarkComponent->GetLocalBookmarksFileName();

        bool success = false;
        if (auto registry = AZ::SettingsRegistry::Get())
        {
            // write to the settings registry
            success = registry->SetObject(LastKnownLocationSetRegPath(localBookmarksFileName), bookmark);
        }

        if (!success)
        {
            AZ_Error(
                "LocalViewBookmarkLoader", false, "View Bookmark x=%.4f, y=%.4f, z=%.4f couldn't be saved", bookmark.m_position.GetX(),
                bookmark.m_position.GetY(), bookmark.m_position.GetZ());

            return false;
        }

        // if we managed to add the bookmark
        WriteViewBookmarksSettingsRegistryToFile(localBookmarksFileName);

        return success;
    }

    bool LocalViewBookmarkLoader::ReadViewBookmarksSettingsRegistryFromFile(const AZStd::string& localBookmarksFileName)
    {
        auto registry = AZ::SettingsRegistry::Get();
        if (!registry)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "Unable to access global settings registry. Editor Preferences cannot be saved");
            return false;
        }

        // merge the current view bookmark file into the settings registry
        if (registry->MergeSettings(
                m_streamReadFn(localBookmarksFileName), AZ::SettingsRegistryInterface::Format::JsonMergePatch, ViewBookmarksRegistryPath))
        {
            ViewBookmarkVisitor viewBookmarkVisitor;
            const bool visitedViewBookmarks = registry->Visit(
                viewBookmarkVisitor, AZStd::string::format("%s/%s", ViewBookmarksRegistryPath, localBookmarksFileName.c_str()));

            if (visitedViewBookmarks)
            {
                // update local cache
                m_localBookmarks = viewBookmarkVisitor.m_bookmarkMap.at(localBookmarksFileName);
                m_lastKnownLocation = viewBookmarkVisitor.m_lastKnownLocation;

                // update in-memory settings registry
                registry->SetObject(LocalViewBookmarkSetRegPath(localBookmarksFileName), m_localBookmarks);
                if (m_lastKnownLocation.has_value())
                {
                    registry->SetObject(LastKnownLocationSetRegPath(localBookmarksFileName), m_lastKnownLocation.value());
                }
            }

            return visitedViewBookmarks;
        }

        // remove cached local bookmarks if a view bookmark file could not be loaded
        m_localBookmarks.clear();
        m_lastKnownLocation.reset();

        return false;
    }

    AZStd::optional<ViewBookmark> LocalViewBookmarkLoader::LoadBookmarkAtIndex(const int index)
    {
        const LocalViewBookmarkComponent* bookmarkComponent = FindOrCreateLocalViewBookmarkComponent();
        if (!bookmarkComponent)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "Couldn't find or create LocalViewBookmarkComponent.");
            return AZStd::nullopt;
        }

        ReadViewBookmarksSettingsRegistryFromFile(bookmarkComponent->GetLocalBookmarksFileName());

        if (index >= 0 && index < m_localBookmarks.size())
        {
            return m_localBookmarks[index];
        }

        AZ_Error("LocalViewBookmarkLoader", false, "Couldn't load View Bookmark from file.");
        return AZStd::nullopt;
    }

    bool LocalViewBookmarkLoader::RemoveBookmarkAtIndex(const int index)
    {
        if (index < 0 || index >= DefaultViewBookmarkCount)
        {
            return false;
        }

        const LocalViewBookmarkComponent* bookmarkComponent = FindOrCreateLocalViewBookmarkComponent();
        if (!bookmarkComponent)
        {
            AZ_Error("LocalViewBookmarkLoader", bookmarkComponent, "Couldn't find or create LocalViewBookmarkComponent.");
            return false;
        }

        const auto& localBookmarksFileName = bookmarkComponent->GetLocalBookmarksFileName();

        bool success = false;
        if (auto registry = AZ::SettingsRegistry::Get())
        {
            success = registry->Remove(LocalViewBookmarkSetRegPathAtIndex(localBookmarksFileName, index));
        }

        if (!success)
        {
            AZ_Error("LocalViewBookmarkLoader", false, "Couldn't remove View Bookmark at index %d", index);
            return false;
        }

        // if we managed to remove the bookmark
        WriteViewBookmarksSettingsRegistryToFile(localBookmarksFileName);

        return success;
    }

    AZStd::optional<ViewBookmark> LocalViewBookmarkLoader::LoadLastKnownLocation()
    {
        const LocalViewBookmarkComponent* bookmarkComponent = FindOrCreateLocalViewBookmarkComponent();
        if (!bookmarkComponent)
        {
            AZ_Error("LocalViewBookmarkLoader", bookmarkComponent, "Couldn't find or create LocalViewBookmarkComponent.");
            return AZStd::nullopt;
        }

        ReadViewBookmarksSettingsRegistryFromFile(bookmarkComponent->GetLocalBookmarksFileName());

        return m_lastKnownLocation;
    }

    LocalViewBookmarkComponent* LocalViewBookmarkLoader::FindOrCreateLocalViewBookmarkComponent()
    {
        auto prefabEditorEntityOwnershipInterface = AZ::Interface<PrefabEditorEntityOwnershipInterface>::Get();
        if (!prefabEditorEntityOwnershipInterface)
        {
            return nullptr;
        }

        const AZ::EntityId containerEntityId = prefabEditorEntityOwnershipInterface->GetRootPrefabInstance()->get().GetContainerEntityId();
        if (!containerEntityId.IsValid())
        {
            return nullptr;
        }

        AZ::Entity* containerEntity = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(containerEntity, &AZ::ComponentApplicationBus::Events::FindEntity, containerEntityId);
        if (!containerEntity)
        {
            return nullptr;
        }

        LocalViewBookmarkComponent* bookmarkComponent = containerEntity->FindComponent<LocalViewBookmarkComponent>();
        if (!bookmarkComponent)
        {
            // if we didn't find a component then we add it and return it.
            containerEntity->Deactivate();
            bookmarkComponent = containerEntity->CreateComponent<LocalViewBookmarkComponent>();
            containerEntity->Activate();
        }

        AZ_Assert(bookmarkComponent, "Couldn't create LocalViewBookmarkComponent.");

        // record an undo step if a local view bookmark component is added and configured
        ScopedUndoBatch undoBatch("SetupLocalViewBookmarks");
        undoBatch.MarkEntityDirty(containerEntityId);

        // if the field is empty, we don't have a file linked to the prefab, so we create one and we save it in the component
        if (bookmarkComponent->GetLocalBookmarksFileName().empty())
        {
            bookmarkComponent->SetLocalBookmarksFileName(GenerateBookmarkFileName());
        }

        if (const auto localBookmarksFileName = bookmarkComponent->GetLocalBookmarksFileName(); !m_fileExistsFn(localBookmarksFileName))
        {
            auto registry = AZ::SettingsRegistry::Get();
            if (!registry)
            {
                AZ_Error("LocalViewBookmarkLoader", false, "Unable to access global settings registry.");
                return nullptr;
            }

            // initialize default locations to zero, this is a temporary solution to match the 12 locations of the legacy system
            // once there is a UI for the view bookmarks these lines should be removed
            if (const auto setRegPath = LocalViewBookmarkSetRegPath(localBookmarksFileName);
                !registry->SetObject(setRegPath, AZStd::vector<ViewBookmark>(DefaultViewBookmarkCount, ViewBookmark())))
            {
                AZ_Error("LocalViewBookmarkLoader", false, "SetObject for \"%s\" failed", setRegPath.c_str());
                return nullptr;
            }

            WriteViewBookmarksSettingsRegistryToFile(localBookmarksFileName);
        }

        return bookmarkComponent;
    }

    void LocalViewBookmarkLoader::OverrideStreamWriteFn(StreamWriteFn streamWriteFn)
    {
        m_streamWriteFn = AZStd::move(streamWriteFn);
    }

    void LocalViewBookmarkLoader::OverrideStreamReadFn(StreamReadFn streamReadFn)
    {
        m_streamReadFn = AZStd::move(streamReadFn);
    }

    void LocalViewBookmarkLoader::OverrideFileExistsFn(FileExistsFn fileExistsFn)
    {
        m_fileExistsFn = AZStd::move(fileExistsFn);
    }

    static AZ::Vector3 RadiansToDegrees(const AZ::Vector3& radians)
    {
        return AZ::Vector3(AZ::RadToDeg(radians.GetX()), AZ::RadToDeg(radians.GetY()), AZ::RadToDeg(radians.GetZ()));
    }

    static ViewBookmark ViewBookmarkFromCameraState(const AzFramework::CameraState& cameraState)
    {
        return { cameraState.m_position,
                 RadiansToDegrees(AzFramework::EulerAngles(
                     AZ::Matrix3x3::CreateFromColumns(cameraState.m_side, cameraState.m_forward, cameraState.m_up))) };
    }

    AZStd::optional<ViewBookmark> StoreViewBookmarkLastKnownLocationFromActiveCamera()
    {
        bool found = false;
        AzFramework::CameraState cameraState;
        Camera::EditorCameraRequestBus::BroadcastResult(found, &Camera::EditorCameraRequestBus::Events::GetActiveCameraState, cameraState);

        if (found)
        {
            return StoreViewBookmarkLastKnownLocationFromCameraState(cameraState);
        }

        return AZStd::nullopt;
    }

    AZStd::optional<ViewBookmark> StoreViewBookmarkLastKnownLocationFromCameraState(const AzFramework::CameraState& cameraState)
    {
        ViewBookmarkInterface* viewBookmarkInterface = AZ::Interface<ViewBookmarkInterface>::Get();

        if (const ViewBookmark viewBookmark = ViewBookmarkFromCameraState(cameraState);
            viewBookmarkInterface->SaveLastKnownLocation(viewBookmark))
        {
            return viewBookmark;
        }

        return AZStd::nullopt;
    }

    AZStd::optional<ViewBookmark> StoreViewBookmarkFromActiveCameraAtIndex(const int index)
    {
        bool found = false;
        AzFramework::CameraState cameraState;
        Camera::EditorCameraRequestBus::BroadcastResult(found, &Camera::EditorCameraRequestBus::Events::GetActiveCameraState, cameraState);

        if (found)
        {
            return StoreViewBookmarkFromCameraStateAtIndex(index, cameraState);
        }

        return AZStd::nullopt;
    }

    AZStd::optional<ViewBookmark> StoreViewBookmarkFromCameraStateAtIndex(const int index, const AzFramework::CameraState& cameraState)
    {
        ViewBookmarkInterface* viewBookmarkInterface = AZ::Interface<ViewBookmarkInterface>::Get();

        if (const ViewBookmark viewBookmark = ViewBookmarkFromCameraState(cameraState);
            viewBookmarkInterface->SaveBookmarkAtIndex(ViewBookmarkFromCameraState(cameraState), index))
        {
            return viewBookmark;
        }

        return AZStd::nullopt;
    }
} // namespace AzToolsFramework
