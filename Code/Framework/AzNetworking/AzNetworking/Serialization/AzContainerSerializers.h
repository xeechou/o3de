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

#include <AzNetworking/Serialization/ISerializer.h>
#include <AzNetworking/Utilities/NetworkCommon.h>
#include <AzCore/std/containers/stack.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/fixed_unordered_map.h>
#include <AzCore/std/containers/map.h>
#include <AzCore/std/string/string.h>
#include <limits>

namespace AzNetworking
{
    // Generic AZ Containers
    template <typename TYPE>
    struct SerializeAzContainer
    {
        static bool Serialize(ISerializer& serializer, TYPE& container)
        {
            using ValueType = typename TYPE::value_type;
            constexpr uint32_t max = std::numeric_limits<uint32_t>::max(); // Limit to uint32 max elements
            const bool write = (serializer.GetSerializerMode() == SerializerMode::WriteToObject);

            uint32_t size = static_cast<uint32_t>(container.size());
            bool success = serializer.Serialize(size, "Size");

            // Dynamic containers require different read/write serialization interfaces
            if (write)
            {
                container.clear();
                AzNetworking::AzContainerHelper::ReserveContainer<TYPE>(container, size);
                ValueType element;
                for (uint32_t i = 0; i < size; ++i)
                {
                    success &= serializer.Serialize(element, GenerateIndexLabel<max>(i).c_str());
                    container.insert(container.end(), element);
                }
            }
            else
            {
                uint32_t i = 0;
                for (auto it = container.begin(); it != container.end(); ++it, ++i)
                {
                    success &= serializer.Serialize(*it, GenerateIndexLabel<max>(i).c_str());
                }
            }
            return success;
        }
    };

    // fixed size array
    template <typename TYPE, AZStd::size_t Size>
    struct SerializeAzContainer<AZStd::array<TYPE, Size>>
    {
        static bool Serialize(ISerializer& serializer, AZStd::array<TYPE, Size>& container)
        {
            constexpr uint32_t max = static_cast<uint32_t>(Size);
            static_assert(Size <= max, "Array size must be less than max.\n");
            bool success = true;

            int i = 0;
            for (auto &elem : container)
            {
                success &= serializer.Serialize(elem, GenerateIndexLabel<max>(i++).c_str());
            }
            return success;
        }
    };

    // fixed_unordered_map
    template <typename Key, typename MappedType, AZStd::size_t FixedNumBuckets, AZStd::size_t FixedNumElements, class Hasher, class EqualKey>
    struct SerializeAzContainer<AZStd::fixed_unordered_map<Key, MappedType, FixedNumBuckets, FixedNumElements, Hasher, EqualKey>>
    {
        using TYPE = AZStd::fixed_unordered_map<Key, MappedType, FixedNumBuckets, FixedNumElements, Hasher, EqualKey>;
        static bool Serialize(ISerializer& serializer, TYPE& container)
        {
            using SizeType = typename TYPE::size_type;
            using ValueType = typename TYPE::value_type;
            static_assert(FixedNumElements >= FixedNumBuckets, "fixed_unordered_map buckets is less than elements.");
            constexpr uint32_t max = static_cast<uint32_t>(FixedNumElements); // Elements is > Buckets
            const bool write = (serializer.GetSerializerMode() == SerializerMode::WriteToObject);

            SizeType size = container.size();
            bool success = serializer.Serialize(size, "Size");

            if (write)
            {
                container.clear();
                ValueType element;
                for (uint32_t i = 0; i < size; ++i)
                {
                    success &= serializer.Serialize(element, GenerateIndexLabel<max>(i).c_str());
                    container.insert(element);

                }
            }
            else
            {
                uint32_t i = 0;
                for (auto it = container.begin(); it != container.end(); ++it, ++i)
                {
                    success &= serializer.Serialize(*it, GenerateIndexLabel<max>(i).c_str());
                }
            }
            return success;
        }
    };

    // fixed_unordered_multimap
    template <typename Key, typename MappedType, AZStd::size_t FixedNumBuckets, AZStd::size_t FixedNumElements, class Hasher, class EqualKey>
    struct SerializeAzContainer<AZStd::fixed_unordered_multimap<Key, MappedType, FixedNumBuckets, FixedNumElements, Hasher, EqualKey>>
    {
        using TYPE = AZStd::fixed_unordered_multimap<Key, MappedType, FixedNumBuckets, FixedNumElements, Hasher, EqualKey>;
        static bool Serialize(ISerializer& serializer, TYPE& container)
        {
            using SizeType = typename TYPE::size_type;
            using ValueType = typename TYPE::value_type;
            static_assert(FixedNumElements >= FixedNumBuckets, "fixed_unordered_multimap buckets is less than elements.");
            constexpr uint32_t max = static_cast<uint32_t>(FixedNumElements); // Elements is > Buckets
            const bool write = (serializer.GetSerializerMode() == SerializerMode::WriteToObject);

            SizeType size = container.size();
            bool success = serializer.Serialize(size, "Size");

            if (write)
            {
                container.clear();
                ValueType element;
                for (uint32_t i = 0; i < size; ++i)
                {
                    success &= serializer.Serialize(element, GenerateIndexLabel<max>(i).c_str());
                    container.insert(element);

                }
            }
            else
            {
                uint32_t i = 0;
                for (auto it = container.begin(); it != container.end(); ++it, ++i)
                {
                    success &= serializer.Serialize(*it, GenerateIndexLabel<max>(i).c_str());
                }
            }
            return success;
        }
    };

    // multimap
    template <class Key, class MappedType, class Compare, class Allocator>
    struct SerializeAzContainer<AZStd::multimap<Key, MappedType, Compare, Allocator>>
    {
        using TYPE = AZStd::multimap<Key, MappedType, Compare, Allocator>;
        static bool Serialize(ISerializer& serializer, TYPE& container)
        {
            using SizeType = typename TYPE::size_type;
            using ValueType = typename TYPE::value_type;
            constexpr uint32_t max = std::numeric_limits<uint32_t>::max(); // Limit to uint32 max elements
            const bool write = (serializer.GetSerializerMode() == SerializerMode::WriteToObject);

            SizeType size = static_cast<uint32_t>(container.size());
            bool success = serializer.Serialize(size, "Size");

            if (write)
            {
                container.clear();
                ValueType element;
                for (uint32_t i = 0; i < size; ++i)
                {
                    success &= serializer.Serialize(element, GenerateIndexLabel<max>(i).c_str());
                    container.insert(element);
                }
            }
            else
            {
                uint32_t i = 0;
                for (auto it = container.begin(); it != container.end(); ++it, ++i)
                {
                    success &= serializer.Serialize(*it, GenerateIndexLabel<max>(i).c_str());
                }
            }
            return success;
        }
    };

    // String
    template<>
    struct SerializeAzContainer<AZStd::string>
    {
        static bool Serialize(ISerializer& serializer, AZStd::string& value)
        {
            uint32_t size = aznumeric_cast<uint32_t>(value.length());
            uint32_t outBytes = size;

            bool success = serializer.Serialize(size, "Size");
            value.resize_no_construct(size);
            success &= serializer.SerializeBytes(reinterpret_cast<uint8_t*>(value.data()), size, true, outBytes, "String");

            return success && outBytes == size;
        }
    };

    // fixed_string
    template <AZStd::size_t MaxElementCount>
    struct SerializeAzContainer<AZStd::fixed_string<MaxElementCount>>
    {
        static bool Serialize(ISerializer& serializer, AZStd::fixed_string<MaxElementCount>& value)
        {
            using SizeType = typename AZ::SizeType<AZ::RequiredBytesForValue<MaxElementCount>(), false>::Type;
            SizeType size = aznumeric_cast<SizeType>(value.length());
            uint32_t outBytes = static_cast<uint32_t>(size);

            bool success = serializer.Serialize(size, "Size");
            value.resize_no_construct(size);
            success &= serializer.SerializeBytes(reinterpret_cast<uint8_t*>(value.data()), static_cast<uint32_t>(size), true, outBytes, "String");

            return success && outBytes == size;
        }
    };

    // Az Containers
    template <typename TYPE>
    struct SerializeObjectHelper<TYPE, AZStd::enable_if_t<AzContainerHelper::IsIterableContainer<TYPE>::Value>>
    {
        static bool SerializeObject(ISerializer& serializer, TYPE& container)
        {
            return SerializeAzContainer<TYPE>::Serialize(serializer, container);
        }
    };
}
