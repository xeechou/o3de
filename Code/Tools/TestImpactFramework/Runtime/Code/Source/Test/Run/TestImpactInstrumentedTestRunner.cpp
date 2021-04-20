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

#include <Artifact/Factory/TestImpactModuleCoverageFactory.h>
#include <Artifact/Factory/TestImpactTestRunSuiteFactory.h>
#include <Test/Job/TestImpactTestJobCommon.h>
#include <Test/Run/TestImpactInstrumentedTestRunner.h>
#include <Test/Run/TestImpactTestRunException.h>
#include <Test/Run/TestImpactTestRunSerializer.h>

#include <AzCore/IO/SystemFile.h>

namespace TestImpact
{
    InstrumentedTestRunJobData::InstrumentedTestRunJobData(const AZ::IO::Path& resultsArtifact, const AZ::IO::Path& coverageArtifact)
        : TestRunJobData(resultsArtifact)
        , m_coverageArtifact(coverageArtifact)
    {
    }

    const AZ::IO::Path& InstrumentedTestRunJobData::GetCoverageArtifactPath() const
    {
        return m_coverageArtifact;
    }

    InstrumentedTestRunner::JobPayload ParseTestRunAndCoverageFiles(
        const AZ::IO::Path& runFile,
        const AZ::IO::Path& coverageFile,
        AZStd::chrono::milliseconds duration,
        InstrumentedTestRunner::CoverageExceptionPolicy coverageExceptionPolicy)
    {
        TestRun run(GTest::TestRunSuitesFactory(ReadFileContents<TestRunException>(runFile)), duration);
        AZStd::vector<ModuleCoverage> moduleCoverages = Cobertura::ModuleCoveragesFactory(ReadFileContents<TestRunException>(coverageFile));
        if (moduleCoverages.empty())
        {
            AZ_TestImpact_Eval(
                !IsFlagSet(coverageExceptionPolicy, Bitwise::CoverageExceptionPolicy::OnEmptyCoverage), TestRunException,
                "No coverage data generated");
        }

        TestCoverage coverage(AZStd::move(moduleCoverages));
        return {AZStd::move(run), AZStd::move(coverage)};
    }

    InstrumentedTestRunner::InstrumentedTestRunner(
        AZStd::optional<ClientJobCallback> clientCallback,
        size_t maxConcurrentRuns,
        AZStd::optional<AZStd::chrono::milliseconds> runTimeout,
        AZStd::optional<AZStd::chrono::milliseconds> runnerTimeout)
        : JobRunner(clientCallback, AZStd::nullopt, StdOutputRouting::None, StdErrorRouting::None, maxConcurrentRuns, runTimeout, runnerTimeout)
    {
    }

    AZStd::vector<InstrumentedTestRunner::Job> InstrumentedTestRunner::RunInstrumentedTests(
        const AZStd::vector<JobInfo>& jobInfos,
        CoverageExceptionPolicy coverageExceptionPolicy,
        JobExceptionPolicy jobExceptionPolicy)
    {
        const auto payloadGenerator = [this, coverageExceptionPolicy](const JobDataMap& jobDataMap)
        {
            PayloadMap<Job> runs;
            for (const auto& [jobId, jobData] : jobDataMap)
            {
                const auto& [meta, jobInfo] = jobData;
                if (meta.m_result == JobResult::ExecutedWithSuccess || meta.m_result == JobResult::ExecutedWithFailure)
                {
                    runs[jobId] = ParseTestRunAndCoverageFiles(
                        jobInfo->GetRunArtifactPath(),
                        jobInfo->GetCoverageArtifactPath(),
                        meta.m_duration.value(),
                        coverageExceptionPolicy);
                }
            }

            return runs;
        };

        return ExecuteJobs(jobInfos, payloadGenerator, jobExceptionPolicy);
    }
} // namespace TestImpact
