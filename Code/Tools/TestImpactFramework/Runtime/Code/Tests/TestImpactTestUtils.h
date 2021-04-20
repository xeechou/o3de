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

#include <Artifact/Dynamic/TestImpactCoverage.h>
#include <Artifact/Dynamic/TestImpactTestEnumerationSuite.h>
#include <Artifact/Dynamic/TestImpactTestRunSuite.h>
#include <Artifact/Static/TestImpactBuildTargetDescriptor.h>
#include <Artifact/Static/TestImpactProductionTargetDescriptor.h>
#include <Artifact/Static/TestImpactTestTargetDescriptor.h>
#include <Artifact/Static/TestImpacttestTargetMeta.h>
#include <Process/TestImpactProcess.h>
#include <Test/Enumeration/TestImpactTestEnumeration.h>
#include <Test/Run/TestImpactTestCoverage.h>
#include <Test/Run/TestImpactTestRun.h>

#include <AzCore/std/optional.h>
#include <AzCore/std/string/string.h>

namespace UnitTest
{
    // Common parameters for process related tests
    inline constexpr const char* ValidProcessPath = LY_TEST_IMPACT_TEST_PROCESS_BIN;
    inline constexpr const char* InvalidProcessPath = "!!!@@@---???";
    inline constexpr const AZStd::chrono::milliseconds LongSleep = AZStd::chrono::minutes(60);
    inline constexpr const size_t LargeTextSize = 0xFFFF - 1; // 65,535 chars less the null terminator
    inline constexpr const AZStd::chrono::milliseconds ShortSleep = AZStd::chrono::milliseconds(500);
    inline constexpr const AZStd::chrono::milliseconds NoSleep = AZStd::chrono::milliseconds(0);

    // Writes the specified text string to the specified file
    void WriteTextToFile(const AZStd::string& text, const AZ::IO::Path& path);

    // Construct the arguments for launcing the test process
    AZStd::string ConstructTestProcessArgs(TestImpact::ProcessId pid, AZStd::chrono::milliseconds sleepTime);

    // Construct the arguments for launcing the test process with large text dump
    AZStd::string ConstructTestProcessArgsLargeText(TestImpact::ProcessId pid, AZStd::chrono::milliseconds sleepTime);

    // Known standard output string of the test process
    AZStd::string KnownTestProcessOutputString(TestImpact::ProcessId pid);

    // Known standard error string of the test process
    AZStd::string KnownTestProcessErrorString(TestImpact::ProcessId pid);

    // Generate a gtest typed test fixture name string based on the specified name and type
    AZStd::string GenerateTypedFixtureName(const AZStd::string& name, size_t typeNum);

    // Generate a gtest parameterized test fixture name string based on the specified name and permutation number
    AZStd::string GenerateParameterizedFixtureName(
        const AZStd::string& name,
        const AZStd::optional<const AZStd::string>& prefix = AZStd::nullopt);

    // Generate a gtest parameterized test name string based on the specified name and permutation number
    AZStd::string GenerateParameterizedTestName(const AZStd::string& name, size_t testNum);

    // Generate a JSON string of array elements from the specified vector
    AZStd::string StringVectorToJSONElements(const AZStd::vector<AZStd::string> strings);

    // Generate a build target descriptor string in JSON format from the specified build target description
    AZStd::string GenerateBuildTargetDescriptorString(
        const AZStd::string& name,
        const AZStd::string& outputName,
        const AZStd::string& path,
        const AZStd::vector<AZStd::string>& staticSources,
        const AZStd::vector<AZStd::string>& autogenInputs,
        const AZStd::vector<AZStd::string>& autogenOutputs);

    // Generate a build target descriptor from the specified build target description
    // Note: no check for correctness of arguments is peformed
    TestImpact::BuildTargetDescriptor GenerateBuildTargetDescriptor(
        const AZStd::string& name,
        const AZStd::string& outputName,
        const AZStd::string& path,
        const AZStd::vector<AZStd::string>& staticSources,
        const TestImpact::AutogenSources& autogenSources);

    // Procedurally generate a parameterized test suite based on the supplied parameters
    TestImpact::TestEnumerationSuite GenerateParamterizedSuite(
        const AZStd::pair<AZStd::string, bool>& fixture,
        const AZStd::optional<AZStd::string>& permutation,
        const AZStd::vector<AZStd::pair<AZStd::string, bool>> tests,
        size_t permutationCount);

    // Procedurally generate a typed test suite based on the supplied parameters
    void GenerateTypedSuite(
        const AZStd::pair<AZStd::string, bool>& fixture,
        const AZStd::vector<AZStd::pair<AZStd::string, bool>> tests,
        size_t permutationCount,
        AZStd::vector<TestImpact::TestEnumerationSuite>& parentSuiteList);

    // Helper functions for calculating test suite meta-data
    size_t CalculateNumPassedTests(const AZStd::vector<TestImpact::TestRunSuite>& suites);
    size_t CalculateNumFailedTests(const AZStd::vector<TestImpact::TestRunSuite>& suites);
    size_t CalculateNumRunTests(const AZStd::vector<TestImpact::TestRunSuite>& suites);
    size_t CalculateNumNotRunTests(const AZStd::vector<TestImpact::TestRunSuite>& suites);

    template<typename TestSuite>
    size_t CalculateNumTestSuites(const AZStd::vector<TestSuite>& suites)
    {
        return suites.size();
    }

    template<typename TestSuite>
    size_t CalculateNumTests(const AZStd::vector<TestSuite>& suites)
    {
        size_t numTests = 0;
        for (const auto& suite : suites)
        {
            numTests += suite.m_tests.size();
        }

        return numTests;
    }

    template<typename TestSuite>
    size_t CalculateNumEnabledTests(const AZStd::vector<TestSuite>& suites)
    {
        size_t numEnabledTests = 0;
        for (const auto& suite : suites)
        {
            if (!suite.m_enabled)
            {
                continue;
            }

            for (const auto& test : suite.m_tests)
            {
                if (test.m_enabled)
                {
                    numEnabledTests++;
                }
            }
        }

        return numEnabledTests;
    }

    template<typename TestSuite>
    size_t CalculateNumDisabledTests(const AZStd::vector<TestSuite>& suites)
    {
        size_t numDisabledTests = 0;
        for (const auto& suite : suites)
        {
            if (!suite.m_enabled)
            {
                numDisabledTests += suite.m_tests.size();
                continue;
            }

            for (const auto& test : suite.m_tests)
            {
                if (!test.m_enabled)
                {
                    numDisabledTests++;
                }
            }
        }

        return numDisabledTests;
    }

    // Test enumeration suite representation of the test targets used for testing
    AZStd::vector<TestImpact::TestEnumerationSuite> GetTestTargetATestEnumerationSuites();
    AZStd::vector<TestImpact::TestEnumerationSuite> GetTestTargetBTestEnumerationSuites();
    AZStd::vector<TestImpact::TestEnumerationSuite> GetTestTargetCTestEnumerationSuites();
    AZStd::vector<TestImpact::TestEnumerationSuite> GetTestTargetDTestEnumerationSuites();

    // Test run suite representation of the test targets used for testing
    AZStd::vector<TestImpact::TestRunSuite> GetTestTargetATestRunSuites();
    AZStd::vector<TestImpact::TestRunSuite> GetTestTargetBTestRunSuites();
    AZStd::vector<TestImpact::TestRunSuite> GetTestTargetCTestRunSuites();
    AZStd::vector<TestImpact::TestRunSuite> GetTestTargetDTestRunSuites();

    // Line coverage representation of the test targets used for testing
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetALineModuleCoverages();
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetBLineModuleCoverages();
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetCLineModuleCoverages();
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetDLineModuleCoverages();

    // Source coverage representation of the test targets used for testing
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetASourceModuleCoverages();
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetBSourceModuleCoverages();
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetCSourceModuleCoverages();
    AZStd::vector<TestImpact::ModuleCoverage> GetTestTargetDSourceModuleCoverages();

    // Helper comparisons for test validation (could potentially be moved to production source in the future)
    bool operator==(const TestImpact::TestEnumerationCase& lhs, const TestImpact::TestEnumerationCase& rhs);
    bool operator==(const TestImpact::TestEnumerationSuite& lhs, const TestImpact::TestEnumerationSuite& rhs);
    bool operator==(const AZStd::vector<TestImpact::TestEnumerationSuite>& lhs, const AZStd::vector<TestImpact::TestEnumerationSuite>& rhs);
    bool operator==(const TestImpact::TestEnumeration& lhs, const TestImpact::TestEnumeration& rhs);

    bool operator==(const TestImpact::TestRunCase& lhs, const TestImpact::TestRunCase& rhs);
    bool operator==(const TestImpact::TestRunSuite& lhs, const TestImpact::TestRunSuite& rhs);
    bool operator==(const AZStd::vector<TestImpact::TestRunSuite>& lhs, const AZStd::vector<TestImpact::TestRunSuite>& rhs);
    bool operator==(const TestImpact::TestRun& lhs, const TestImpact::TestRun& rhs);
    bool CheckTestRunsAreEqualIgnoreDurations(const TestImpact::TestRun& lhs, const TestImpact::TestRun& rhs);

    bool operator==(const TestImpact::BuildMetaData& lhs, const TestImpact::BuildMetaData& rhs);
    bool operator==(const TestImpact::TargetSources& lhs, const TestImpact::TargetSources& rhs);
    bool operator==(const TestImpact::BuildTargetDescriptor& lhs, const TestImpact::BuildTargetDescriptor& rhs);
    bool operator==(const TestImpact::TestTargetMeta& lhs, const TestImpact::TestTargetMeta& rhs);
    bool operator==(const TestImpact::ProductionTargetDescriptor& lhs, const TestImpact::ProductionTargetDescriptor& rhs);
    bool operator==(const TestImpact::TestTargetDescriptor& lhs, const TestImpact::TestTargetDescriptor& rhs);

    bool operator==(const TestImpact::LineCoverage& lhs, const TestImpact::LineCoverage& rhs);
    bool operator==(const TestImpact::SourceCoverage& lhs, const TestImpact::SourceCoverage& rhs);
    bool operator==(const TestImpact::ModuleCoverage& lhs, const TestImpact::ModuleCoverage& rhs);
    bool operator==(const AZStd::vector<TestImpact::ModuleCoverage>& lhs, const AZStd::vector<TestImpact::ModuleCoverage>& rhs);
    bool operator==(const TestImpact::TestCoverage& lhs, const TestImpact::TestCoverage& rhs);
} // namespace UnitTest
