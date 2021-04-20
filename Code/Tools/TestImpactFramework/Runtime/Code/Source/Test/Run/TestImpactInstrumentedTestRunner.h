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

#include <Test/Job/TestImpactTestJobRunner.h>
#include <Test/Run/TestImpactTestCoverage.h>
#include <Test/Run/TestImpactTestRun.h>
#include <Test/Run/TestImpactTestRunJobData.h>

namespace TestImpact
{
    //! Per-job data for instrumented test runs.
    class InstrumentedTestRunJobData : public TestRunJobData
    {
    public:
        InstrumentedTestRunJobData(const AZ::IO::Path& resultsArtifact, const AZ::IO::Path& coverageArtifact);

        //! Returns the path to the coverage artifact produced by the test target.
        const AZ::IO::Path& GetCoverageArtifactPath() const;

    private:
        AZ::IO::Path m_coverageArtifact; //!< Path to coverage data.
    };

    namespace Bitwise
    {
        //! Exception policy for test coverage artifacts.
        enum class CoverageExceptionPolicy
        {
            Never = 0, //! Never throw.
            OnEmptyCoverage = 1 //! Throw when no coverage data was produced.
        };
    } // namespace Bitwise

    //! Runs a batch of test targets to determine the test coverage and passes/failures.
    class InstrumentedTestRunner : public TestJobRunner<InstrumentedTestRunJobData, AZStd::pair<TestRun, TestCoverage>>
    {
        using JobRunner = TestJobRunner<InstrumentedTestRunJobData, AZStd::pair<TestRun, TestCoverage>>;

    public:
        using CoverageExceptionPolicy = Bitwise::CoverageExceptionPolicy;

        //! Constructs an instrumented test runner with the specified parameters common to all job runs of this runner.
        //! @param clientCallback The optional client callback to be called whenever a run job changes state.
        //! @param maxConcurrentRuns The maximum number of runs to be in flight at any given time.
        //! @param runTimeout The maximum duration a run may be in-flight for before being forcefully terminated.
        //! @param runnerTimeout The maximum duration the runner may run before forcefully terminating all in-flight runs.
        InstrumentedTestRunner(
            AZStd::optional<ClientJobCallback> clientCallback, size_t maxConcurrentRuns,
            AZStd::optional<AZStd::chrono::milliseconds> runTimeout, AZStd::optional<AZStd::chrono::milliseconds> runnerTimeout);

        //! Executes the specified instrumented test run jobs according to the specified job exception policies.
        //! @param jobInfos The test run jobs to execute.
        //! @param CoverageExceptionPolicy The coverage exception policy to be used for this run.
        //! @param jobExceptionPolicy The test run job exception policy to be used for this run (use
        //! TestJobExceptionPolicy::OnFailedToExecute to throw on test failures).
        //! @return the instrumented test run jobs with their associated test run and test coverage payloads.
        AZStd::vector<Job> RunInstrumentedTests(
            const AZStd::vector<JobInfo>& jobInfos, CoverageExceptionPolicy coverageExceptionPolicy, JobExceptionPolicy jobExceptionPolicy);
    };
} // namespace TestImpact
