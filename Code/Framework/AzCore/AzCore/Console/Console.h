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

#include <AzCore/Console/IConsole.h>
#include <AzCore/Memory/OSAllocator.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/containers/unordered_map.h>

namespace AZ
{
    //! @class Console
    //! A simple console class for providing text based variable and process interaction.
    class Console final
        : public IConsole
    {
    public:
        AZ_RTTI(Console, "{CF6DCDE7-1A66-442C-BA87-01A432C13E7D}", IConsole);
        AZ_CLASS_ALLOCATOR(Console, AZ::OSAllocator, 0);

        Console();
        ~Console() override;

        //! IConsole interface
        //! @{
        bool PerformCommand
        (
            const char* command,
            ConsoleSilentMode silentMode = ConsoleSilentMode::NotSilent,
            ConsoleInvokedFrom invokedFrom = ConsoleInvokedFrom::AzConsole,
            ConsoleFunctorFlags requiredSet = ConsoleFunctorFlags::Null,
            ConsoleFunctorFlags requiredClear = ConsoleFunctorFlags::ReadOnly
        ) override;
        bool PerformCommand
        (
            const ConsoleCommandContainer& commandAndArgs,
            ConsoleSilentMode silentMode = ConsoleSilentMode::NotSilent,
            ConsoleInvokedFrom invokedFrom = ConsoleInvokedFrom::AzConsole,
            ConsoleFunctorFlags requiredSet = ConsoleFunctorFlags::Null,
            ConsoleFunctorFlags requiredClear = ConsoleFunctorFlags::ReadOnly
        ) override;
        bool PerformCommand
        (
            AZStd::string_view command,
            const ConsoleCommandContainer& commandArgs,
            ConsoleSilentMode silentMode = ConsoleSilentMode::NotSilent,
            ConsoleInvokedFrom invokedFrom = ConsoleInvokedFrom::AzConsole,
            ConsoleFunctorFlags requiredSet = ConsoleFunctorFlags::Null,
            ConsoleFunctorFlags requiredClear = ConsoleFunctorFlags::ReadOnly
        ) override;
        void ExecuteConfigFile(AZStd::string_view configFileName) override;
        void ExecuteCommandLine(const AZ::CommandLine& commandLine) override;
        bool HasCommand(const char* command) override;
        ConsoleFunctorBase* FindCommand(const char* command) override;
        AZStd::string AutoCompleteCommand(const char* command) override;
        void VisitRegisteredFunctors(const FunctorVisitor& visitor) override;
        void RegisterFunctor(ConsoleFunctorBase* functor) override;
        void UnregisterFunctor(ConsoleFunctorBase* functor) override;
        void LinkDeferredFunctors(ConsoleFunctorBase*& deferredHead) override;
        //! @}

    private:

        void MoveFunctorsToDeferredHead(ConsoleFunctorBase*& deferredHead);

        //! Invokes a single console command, optionally returning the command output.
        //! @param command       the function to invoke
        //! @param inputs        the set of inputs to provide the function
        //! @param silentMode    if true, logs will be suppressed during command execution
        //! @param invokedFrom   the source point that initiated console invocation
        //! @param requiredSet   a set of flags that must be set on the functor for it to execute
        //! @param requiredClear a set of flags that must *NOT* be set on the functor for it to execute
        //! @return boolean true on success, false otherwise
        bool DispatchCommand
        (
            AZStd::string_view command,
            const ConsoleCommandContainer& inputs,
            ConsoleSilentMode silentMode,
            ConsoleInvokedFrom invokedFrom,
            ConsoleFunctorFlags requiredSet,
            ConsoleFunctorFlags requiredClear
        );

        AZ_DISABLE_COPY_MOVE(Console);

        ConsoleFunctorBase* m_head;
        using CommandMap = AZStd::unordered_map<CVarFixedString, AZStd::vector<ConsoleFunctorBase*>>;
        CommandMap m_commands;

        friend class ConsoleFunctorBase;
    };
}
