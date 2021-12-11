/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/std/functional.h>
#include <AzCore/std/string/conversions.h>
#include <AzCore/std/string/string.h>

namespace AZ
{
    class Matrix4x4;
    class Quaternion;
    class Transform;
    class Vector2;
    class Vector3;
    class Vector4;
}

namespace AZStd
{
    void to_string(string& str, const AZ::Vector2& value);
    void to_string(string& str, const AZ::Vector3& value);
    void to_string(string& str, const AZ::Vector4& value);
    void to_string(string& str, const AZ::Quaternion& value);
    void to_string(string& str, const AZ::Matrix4x4& value);
    void to_string(string& str, const AZ::Transform& value);

    inline AZStd::string to_string(const AZ::Vector2& val) { AZStd::string str; to_string(str, val); return str; }
    inline AZStd::string to_string(const AZ::Vector3& val) { AZStd::string str; to_string(str, val); return str; }
    inline AZStd::string to_string(const AZ::Vector4& val) { AZStd::string str; to_string(str, val); return str; }
    inline AZStd::string to_string(const AZ::Quaternion& val) { AZStd::string str; to_string(str, val); return str; }
    inline AZStd::string to_string(const AZ::Matrix4x4& val) { AZStd::string str; to_string(str, val); return str; }
    inline AZStd::string to_string(const AZ::Transform& val) { AZStd::string str; to_string(str, val); return str; }
}
