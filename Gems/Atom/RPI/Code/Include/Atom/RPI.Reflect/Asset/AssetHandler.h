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

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/std/smart_ptr/make_shared.h>

#include <AzFramework/Asset/GenericAssetHandler.h>

namespace AZ
{
    namespace RPI
    {
        //! Base asset handler class for all assets in the RPI. Provides constructor to initialize
        //! asset type information from static members on the asset class.
        template <typename AssetDataT>
        class AssetHandler : public AzFramework::GenericAssetHandler<AssetDataT>
        {
            using Base = AzFramework::GenericAssetHandler<AssetDataT>;
        public:
            AssetHandler()
                : Base(AssetDataT::DisplayName, AssetDataT::Group, AssetDataT::Extension)
            {}

            virtual ~AssetHandler()
            {
                Base::Unregister();
            }

            Data::AssetHandler::LoadResult LoadAssetData(
                const AZ::Data::Asset<AZ::Data::AssetData>& asset,
                AZStd::shared_ptr<AZ::Data::AssetDataStream> stream,
                const AZ::Data::AssetFilterCB& assetLoadFilterCB) override
            {
                return Base::LoadAssetData(asset, stream, assetLoadFilterCB);
            }
        };

        //! This is a helper function for creating an asset handler unique_ptr instance and registering it.
        template<class T, class ... Args>
        AZStd::unique_ptr<T> MakeAssetHandler(Args&& ... args)
        {
            auto assetHandler = AZStd::make_unique<T>(AZStd::forward<Args&&>(args) ...);
            assetHandler->Register();
            return AZStd::move(assetHandler);
        }

        using AssetHandlerPtrList = AZStd::vector<AZStd::unique_ptr<Data::AssetHandler>>;
        
        //! This is a helper function for creating an asset handler shared_ptr instance and registering it.
        //! (This can be used in cases where AssetHandlerPtrList won't work because the owning class has
        //! to have a copy constructor).
        template<class T, class ... Args>
        AZStd::shared_ptr<T> MakeSharedAssetHandler(Args&& ... args)
        {
            auto assetHandler = AZStd::make_shared<T>(AZStd::forward<Args&&>(args) ...);
            assetHandler->Register();
            return AZStd::move(assetHandler);
        }

        using AssetHandlerSharedPtrList = AZStd::vector<AZStd::shared_ptr<Data::AssetHandler>>;
    }
}
