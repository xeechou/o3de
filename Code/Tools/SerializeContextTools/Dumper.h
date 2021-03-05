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

#include <AzCore/JSON/document.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/string/string_view.h>

namespace AZ
{
    namespace SerializeContextTools
    {
        class Application;

        class Dumper
        {
        public:
            static bool DumpFiles(Application& application);
            static bool DumpSerializeContext(Application& application);

        private:
            static AZStd::vector<Uuid> CreateFilterListByNames(SerializeContext* context, AZStd::string_view name);
            
            static AZStd::string_view ExtractNamespace(const AZStd::string& name);

            static rapidjson::Value WriteToJsonValue(const Uuid& uuid, rapidjson::Document& document);
            
            static bool DumpClassContent(const SerializeContext::ClassData* classData, rapidjson::Value& parent, rapidjson::Document& document, 
                const AZStd::vector<Uuid>& systemComponents, SerializeContext* context, AZStd::string& scratchStringBuffer);
            static bool DumpClassContent(AZStd::string& output, void* classPtr, const Uuid& classId, SerializeContext* context);
            
            static void DumpElementInfo(const SerializeContext::ClassElement& element, const SerializeContext::ClassData* classData, SerializeContext* context, 
                rapidjson::Value& fields, rapidjson::Value& bases, rapidjson::Document& document, AZStd::string& scratchStringBuffer);
            static void DumpElementInfo(AZStd::string& output, const SerializeContext::ClassElement* classElement, SerializeContext* context);
            
            static void DumpGenericStructure(AZStd::string& output, GenericClassInfo* genericClassInfo, SerializeContext* context);
            static rapidjson::Value DumpGenericStructure(GenericClassInfo* genericClassInfo, SerializeContext* context, 
                rapidjson::Document& parentDoc, AZStd::string& scratchStringBuffer);
            
            static void DumpPrimitiveTag(AZStd::string& output, const SerializeContext::ClassData* classData, 
                const SerializeContext::ClassElement* classElement);
            static void DumpClassName(rapidjson::Value& parent, SerializeContext* context, const SerializeContext::ClassData* classData, 
                rapidjson::Document& parentDoc, AZStd::string& scratchStringBuffer);
            
            static void AppendTypeName(AZStd::string& output, const SerializeContext::ClassData* classData, const Uuid& classId);
        };
    } // namespace SerializeContextTools
} // namespace AZ