/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/DOM/DomVisitor.h>

#include <AzCore/Memory/Memory.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/std/containers/variant.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>

namespace AZ::Dom
{
    using KeyType = AZ::Name;

    enum class Type
    {
        NullType = 0,
        FalseType = 1,
        TrueType = 2,
        ObjectType = 3,
        ArrayType = 4,
        StringType = 5,
        NumberType = 6,
        NodeType = 7,
        OpaqueType = 8,
    };

    class Value;

    class Array
    {
    public:
        using ContainerType = AZStd::vector<Value>;
        using Iterator = ContainerType::iterator;
        using ConstIterator = ContainerType::const_iterator;

    private:
        ContainerType m_values;

        friend class Value;
    };

    using ArrayPtr = AZStd::shared_ptr<Array>;
    using ConstArrayPtr = AZStd::shared_ptr<const Array>;

    class Object
    {
    public:
        using EntryType = AZStd::pair<KeyType, Value>;
        using ContainerType = AZStd::vector<EntryType>;
        using Iterator = ContainerType::iterator;
        using ConstIterator = ContainerType::const_iterator;

    private:
        ContainerType m_values;

        friend class Value;
    };

    using ObjectPtr = AZStd::shared_ptr<Object>;
    using ConstObjectPtr = AZStd::shared_ptr<const Object>;

    class Node
    {
    public:
        AZ::Name GetName() const;
        void SetName(AZ::Name name);

        ObjectPtr GetMutableProperties();
        ConstObjectPtr GetProperties() const;

        ArrayPtr GetMutableChildren();
        ConstArrayPtr GetChildren() const;

        bool operator==(const Node& rhs) const;

    private:
        AZ::Name m_name;
        ArrayPtr m_array;
        ObjectPtr m_object;

        friend class Value;
    };

    class Value
    {
    public:
        // Constructors...
        Value();
        Value(const Value&);
        Value(Value&&) noexcept;
        Value(AZStd::string_view string, bool copy);

        explicit Value(int64_t value);
        explicit Value(uint64_t value);
        explicit Value(double value);
        explicit Value(bool value);

        static Value FromOpaqueValue(AZStd::any& value);

        // Equality / comparison / swap...
        Value& operator=(const Value&);
        Value& operator=(Value&&) noexcept;

        bool operator==(const Value& rhs) const;
        bool operator!=(const Value& rhs) const;

        void Swap(Value& other) noexcept;

        // Type info...
        Type GetType() const;
        bool IsNull() const;
        bool IsFalse() const;
        bool IsTrue() const;
        bool IsBool() const;
        bool IsNode() const;
        bool IsObject() const;
        bool IsArray() const;
        bool IsOpaqueValue() const;
        bool IsNumber() const;
        bool IsInt() const;
        bool IsUint() const;
        bool IsDouble() const;
        bool IsString() const;

        // Object API (also used by Node)...
        Value& SetObject();
        size_t MemberCount() const;
        size_t MemberCapacity() const;
        bool ObjectEmpty() const;

        Value& operator[](KeyType name);
        const Value& operator[](KeyType name) const;
        Value& operator[](AZStd::string_view name);
        const Value& operator[](AZStd::string_view name) const;

        Object::ConstIterator MemberBegin() const;
        Object::ConstIterator MemberEnd() const;
        Object::Iterator MemberBegin();
        Object::Iterator MemberEnd();

        Object::ConstIterator FindMember(KeyType name) const;
        Object::ConstIterator FindMember(AZStd::string_view name) const;
        Object::Iterator FindMember(KeyType name);
        Object::Iterator FindMember(AZStd::string_view name);

        Value& MemberReserve(size_t newCapacity);
        bool HasMember(KeyType name) const;
        bool HasMember(AZStd::string_view name) const;

        Value& AddMember(KeyType name, const Value& value);
        Value& AddMember(AZStd::string_view name, const Value& value);
        Value& AddMember(KeyType name, Value&& value);
        Value& AddMember(AZStd::string_view name, Value&& value);

        void RemoveAllMembers();
        void RemoveMember(KeyType name);
        void RemoveMember(AZStd::string_view name);
        Object::Iterator RemoveMember(Object::Iterator pos);
        Object::Iterator EraseMember(Object::ConstIterator pos);
        Object::Iterator EraseMember(Object::ConstIterator first, Object::ConstIterator last);
        Object::Iterator EraseMember(KeyType name);
        Object::Iterator EraseMember(AZStd::string_view name);

        Object::ContainerType& GetObject();
        const Object::ContainerType& GetObject() const;

        // Array API (also used by Node)...
        Value& SetArray();

        size_t Size() const;
        size_t Capacity() const;
        bool Empty() const;
        void Clear();

        Value& operator[](size_t index);
        const Value& operator[](size_t index) const;

        Array::ConstIterator Begin() const;
        Array::ConstIterator End() const;
        Array::Iterator Begin();
        Array::Iterator End();

        Value& Reserve(size_t newCapacity);
        Value& PushBack(Value value);
        Value& PopBack();

        Array::Iterator Erase(Array::ConstIterator pos);
        Array::Iterator Erase(Array::ConstIterator first, Array::ConstIterator last);

        Array::ContainerType& GetArray();
        const Array::ContainerType& GetArray() const;

        // Node API (supports both object + array API, plus a dedicated NodeName)...
        // bool CanConvertToNodeFromObject() const;
        // Value& ConvertToNodeFromObject();
        // Value& ConvertToObjectFromNode();

        void SetNode(AZ::Name name);
        void SetNode(AZStd::string_view name);

        AZ::Name GetNodeName() const;
        void SetNodeName(AZ::Name name);
        void SetNodeName(AZStd::string_view name);
        //! Convenience method, sets the first non-node element of a Node.
        void SetNodeValue(Value value);
        //! Convenience method, gets the first non-node element of a Node.
        Value GetNodeValue() const;

        // int API...
        int64_t GetInt() const;
        void SetInt(int64_t);

        // uint API...
        uint64_t GetUint() const;
        void SetUint(uint64_t);

        // bool API...
        bool GetBool() const;
        void SetBool(bool);

        // double API...
        double GetDouble() const;
        void SetDouble(double);

        // string API...
        AZStd::string_view GetString() const;
        size_t GetStringLength() const;
        void SetString(AZStd::string_view);
        void CopyFromString(AZStd::string_view);

        // opaque type API...
        AZStd::any& GetOpaqueValue() const;
        //! This sets this Value to represent a value of an type that the DOM has
        //! no formal knowledge of. Where possible, it should be preferred to
        //! serialize an opaque type into a DOM value instead, as serializers
        //! and other systems will have no means of dealing with fully arbitrary
        //! values.
        void SetOpaqueValue(AZStd::any&);

        // null API...
        void SetNull();

    private:
        const Object::ContainerType& GetObjectInternal() const;
        Object::ContainerType& GetObjectInternal();
        const Array::ContainerType& GetArrayInternal() const;
        Array::ContainerType& GetArrayInternal();

        explicit Value(AZStd::any* opaqueValue);

        // If using the the copy on write model, anything stored internally as a shared_ptr will
        // detach and copy when doing a mutating operation if use_count() > 1.

        // This internal storage will not have a 1:1 mapping to the public Type, as there may be
        // multiple storage options (e.g. strings being stored as non-owning string_view or
        // owning shared_ptr<string>)
        using ValueType = AZStd::variant<
            // NullType
            AZStd::monostate,
            // NumberType
            int64_t,
            uint64_t,
            double,
            // FalseType & TrueType
            bool,
            // StringType
            AZStd::string_view,
            AZStd::shared_ptr<AZStd::string>,
            // ObjectType
            ObjectPtr,
            // ArrayType
            ArrayPtr,
            // NodeType
            Node,
            // OpaqueType
            AZStd::any*>;

        ValueType m_value;
    };
} // namespace AZ::Dom
