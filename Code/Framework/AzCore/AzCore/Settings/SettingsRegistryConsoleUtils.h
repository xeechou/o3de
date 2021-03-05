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

#include <AzCore/std/containers/fixed_vector.h>
#include <AzCore/Console/ConsoleFunctor.h>
#include <AzCore/Settings/SettingsRegistry.h>

namespace AZ::SettingsRegistryConsoleUtils
{
    //! Only 4 console command are registered for the settings registry
    //! "regset", "regremove", "regdump", "regdumpall"
    //! The value should be increased if more commands are needed
    inline constexpr size_t MaxSettingsRegistryConsoleFunctors = 4;

    inline constexpr const char* SettingsRegistrySet = "sr_regset";
    inline constexpr const char* SettingsRegistryRemove = "sr_regremove";
    inline constexpr const char* SettingsRegistryDump = "sr_regdump";
    inline constexpr const char* SettingsRegistryDumpAll = "sr_regdumpall";

    // RAII structure which owns the instances of the Settings Registry Console commands
    // registered with an AZ Console
    // On destruction of the handle, the Settings Registry Console commands are unregistered
    struct ConsoleFunctorHandle
    {
        ConsoleFunctorHandle() = default;
        using SettingsRegistryConsoleFunctor = AZ::ConsoleFunctor<SettingsRegistryInterface, false>;
        using SettingsRegistryFunctorsArray = AZStd::fixed_vector<SettingsRegistryConsoleFunctor, MaxSettingsRegistryConsoleFunctors>;

        SettingsRegistryFunctorsArray m_consoleFunctors;
    };

    //! Registers the following console commands
    //! "sr_regset" accepts 2 or more arguments - <json path> <json value>*
    //!  Sets the supplied <json value> at the input <json path>
    //!
    //! "sr_regremove" accepts 1 or more argument - <json path>*
    //!  Remove the json values at each of the input <json path>*
    //!  Removal occurs in the order of the json paths specified to the command
    //!
    //! "sr_regdump" accepts 1 or more arguments <json path>*
    //!  Outputs each json value at the supplied json paths to stdout.
    //!  The values are dumped in the order of the json paths specified to the command
    //!  If multiple json paths are dumped, the values will be separated by a newline
    //!
    //! "sr_regdumpall" accepts 0 arguments and dumps the entire settings registry
    //!  NOTE: this might result in a large amount of output to the console
    [[nodiscard]] ConsoleFunctorHandle RegisterAzConsoleCommands(SettingsRegistryInterface& registry, AZ::IConsole& azConsole);
    
}
