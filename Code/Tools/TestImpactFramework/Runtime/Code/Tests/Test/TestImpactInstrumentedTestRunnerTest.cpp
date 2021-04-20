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

#include <TestImpactTestJobRunnerCommon.h>
#include <TestImpactTestUtils.h>

#include <Artifact/TestImpactArtifactException.h>
#include <Test/Run/TestImpactInstrumentedTestRunner.h>
#include <Test/Run/TestImpactTestRunException.h>

#include <AzCore/IO/Path/Path.h>
#include <AzCore/UnitTest/TestTypes.h>
#include <AzCore/std/containers/array.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/optional.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/string/regex.h>
#include <AzCore/std/string/string.h>
#include <AzTest/AzTest.h>

namespace UnitTest
{
    using JobExceptionPolicy = TestImpact::InstrumentedTestRunner::JobExceptionPolicy;
    using CoverageExceptionPolicy = TestImpact::InstrumentedTestRunner::CoverageExceptionPolicy;

    struct TargetPaths
    {
        AZ::IO::Path m_targetBinary;
        AZ::IO::Path m_testRunArtifact;
        AZ::IO::Path m_testCoverageArtifact;
    };

    // Indices for looking up job command arguments for the different coverage levels
    enum CoverageLevel : uint8_t
    {
        LineLevel = 0,
        SourceLevel
    };

    // Get the job command for an instrumented test run
    AZStd::string GetRunCommandForTarget(const TargetPaths& testTarget, CoverageLevel coverageLevel, const char* sourcesFilter)
    {
        AZStd::string args = AZStd::string::format(
            "%s "                                                               // 1. Instrumented test runner
            "--coverage_level %s "                                              // 2. Coverage level
            "--export_type cobertura:\"%s\" "                                   // 3. Test coverage artifact path
            "--modules \"%s\" "                                                 // 4. Modules path
            "--excluded_modules \"%s\" "                                        // 5. Exclude modules
            "--sources \"%s\" -- "                                              // 6. Sources path
            "\"%s\" "                                                           // 7. Test runner binary
            "\"%s\" "                                                           // 8. Test target bin
            "AzRunUnitTests "
            "--gtest_output=xml:%s",                                            // 9. Test run result artifact

            LY_TEST_IMPACT_INSTRUMENTATION_BIN,                                 // 1.
            (coverageLevel == CoverageLevel::LineLevel ? "line" : "source"),    // 2.
            testTarget.m_testCoverageArtifact.c_str(),                          // 3.
            LY_TEST_IMPACT_MODULES_DIR,                                         // 4.
            LY_TEST_IMPACT_AZ_TESTRUNNER_BIN,                                   // 5.
            sourcesFilter,                                                      // 6.
            LY_TEST_IMPACT_AZ_TESTRUNNER_BIN,                                   // 7.
            testTarget.m_targetBinary.c_str(),                                  // 8.
            testTarget.m_testRunArtifact.c_str()                                // 9.
        );

        // OpenCppCoverage doesn't support forward slash directory separators so replace all with escaped backslashes
        return AZStd::regex_replace(args, AZStd::regex("/"), "\\");
    }

    // Get the job command for an instrumented test run with valid source filters to produce coverage artifact
    AZStd::string GetRunCommandForTargetWithSources(const TargetPaths& testTarget, CoverageLevel coverageLevel)
    {
        return GetRunCommandForTarget(testTarget, coverageLevel, LY_TEST_IMPACT_COVERAGE_SOURCES_DIR);
    }

    // Get the job command for an instrumented test run without valid source filters to produce empty coverage artifact
    AZStd::string GetRunCommandForTargetWithoutSources(const TargetPaths& testTarget, CoverageLevel coverageLevel)
    {
        return GetRunCommandForTarget(testTarget, coverageLevel, "C:\\No\\Sources\\Here\\At\\All\\Ever\\Ever\\Ever");
    }

    class InstrumentedTestRunnerFixture
        : public AllocatorsTestFixture
    {
    public:
        void SetUp() override;
        void TearDown() override;

    protected:
        using JobInfo = TestImpact::InstrumentedTestRunner::JobInfo;
        using JobData = TestImpact::InstrumentedTestRunner::JobData;

        AZStd::vector<JobInfo> m_jobInfos;
        AZStd::unique_ptr<TestImpact::InstrumentedTestRunner> m_testRunner;
        AZStd::vector<AZStd::array<AZStd::string, 2>> m_testTargetJobArgs;
        AZStd::vector<TargetPaths> m_testTargetPaths;
        AZStd::vector<TestImpact::TestRun> m_expectedTestTargetRuns;
        AZStd::vector<AZStd::array<TestImpact::TestCoverage, 2>> m_expectedTestTargetCoverages;
        AZStd::vector<TestImpact::TestRunResult> m_expectedTestTargetResult;
        size_t m_maxConcurrency = 0;
        CoverageLevel m_coverageLevel = CoverageLevel::LineLevel;
        inline static AZ::u32 s_uniqueTestCaseId = 0; // Unique id for each test case to be used as the suffix for the written files
    };

    void InstrumentedTestRunnerFixture::SetUp()
    {
        AllocatorsTestFixture::SetUp();

        // Suffix each artifact file with the unique test case id to ensure that there are no possible race conditions between cleaning
        // up the artifact files after each test case and those artifact files being written and read again in future test cases
        const AZStd::string fileExtension = AZStd::string::format(".%u.xml", s_uniqueTestCaseId++);

        const AZStd::string runPath = AZStd::string(LY_TEST_IMPACT_TEST_TARGET_RESULTS_DIR) + "/%s.Run" + fileExtension;
        const AZStd::string coveragePath = AZStd::string(LY_TEST_IMPACT_TEST_TARGET_COVERAGE_DIR) + "/%s.Coverage" + fileExtension;

        // TestTargetA
        m_testTargetPaths.emplace_back(TargetPaths{
            LY_TEST_IMPACT_TEST_TARGET_A_BIN, AZStd::string::format(runPath.c_str(), LY_TEST_IMPACT_TEST_TARGET_A_BASE_NAME),
            AZStd::string::format(coveragePath.c_str(), LY_TEST_IMPACT_TEST_TARGET_A_BASE_NAME)});

        // TestTargetB
        m_testTargetPaths.emplace_back(TargetPaths{
            LY_TEST_IMPACT_TEST_TARGET_B_BIN, AZStd::string::format(runPath.c_str(), LY_TEST_IMPACT_TEST_TARGET_B_BASE_NAME),
            AZStd::string::format(coveragePath.c_str(), LY_TEST_IMPACT_TEST_TARGET_B_BASE_NAME)});

        // TestTargetC
        m_testTargetPaths.emplace_back(TargetPaths{
            LY_TEST_IMPACT_TEST_TARGET_C_BIN, AZStd::string::format(runPath.c_str(), LY_TEST_IMPACT_TEST_TARGET_C_BASE_NAME),
            AZStd::string::format(coveragePath.c_str(), LY_TEST_IMPACT_TEST_TARGET_C_BASE_NAME)});

        // TestTargetD
        m_testTargetPaths.emplace_back(TargetPaths{
            LY_TEST_IMPACT_TEST_TARGET_D_BIN, AZStd::string::format(runPath.c_str(), LY_TEST_IMPACT_TEST_TARGET_D_BASE_NAME),
            AZStd::string::format(coveragePath.c_str(), LY_TEST_IMPACT_TEST_TARGET_D_BASE_NAME)});

        m_expectedTestTargetRuns.emplace_back(GetTestTargetATestRunSuites(), AZStd::chrono::milliseconds{500}); // TestTargetA
        m_expectedTestTargetRuns.emplace_back(GetTestTargetBTestRunSuites(), AZStd::chrono::milliseconds{500}); // TestTargetB
        m_expectedTestTargetRuns.emplace_back(GetTestTargetCTestRunSuites(), AZStd::chrono::milliseconds{500}); // TestTargetC
        m_expectedTestTargetRuns.emplace_back(GetTestTargetDTestRunSuites(), AZStd::chrono::milliseconds{500}); // TestTargetD

        // TestTargetA
        m_expectedTestTargetCoverages.emplace_back(AZStd::array<TestImpact::TestCoverage, 2>{
            TestImpact::TestCoverage(GetTestTargetALineModuleCoverages()),
            TestImpact::TestCoverage(GetTestTargetASourceModuleCoverages())});

        // TestTargetB
        m_expectedTestTargetCoverages.emplace_back(AZStd::array<TestImpact::TestCoverage, 2>{
            TestImpact::TestCoverage(GetTestTargetBLineModuleCoverages()),
            TestImpact::TestCoverage(GetTestTargetBSourceModuleCoverages())});

        // TestTargetC
        m_expectedTestTargetCoverages.emplace_back(AZStd::array<TestImpact::TestCoverage, 2>{
            TestImpact::TestCoverage(GetTestTargetCLineModuleCoverages()),
            TestImpact::TestCoverage(GetTestTargetCSourceModuleCoverages())});

        // TestTargetD
        m_expectedTestTargetCoverages.emplace_back(AZStd::array<TestImpact::TestCoverage, 2>{
            TestImpact::TestCoverage(GetTestTargetDLineModuleCoverages()),
            TestImpact::TestCoverage(GetTestTargetDSourceModuleCoverages())});

        m_expectedTestTargetResult.emplace_back(TestImpact::TestRunResult::Failed); // TestTargetA
        m_expectedTestTargetResult.emplace_back(TestImpact::TestRunResult::Passed); // TestTargetB
        m_expectedTestTargetResult.emplace_back(TestImpact::TestRunResult::Passed); // TestTargetC
        m_expectedTestTargetResult.emplace_back(TestImpact::TestRunResult::Passed); // TestTargetD

        // Generate the job command arguments for both line level and source level coverage permutations
        for (const auto& testTarget : m_testTargetPaths)
        {
            m_testTargetJobArgs.emplace_back(AZStd::array<AZStd::string, 2>{
                GetRunCommandForTargetWithSources(testTarget, CoverageLevel::LineLevel),
                GetRunCommandForTargetWithSources(testTarget, CoverageLevel::SourceLevel)});
        }
    }

    void InstrumentedTestRunnerFixture::TearDown()
    {
        DeleteFiles(LY_TEST_IMPACT_TEST_TARGET_COVERAGE_DIR, "*.xml");
        DeleteFiles(LY_TEST_IMPACT_TEST_TARGET_RESULTS_DIR, "*.xml");

        AllocatorsTestFixture::TearDown();
    }

    using ConcurrencyAndCoveragePermutation = AZStd::tuple
        <
            size_t, // Max number of concurrent processes
            CoverageLevel // Coverage level
        >;

    // Fixture parameterized for different max number of concurrent jobs and coverage levels
    class InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageParams
        : public InstrumentedTestRunnerFixture
        , public ::testing::WithParamInterface<ConcurrencyAndCoveragePermutation>
    {
    public:
        void SetUp() override
        {
            InstrumentedTestRunnerFixture::SetUp();
            auto [maxConcurrency, coverageLevel] = GetParam();
            m_maxConcurrency = maxConcurrency;
            m_coverageLevel = coverageLevel;
        }
    };

    using ConcurrencyAndJobExceptionPermutation = AZStd::tuple
        <
            size_t, // Max number of concurrent processes
            CoverageLevel, // Coverage level
            JobExceptionPolicy // Test job exception policy
        >;

    // Fixture parameterized for different max number of concurrent jobs, coverage levels and different job exception policies
    class InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndJobExceptionParams
        : public InstrumentedTestRunnerFixture
        , public ::testing::WithParamInterface<ConcurrencyAndJobExceptionPermutation>
    {
    public:
        void SetUp() override
        {
            InstrumentedTestRunnerFixture::SetUp();
            const auto& [maxConcurrency, coverageLevel, jobExceptionPolicy] = GetParam();
            m_maxConcurrency = maxConcurrency;
            m_coverageLevel = coverageLevel;
            m_jobExceptionPolicy = jobExceptionPolicy;
        }

    protected:
        JobExceptionPolicy m_jobExceptionPolicy = JobExceptionPolicy::Never;
    };

    class InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndFailedToLaunchExceptionParams
        : public InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndJobExceptionParams
    {
    };

    class InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndExecutedWithFailureExceptionParams
        : public InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndJobExceptionParams
    {
    };

    using ConcurrencyAndCoverageExceptionPermutation = AZStd::tuple
        <
            size_t, // Max number of concurrent processes
            CoverageExceptionPolicy // Test coverage exception policy
        >;

    // Fixture parameterized for different max number of concurrent jobs and different coverage exception policies
    class InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndCoverageExceptionParams
        : public InstrumentedTestRunnerFixture
        , public ::testing::WithParamInterface<ConcurrencyAndCoverageExceptionPermutation>
    {
    public:
        void SetUp() override
        {
            InstrumentedTestRunnerFixture::SetUp();
            const auto& [maxConcurrency, coverageExceptionPolicy] = GetParam();
            m_maxConcurrency = maxConcurrency;
            m_coverageExceptionPolicy = coverageExceptionPolicy;
        }

    protected:
        CoverageExceptionPolicy m_coverageExceptionPolicy = CoverageExceptionPolicy::Never;
    };

    namespace
    {
        AZStd::array<size_t, 4> MaxConcurrentRuns = {1, 2, 3, 4};

        AZStd::array<CoverageLevel, 2> CoverageLevels = {CoverageLevel::LineLevel, CoverageLevel::SourceLevel};

        AZStd::array<JobExceptionPolicy, 2> FailedToLaunchExceptionPolicies = {
            JobExceptionPolicy::Never, JobExceptionPolicy::OnFailedToExecute};

        AZStd::array<JobExceptionPolicy, 2> ExecutedWithFailureExceptionPolicies = {
            JobExceptionPolicy::Never, JobExceptionPolicy::OnExecutedWithFailure};
    } // namespace

    // Validates that the specified test coverage matches the expected output
    void ValidateTestTargetCoverage(const TestImpact::TestCoverage& actualResult, const TestImpact::TestCoverage& expectedResult)
    {
        EXPECT_TRUE(actualResult == expectedResult);
    }

    // Validates that the specified test coverage is empty
    void ValidateEmptyTestTargetCoverage(const TestImpact::TestCoverage& actualResult)
    {
        EXPECT_TRUE(actualResult.GetSourcesCovered().empty());
        EXPECT_TRUE(actualResult.GetModuleCoverages().empty());
        EXPECT_EQ(actualResult.GetNumSourcesCovered(), 0);
        EXPECT_EQ(actualResult.GetNumModulesCovered(), 0);
    }

    TEST_P(
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndCoverageExceptionParams,
        EmptyTestCoverages_ExpectEmptyTestCoveragesOrTestRunException)
    {
        // Given a test runner with no client callback or run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, m_maxConcurrency, AZStd::nullopt, AZStd::nullopt);

        // Given a mixture of instrumented test run jobs with and without coverage sources
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            const AZStd::string args = (jobId % 2) ? GetRunCommandForTargetWithoutSources(m_testTargetPaths[jobId], m_coverageLevel)
                                                   : m_testTargetJobArgs[jobId][m_coverageLevel];

            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            m_jobInfos.emplace_back(JobInfo({jobId}, args, AZStd::move(jobData)));
        }

        try
        {
            // When the instrumented test run jobs are executed with different exception policies
            const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, m_coverageExceptionPolicy, JobExceptionPolicy::Never);

            // Expect this statement to be reachable only if no exception policy for empty coverages
            EXPECT_FALSE(::IsFlagSet(m_coverageExceptionPolicy, CoverageExceptionPolicy::OnEmptyCoverage));

            for (const auto& job : runnerJobs)
            {
                const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;
                ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
                ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);

                if (jobId % 2)
                {
                    // Expect jobs have a empty test coverages
                    ValidateEmptyTestTargetCoverage(job.GetPayload().value().second);
                }
                else
                {
                    // Expect the jobs to successfully result in a test run and coverage that matches the expected test run data
                    ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
                }
            }
        }
        catch ([[maybe_unused]] const TestImpact::TestRunException& e)
        {
            // Expect this statement to be reachable only if there is an exception policy for empty coverages
            EXPECT_TRUE(::IsFlagSet(m_coverageExceptionPolicy, CoverageExceptionPolicy::OnEmptyCoverage));
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_P(
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndFailedToLaunchExceptionParams,
        InvalidCommandArgument_ExpectJobResulFailedToExecuteeOrTestJobException)
    {
        // Given a test runner with no client callback or run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, m_maxConcurrency, AZStd::nullopt, AZStd::nullopt);

        // Given a mixture of instrumented test run jobs with valid and invalid command arguments
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            const AZStd::string args = (jobId % 2) ? InvalidProcessPath : m_testTargetJobArgs[jobId][m_coverageLevel];
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            m_jobInfos.emplace_back(JobInfo({jobId}, args, AZStd::move(jobData)));
        }

        try
        {
            // When the instrumented test run jobs are executed with different exception policies
            const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, m_jobExceptionPolicy);

            // Expect this statement to be reachable only if no exception policy for launch failures
            EXPECT_FALSE(::IsFlagSet(m_jobExceptionPolicy, JobExceptionPolicy::OnFailedToExecute));

            for (const auto& job : runnerJobs)
            {
                const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;
                if (jobId % 2)
                {
                    // Expect invalid jobs have a job result of FailedToExecute
                    ValidateJobFailedToExecute(job);
                }
                else
                {
                    // Expect the valid jobs to successfully result in a test run that matches the expected test run data
                    ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
                    ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
                    ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
                }
            }
        }
        catch ([[maybe_unused]] const TestImpact::TestJobException& e)
        {
            // Expect this statement to be reachable only if there is an exception policy for launch failures
            EXPECT_TRUE(::IsFlagSet(m_jobExceptionPolicy, JobExceptionPolicy::OnFailedToExecute));
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_P(
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndExecutedWithFailureExceptionParams,
        ErroneousReturnCode_ExpectJobResultExecutedWithFailureOrTestJobException)
    {
        // Given a test runner with no client callback or run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, m_maxConcurrency, AZStd::nullopt, AZStd::nullopt);

        // Given a mixture of instrumented test run jobs that execute and return either successfully or with failure
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            m_jobInfos.emplace_back(JobInfo({jobId}, m_testTargetJobArgs[jobId][m_coverageLevel], AZStd::move(jobData)));
        }

        try
        {
            // When the instrumented test run jobs are executed with different exception policies
            const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, m_jobExceptionPolicy);

            // Expect this statement to be reachable only if no exception policy for jobs that return with error
            EXPECT_FALSE(::IsFlagSet(m_jobExceptionPolicy, JobExceptionPolicy::OnExecutedWithFailure));

            for (const auto& job : runnerJobs)
            {
                const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;

                // Expect the valid jobs to successfully result in a test run that matches the expected test run data
                ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
                ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
                ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
            }
        }
        catch ([[maybe_unused]] const TestImpact::TestJobException& e)
        {
            // Expect this statement to be reachable only if there is an exception policy for jobs that return with error
            EXPECT_TRUE(::IsFlagSet(m_jobExceptionPolicy, JobExceptionPolicy::OnExecutedWithFailure));
        }
        catch ([[maybe_unused]] const TestImpact::Exception& e)
        {
            FAIL();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(InstrumentedTestRunnerFixture, EmptyRunRawData_ExpectTestRunnerException)
    {
        // Given a test runner with no client callback, concurrency, run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, OneConcurrentProcess, AZStd::nullopt, AZStd::nullopt);

        // Given an test runner job that will return successfully but with an empty artifact string
        JobData jobData("", m_testTargetPaths[TestTargetA].m_testCoverageArtifact);
        m_jobInfos.emplace_back(JobInfo({TestTargetA}, m_testTargetJobArgs[TestTargetA][LineLevel], AZStd::move(jobData)));

        try
        {
            // When the test runner job is executed
            const auto runnerJobs =
                m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::TestRunException& e)
        {
            // Expect an runner exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(InstrumentedTestRunnerFixture, EmptyCoverageRawData_ExpectTestRunnerException)
    {
        // Given a test runner with no client callback, concurrency, run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, OneConcurrentProcess, AZStd::nullopt, AZStd::nullopt);

        // Given an test runner job that will return successfully but with an empty artifact string
        JobData jobData(m_testTargetPaths[TestTargetA].m_testRunArtifact, "");
        m_jobInfos.emplace_back(JobInfo({TestTargetA}, m_testTargetJobArgs[TestTargetA][LineLevel], AZStd::move(jobData)));

        try
        {
            // When the test runner job is executed
            const auto runnerJobs =
                m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::TestRunException& e)
        {
            // Expect an runner exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(InstrumentedTestRunnerFixture, InvalidRunArtifact_ExpectArtifactException)
    {
        // Given a run artifact with invalid contents
        WriteTextToFile("There is nothing valid here", m_testTargetPaths[TestTargetA].m_testRunArtifact);

        // Given a job command that will write the run artifact to a different location that what we will read from
        TargetPaths invalidRunArtifact = m_testTargetPaths[TestTargetA];
        invalidRunArtifact.m_testRunArtifact /= ".xml";
        const AZStd::string args = GetRunCommandForTargetWithSources(invalidRunArtifact, LineLevel);

        // Given a test runner with no client callback, concurrency, run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, OneConcurrentProcess, AZStd::nullopt, AZStd::nullopt);

        // Given an test runner job that will return successfully but not produce a run artifact
        JobData jobData(m_testTargetPaths[TestTargetA].m_testRunArtifact, m_testTargetPaths[TestTargetA].m_testCoverageArtifact);
        m_jobInfos.emplace_back(JobInfo({TestTargetA}, args, AZStd::move(jobData)));

        try
        {
            // When the test runner job is executed
            const auto runnerJobs =
                m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an runner exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_F(InstrumentedTestRunnerFixture, InvalidCoverageArtifact_ExpectArtifactException)
    {
        // Given a coverage artifact with invalid contents
        WriteTextToFile("There is nothing valid here", m_testTargetPaths[TestTargetA].m_testCoverageArtifact);

        // Given a job command that will write the coverage artifact to a different location that what we will read from
        TargetPaths invalidCoverageArtifact = m_testTargetPaths[TestTargetA];
        invalidCoverageArtifact.m_testCoverageArtifact /= ".xml";
        const AZStd::string args = GetRunCommandForTargetWithSources(invalidCoverageArtifact, LineLevel);

        // Given a test runner with no client callback, concurrency, run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, OneConcurrentProcess, AZStd::nullopt, AZStd::nullopt);

        // Given an test runner job that will return successfully but not produce a run artifact
        JobData jobData(m_testTargetPaths[TestTargetA].m_testRunArtifact, m_testTargetPaths[TestTargetA].m_testCoverageArtifact);
        m_jobInfos.emplace_back(JobInfo({TestTargetA}, args, AZStd::move(jobData)));

        try
        {
            // When the test runner job is executed
            const auto runnerJobs =
                m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

            // Do not expect this statement to be reachable
            FAIL();
        }
        catch ([[maybe_unused]] const TestImpact::ArtifactException& e)
        {
            // Expect an runner exception
            SUCCEED();
        }
        catch (...)
        {
            // Do not expect any other exceptions
            FAIL();
        }
    }

    TEST_P(InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageParams, RunTestTargets_RunsAndCoverageMatchTestSuitesInTarget)
    {
        // Given a test runner with no client callback, runner timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, m_maxConcurrency, AZStd::nullopt, AZStd::nullopt);

        // Given an test runner job for each test target with no runner caching
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            m_jobInfos.emplace_back(JobInfo({jobId}, m_testTargetJobArgs[jobId][m_coverageLevel], AZStd::move(jobData)));
        }

        // When the test runner jobs are executed
        const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

        // Expect each job to successfully result in a test runner that matches the expected test runner data for that test target
        for (const auto& job : runnerJobs)
        {
            const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;
            ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
            ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
            ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
        }
    }

    TEST_P(
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageParams,
        RunTestTargetsWithArbitraryJobIds_RunsAndCoverageMatchTestSuitesInTarget)
    {
        // Given a set of arbitrary job ids to be used for the test target jobs
        enum
        {
            ArbitraryA = 36,
            ArbitraryB = 890,
            ArbitraryC = 19,
            ArbitraryD = 1
        };

        const AZStd::unordered_map<JobInfo::IdType, JobInfo::IdType> sequentialToArbitrary =
        {
            {TestTargetA, ArbitraryA},
            {TestTargetB, ArbitraryB},
            {TestTargetC, ArbitraryC},
            {TestTargetD, ArbitraryD},
        };

        const AZStd::unordered_map<JobInfo::IdType, JobInfo::IdType> arbitraryToSequential =
        {
            {ArbitraryA, TestTargetA},
            {ArbitraryB, TestTargetB},
            {ArbitraryC, TestTargetC},
            {ArbitraryD, TestTargetD},
        };

        // Given a test runner with no client callback, run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(AZStd::nullopt, m_maxConcurrency, AZStd::nullopt, AZStd::nullopt);

        // Given an test run job for each test target
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            m_jobInfos.emplace_back(
                JobInfo({sequentialToArbitrary.at(jobId)}, m_testTargetJobArgs[jobId][m_coverageLevel], AZStd::move(jobData)));
        }

        // When the instrumented test run jobs are executed
        const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

        // Expect each job to successfully result in a test run that matches the expected test run data for that test target
        for (const auto& job : runnerJobs)
        {
            const JobInfo::IdType jobId = arbitraryToSequential.at(job.GetJobInfo().GetId().m_value);
            ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
            ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
            ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
        }
    }

    TEST_P(InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageParams, RunTestTargetsWithCallback_RunsAndCoverageMatchTestSuitesInTarget)
    {
        // Given a client callback function that tracks the number of successful runs
        size_t numSuccesses = 0;
        const auto jobCallback =
            [&numSuccesses]([[maybe_unused]] const TestImpact::InstrumentedTestRunner::JobInfo& jobInfo, const TestImpact::JobMeta& meta)
            {
                if (meta.m_result == TestImpact::JobResult::ExecutedWithSuccess)
                {
                    numSuccesses++;
                }
            };

        // Given a test runner with no run timeout or runner timeout
        m_testRunner =
            AZStd::make_unique<TestImpact::InstrumentedTestRunner>(jobCallback, m_maxConcurrency, AZStd::nullopt, AZStd::nullopt);

        // Given an test run job for each test target
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            m_jobInfos.emplace_back(JobInfo({jobId}, m_testTargetJobArgs[jobId][m_coverageLevel], AZStd::move(jobData)));
        }

        // When the instrumented test run jobs are executed
        const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

        // Expect the number of successful runs tracked in the callback to match the number of test targets run with no failures
        EXPECT_EQ(
            numSuccesses,
            AZStd::count_if(m_expectedTestTargetResult.begin(), m_expectedTestTargetResult.end(), [](TestImpact::TestRunResult result) {
                return result == TestImpact::TestRunResult::Passed;
            }));

        // Expect each job to successfully result in a test run that matches the expected test run data for that test target
        for (const auto& job : runnerJobs)
        {
            const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;
            ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
            ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
            ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
        }
    }

    TEST_P(InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageParams, JobRunnerTimeout_InFlightJobsTimeoutAndQueuedJobsUnlaunched)
    {
        // Given a test runner with no client callback or runner timeout and 2 second run timeout
        m_testRunner = AZStd::make_unique<TestImpact::InstrumentedTestRunner>(
            AZStd::nullopt, m_maxConcurrency, AZStd::chrono::seconds(2), AZStd::nullopt);

        // Given an test run job for each test target where half will sleep indefinitely
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            const AZStd::string args = (jobId % 2)
                ? AZStd::string::format("%s %s", ValidProcessPath, ConstructTestProcessArgs(jobId, LongSleep).c_str())
                : m_testTargetJobArgs[jobId][m_coverageLevel];
            m_jobInfos.emplace_back(JobInfo({jobId}, args, AZStd::move(jobData)));
        }

        // When the instrumented test run jobs are executed
        const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

        // Expect half the jobs to successfully result in a test run that matches the expected test run data for that test target
        // with the other half having timed out
        for (const auto& job : runnerJobs)
        {
            const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;
            if (jobId % 2)
            {
                ValidateJobTimeout(job);
            }
            else
            {
                ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
                ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
                ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
            }
        }
    }

    TEST_F(InstrumentedTestRunnerFixture, JobTimeout_InFlightJobTimeoutAndQueuedJobsUnlaunched)
    {
        // Given a test runner with no client callback or run timeout and a 5 second runner timeout
        m_testRunner = AZStd::make_unique<TestImpact::InstrumentedTestRunner>(
            AZStd::nullopt, FourConcurrentProcesses, AZStd::nullopt, AZStd::chrono::seconds(5));

        // Given an test run job for each test target where half will sleep indefinitely
        for (size_t jobId = 0; jobId < m_testTargetJobArgs.size(); jobId++)
        {
            JobData jobData(m_testTargetPaths[jobId].m_testRunArtifact, m_testTargetPaths[jobId].m_testCoverageArtifact);
            const AZStd::string args = (jobId % 2)
                ? AZStd::string::format("%s %s", ValidProcessPath, ConstructTestProcessArgs(jobId, LongSleep).c_str())
                : m_testTargetJobArgs[jobId][m_coverageLevel];
            m_jobInfos.emplace_back(JobInfo({jobId}, args, AZStd::move(jobData)));
        }

        // When the instrumented test run jobs are executed
        const auto runnerJobs = m_testRunner->RunInstrumentedTests(m_jobInfos, CoverageExceptionPolicy::Never, JobExceptionPolicy::Never);

        // Expect half the jobs to successfully result in a test run that matches the expected test run data for that test target
        // with the other half having timed out
        for (const auto& job : runnerJobs)
        {
            const JobInfo::IdType jobId = job.GetJobInfo().GetId().m_value;
            if (jobId % 2)
            {
                ValidateJobTimeout(job);
            }
            else
            {
                ValidateTestRunCompleted(job, m_expectedTestTargetResult[jobId]);
                ValidateTestTargetRun(job.GetPayload().value().first, m_expectedTestTargetRuns[jobId]);
                ValidateTestTargetCoverage(job.GetPayload().value().second, m_expectedTestTargetCoverages[jobId][m_coverageLevel]);
            }
        }
    }

    INSTANTIATE_TEST_CASE_P(
        ,
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndCoverageExceptionParams,
        ::testing::Combine(
            ::testing::ValuesIn(MaxConcurrentRuns),
            ::testing::Values(CoverageExceptionPolicy::Never, CoverageExceptionPolicy::OnEmptyCoverage)));

    INSTANTIATE_TEST_CASE_P(
        ,
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndFailedToLaunchExceptionParams,
        ::testing::Combine(
            ::testing::ValuesIn(MaxConcurrentRuns), ::testing::ValuesIn(CoverageLevels),
            ::testing::ValuesIn(FailedToLaunchExceptionPolicies)));

    INSTANTIATE_TEST_CASE_P(
        ,
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageAndExecutedWithFailureExceptionParams,
        ::testing::Combine(
            ::testing::ValuesIn(MaxConcurrentRuns), ::testing::ValuesIn(CoverageLevels),
            ::testing::ValuesIn(ExecutedWithFailureExceptionPolicies)));

    INSTANTIATE_TEST_CASE_P(
        ,
        InstrumentedTestRunnerFixtureWithConcurrencyAndCoverageParams,
        ::testing::Combine(::testing::ValuesIn(MaxConcurrentRuns), ::testing::ValuesIn(CoverageLevels)));
} // namespace UnitTest
