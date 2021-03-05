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

#include <AzCore/Utils/TypeHash.h>
#include <city.h>

namespace AZ
{
    HashValue32 TypeHash32(const uint8_t* buffer, uint64_t length)
    {
        AZ_Assert(buffer, "TypeHash32() - buffer cannot be null");
        return static_cast<HashValue32>(CityHash32(reinterpret_cast<const char*>(buffer), length));
    }

    HashValue64 TypeHash64(const uint8_t* buffer, uint64_t length)
    {
        AZ_Assert(buffer, "TypeHash64() - buffer cannot be null");
        return static_cast<HashValue64>(CityHash64(reinterpret_cast<const char*>(buffer), length));
    }

    HashValue64 TypeHash64(const uint8_t* buffer, uint64_t length, HashValue64 seed)
    {
        AZ_Assert(buffer, "TypeHash64() - buffer cannot be null");
        return static_cast<HashValue64>(CityHash64WithSeed(reinterpret_cast<const char*>(buffer), length, static_cast<uint64_t>(seed)));
    }

    HashValue64 TypeHash64(const uint8_t* buffer, uint64_t length, HashValue64 seed1, HashValue64 seed2)
    {
        AZ_Assert(buffer, "TypeHash64() - buffer cannot be null");
        return static_cast<HashValue64>(CityHash64WithSeeds(reinterpret_cast<const char*>(buffer), length, static_cast<uint64_t>(seed1), static_cast<uint64_t>(seed2)));
    }
}
