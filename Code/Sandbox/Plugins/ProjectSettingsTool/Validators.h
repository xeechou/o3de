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

#include "FunctorValidator.h"

namespace ProjectSettingsTool
{
    namespace Validators
    {
        namespace Internal 
        {
            // Returns true if file is readable and the correct mime type
            FunctorValidator::ReturnType FileReadableAndCorrectType(const QString& path, const QString& fileType);
        }

        static const int maxAndroidVersion = 2100000000;

        // Returns true if valid cross platform file or directory name
        FunctorValidator::ReturnType FileName(const QString& name);
        // Returns true if valid cross platform file or directory name or empty
        FunctorValidator::ReturnType FileNameOrEmpty(const QString& name);
        // Returns true if string isn't empty
        FunctorValidator::ReturnType IsNotEmpty(const QString& value);
        // Returns true if string is valid as a boolean
        FunctorValidator::ReturnType BoolString(const QString& value);
        // Returns true if string is valid android package and apple bundle identifier
        FunctorValidator::ReturnType PackageName(const QString& name);
        // Returns true if valid android version number
        FunctorValidator::ReturnType VersionNumber(const QString& value);
        // Returns true if valid ios version number
        FunctorValidator::ReturnType IOSVersionNumber(const QString& value);
        // Returns true if Public App Key is valid length
        FunctorValidator::ReturnType PublicAppKeyOrEmpty(const QString& value);
        // Returns true if path is empty or a valid xml file relative to <build dir>
        FunctorValidator::ReturnType ValidXmlOrEmpty(const QString& path);
        // Returns true if path is empty or a valid png file
        FunctorValidator::ReturnType ValidPngOrEmpty(const QString& path);
        // Returns true if path is empty or valid png file with specified dimensions
        template <int imageWidth, int imageHeight = imageWidth>
        FunctorValidator::ReturnType PngImageSetSizeOrEmpty(const QString& path);
    } // namespace Validators
} // namespace ProjectSettingsTool

#include "Validators_impl.h"
