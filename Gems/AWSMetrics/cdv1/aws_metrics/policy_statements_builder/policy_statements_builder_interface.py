"""
Copyright (c) Contributors to the Open 3D Engine Project.
For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT
"""

from __future__ import annotations
import typing

from aws_metrics.aws_metrics_stack import AWSMetricsStack
from aws_metrics.real_time_data_processing import RealTimeDataProcessing
from aws_metrics.data_ingestion import DataIngestion
from aws_metrics.dashboard import Dashboard
from aws_metrics.data_lake_integration import DataLakeIntegration
from aws_metrics.batch_processing import BatchProcessing
from aws_metrics.batch_analytics import BatchAnalytics


class PolicyStatementsBuilderInterface:
    """
    Build the policy statement list for the AWSMetrics gem
    """

    def __init__(self):
        self._policy_statement_mapping = dict()

    def add_aws_metrics_stack_policy_statements(self, component: AWSMetricsStack) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statements related with the CloudFormation stack for basic users.

        :param component: CloudFormation stack created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def add_data_ingestion_policy_statements(self, component: DataIngestion) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statement related with the data ingestion component for basic users.

        :param component: Data ingestion component created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def add_real_time_data_processing_policy_statements(
            self,
            component: RealTimeDataProcessing) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statements related with the real-time data processing component for basic users.

        :param component: Real-time data processing component created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def add_dashboard_policy_statements(self, component: Dashboard) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statements related with the CloudWatch dashboard component for basic users.

        :param component: CloudWatch dashboard component created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def add_data_lake_integration_policy_statements(
            self,
            component: DataLakeIntegration) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statements related with the data lake integration component for basic users.

        :param component: Data lake integration component created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def add_batch_processing_policy_statements(self, component: BatchProcessing) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statements related with the batch processing component for basic users.

        :param component: Batch processing component created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def add_batch_analytics_policy_statements(self, component: BatchAnalytics) -> PolicyStatementsBuilderInterface:
        """
        Add the policy statements related with the batch analytics component for basic users.

        :param component: Batch analytics component created by the metrics gem.
        :return: The policy statement builder itself.
        """
        return self

    def build(self) -> typing.List:
        """
        Retrieve the policy statement list generated by the builder.

        :return: The policy statement list.
        """
        return [value for key, value in self._policy_statement_mapping.items()]
