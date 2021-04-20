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

#include <TestImpactTestUtils.h>

#include <Artifact/Factory/TestImpactBuildTargetDescriptorFactory.h>
#include <Artifact/TestImpactArtifactException.h>

#include <AzCore/UnitTest/TestTypes.h>
#include <AzTest/AzTest.h>

namespace UnitTest
{
    class BuildTargetDescriptorFactoryTestFixture
        : public AllocatorsTestFixture
    {
    protected:
        const AZStd::vector<AZStd::string> m_staticExclude = {".cmake"};
        const AZStd::vector<AZStd::string> m_inputExclude = {".jinja"};
        const AZStd::string m_autogenMatcher = {"(.*)\\..*"};

        const AZStd::vector<AZStd::string> m_autogenInputs =
        {
            "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/Log.ScriptCanvasNode.xml",
            "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/DrawText.ScriptCanvasNode.xml",
            "Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNode_Header.jinja",
            "Gems/ScriptCanvas/Code/Include/ScriptCanvas/AutoGen/ScriptCanvasNode_Source.jinja"
        };

        const AZStd::vector<AZStd::string> m_autogenOutputs =
        {
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.cpp",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.cpp"
        };

        const AZStd::vector<AZStd::string> m_staticSources =
        {
            "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/precompiled.cpp",
            "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/precompiled.h",
            "Gems/ScriptCanvasDiagnosticLibrary/Code/scriptcanvasdiagnosticlibrary_autogen_files.cmake",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.cpp",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.cpp"
        };

        const AZStd::vector<AZStd::string> m_expectedStaticSources =
        {
            "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/precompiled.cpp",
            "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/precompiled.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.h",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.cpp",
            "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.cpp"
        };

        const AZStd::string m_name = "ScriptCanvasDiagnosticLibrary.Static";
        const AZStd::string m_outputName = "ScriptCanvasDiagnosticLibrary";
        const AZStd::string m_path = "Gems/ScriptCanvasDiagnosticLibrary/Code";

        const TestImpact::AutogenSources m_expectedAutogenSources =
        {
            {
                "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/Log.ScriptCanvasNode.xml",
                {
                    "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.h",
                    "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/Log.generated.cpp"
                }
            },
            {
                "Gems/ScriptCanvasDiagnosticLibrary/Code/Source/DrawText.ScriptCanvasNode.xml",
                {
                    "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.h",
                    "windows_vs2019/Gems/ScriptCanvasDiagnosticLibrary/Code/Azcg/Generated/Source/DrawText.generated.cpp"
                }
            }
        };
    };

    TEST_F(BuildTargetDescriptorFactoryTestFixture, NoRawData_ExpectArtifactException)
    {
        // Given an empty raw descriptor string
        const AZStd::string rawTargetDescriptor;

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, InvalidRawData_ExpectArtifactException)
    {
        // Given a raw descriptor string of invalid data
        const AZStd::string rawTargetDescriptor = "abcde";

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, NoAutogenMatcher_ExpectArtifactException)
    {
        // Given a valid raw descriptor string
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, m_staticSources, m_autogenInputs, m_autogenOutputs);

        try
        {
            // When attempting to construct the build target descriptor with an empty autogen matcher
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, "");

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, EmptyName_ExpectArtifactException)
    {
        // Given a invalid raw descriptor string lacking build meta-data name
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString("", m_outputName, m_path, m_staticSources, m_autogenInputs, m_autogenOutputs);

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, EmptyOutputName_ExpectArtifactException)
    {
        // Given a invalid raw descriptor string lacking build meta-data output name
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, "", m_path, m_staticSources, m_autogenInputs, m_autogenOutputs);

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, EmptyPath_ExpectArtifactException)
    {
        // Given a invalid raw descriptor string lacking build meta-data path
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, "", m_staticSources, m_autogenInputs, m_autogenOutputs);

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, NoStaticSources_ExpectValidDescriptor)
    {
        const TestImpact::BuildTargetDescriptor expectedBuiltTarget =
            GenerateBuildTargetDescriptor(m_name, m_outputName, m_path, {}, m_expectedAutogenSources);

        // Given a valid raw descriptor string with no static sources
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, {}, m_autogenInputs, m_autogenOutputs);

        // When attempting to construct the build target descriptor
        const TestImpact::BuildTargetDescriptor buildTarget =
            TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, {}, m_inputExclude, m_autogenMatcher);

        // Expect the constructed build target descriptor to match the specified descriptor
        EXPECT_TRUE(buildTarget == expectedBuiltTarget);
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, NoAutogenSources_ExpectValidDescriptor)
    {
        const TestImpact::BuildTargetDescriptor expectedBuiltTarget =
            GenerateBuildTargetDescriptor(m_name, m_outputName, m_path, m_expectedStaticSources, {});

        // Given a valid raw descriptor string with no autogen sources
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, m_staticSources, {}, {});

        // When attempting to construct the build target descriptor
        const TestImpact::BuildTargetDescriptor buildTarget =
            TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

        // Expect the constructed build target descriptor to match the specified descriptor
        EXPECT_TRUE(buildTarget == expectedBuiltTarget);
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, NoStaticOrAutogenSources_ExpectValidDescriptor)
    {
        const TestImpact::BuildTargetDescriptor expectedBuiltTarget = GenerateBuildTargetDescriptor(m_name, m_outputName, m_path, {}, {});

        // Given a valid raw descriptor string with no static or autogen sources
        const AZStd::string rawTargetDescriptor = GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, {}, {}, {});

        // When attempting to construct the build target descriptor
        const TestImpact::BuildTargetDescriptor buildTarget =
            TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

        // Expect the constructed build target descriptor to match the specified descriptor
        EXPECT_TRUE(buildTarget == expectedBuiltTarget);
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, AutogenOutputSourcesButNoAutogenInputSources_ExpectArtifactException)
    {
        // Given a valid raw descriptor string with autogen output sources but no autogen input sources
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, m_staticSources, {}, m_autogenOutputs);

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, AutogenInputSourcesButNoAutogenOutputSources_ExpectArtifactException)
    {
        // Given a valid raw descriptor string with autogen inout sources but no autogen output sources
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, m_staticSources, m_autogenInputs, {});

        try
        {
            // When attempting to construct the build target descriptor
            const TestImpact::BuildTargetDescriptor buildTarget =
                TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an artifact exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(BuildTargetDescriptorFactoryTestFixture, StaticAndAutogenSources_ExpectValidDescriptor)
    {
        const TestImpact::BuildTargetDescriptor expectedBuiltTarget =
            GenerateBuildTargetDescriptor(m_name, m_outputName, m_path, m_expectedStaticSources, m_expectedAutogenSources);

        // Given a valid raw descriptor string with static and autogen sources
        const AZStd::string rawTargetDescriptor =
            GenerateBuildTargetDescriptorString(m_name, m_outputName, m_path, m_staticSources, m_autogenInputs, m_autogenOutputs);

        // When attempting to construct the build target descriptor
        const TestImpact::BuildTargetDescriptor buildTarget =
            TestImpact::BuildTargetDescriptorFactory(rawTargetDescriptor, m_staticExclude, m_inputExclude, m_autogenMatcher);

        // Expect the constructed build target descriptor to match the specified descriptor
        EXPECT_TRUE(buildTarget == expectedBuiltTarget);
    }
} // namespace UnitTest
