/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <QTemporaryDir>
#include <AzTest/AzTest.h>
#include <AzCore/UnitTest/TestTypes.h>
#include "AzToolsFramework/API/AssetDatabaseBus.h"
#include "AssetDatabase/AssetDatabase.h"
#include <AssetManager/PathDependencyManager.h>

namespace UnitTests
{
    class MockDatabaseLocationListener : public AzToolsFramework::AssetDatabase::AssetDatabaseRequests::Bus::Handler
    {
    public:
        bool GetAssetDatabaseLocation(AZStd::string& location) override
        {
            location = m_databaseLocation;
            return true;
        }

        AZStd::string m_databaseLocation;
    };

    namespace Util
    {
        using namespace AzToolsFramework::AssetDatabase;

        void CreateSourceJobAndProduct(AssetProcessor::AssetDatabaseConnection* stateData, AZ::s64 scanfolderPk, SourceDatabaseEntry& source, JobDatabaseEntry& job, ProductDatabaseEntry& product, const char* sourceName, const char* productName)
        {
            source = SourceDatabaseEntry(scanfolderPk, sourceName, AZ::Uuid::CreateRandom(), "fingerprint");
            EXPECT_TRUE(stateData->SetSource(source));

            job = JobDatabaseEntry(source.m_sourceID, "jobkey", 1111, "pc", AZ::Uuid::CreateRandom(), AzToolsFramework::AssetSystem::JobStatus::Completed, 4444);
            EXPECT_TRUE(stateData->SetJob(job));

            product = ProductDatabaseEntry(job.m_jobID, 0, productName, AZ::Data::AssetType::CreateRandom());
            EXPECT_TRUE(stateData->SetProduct(product));
        }
    }

    struct PathDependencyBase
        : UnitTest::TraceBusRedirector
    {
        void Init();
        void Destroy();

        QTemporaryDir m_tempDir;
        AZStd::string m_databaseLocation;
        MockDatabaseLocationListener m_databaseLocationListener;
        AZStd::shared_ptr<AssetProcessor::AssetDatabaseConnection> m_stateData;
        AZStd::unique_ptr<AssetProcessor::PlatformConfiguration> m_platformConfig;
        AZ::JobManager* m_jobManager{};
        AZ::JobContext* m_jobContext{};
    };

    struct PathDependencyDeletionTest
        : UnitTest::ScopedAllocatorSetupFixture
        , PathDependencyBase
    {
        void SetUp() override
        {
            PathDependencyBase::Init();
        }

        void TearDown() override
        {
            PathDependencyBase::Destroy();
        }
    };

    void PathDependencyBase::Init()
    {
        using namespace ::testing;
        using namespace AzToolsFramework::AssetDatabase;

        ::UnitTest::TestRunner::Instance().m_suppressAsserts = false;
        ::UnitTest::TestRunner::Instance().m_suppressErrors = false;

        BusConnect();

        QDir tempPath(m_tempDir.path());

        m_databaseLocationListener.BusConnect();

        // in other unit tests we may open the database called ":memory:" to use an in-memory database instead of one on disk.
        // in this test, however, we use a real database, because the file processor shares it and opens its own connection to it.
        // ":memory:" databases are one-instance-only, and even if another connection is opened to ":memory:" it would
        // not share with others created using ":memory:" and get a unique database instead.
        m_databaseLocation = tempPath.absoluteFilePath("test_database.sqlite").toUtf8().constData();
        m_databaseLocationListener.m_databaseLocation = m_databaseLocation;

        m_stateData = AZStd::shared_ptr<AssetProcessor::AssetDatabaseConnection>(new AssetProcessor::AssetDatabaseConnection());
        m_stateData->OpenDatabase();

        m_platformConfig = AZStd::make_unique<AssetProcessor::PlatformConfiguration>();

        AZ::AllocatorInstance<AZ::PoolAllocator>::Create();
        AZ::AllocatorInstance<AZ::ThreadPoolAllocator>::Create();

        AZ::JobManagerDesc jobDesc;
        AZ::JobManagerThreadDesc threadDesc;
        for (int i = 0; i < 16; ++i)
        {
            jobDesc.m_workerThreads.push_back(threadDesc);
        }

        m_jobManager = aznew AZ::JobManager(jobDesc);
        m_jobContext = aznew AZ::JobContext(*m_jobManager);
        AZ::JobContext::SetGlobalContext(m_jobContext);
    }

    void PathDependencyBase::Destroy()
    {
        m_stateData = nullptr;
        m_platformConfig = nullptr;

        AZ::JobContext::SetGlobalContext(nullptr);
        delete m_jobContext;
        delete m_jobManager;

        AZ::AllocatorInstance<AZ::ThreadPoolAllocator>::Destroy();
        AZ::AllocatorInstance<AZ::PoolAllocator>::Destroy();

        BusDisconnect();
    }

    TEST_F(PathDependencyDeletionTest, ExistingSourceWithUnmetDependency_RemovedFromDB_DependentSourceCreatedWithoutError)
    {
        using namespace AzToolsFramework::AssetDatabase;

        // Add a product to the db with an unmet dependency
        ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
        m_stateData->SetScanFolder(scanFolder);

        SourceDatabaseEntry source1, source2;
        JobDatabaseEntry job1, job2;
        ProductDatabaseEntry product1, product2;

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source1, job1, product1, "source1.txt", "product1.jpg");

        ProductDependencyDatabaseEntry dependency(product1.m_productID, AZ::Uuid::CreateRandom(), 0, 0, "pc", 0, "source2.txt", ProductDependencyDatabaseEntry::DependencyType::ProductDep_SourceFile);
        m_stateData->SetProductDependency(dependency);

        AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

        // Delete the data from the database
        m_stateData->RemoveSource(source1.m_sourceID);

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source2, job2, product2, "source2.txt", "product2.jpg");

        manager.QueueSourceForDependencyResolution(source2);
        manager.ProcessQueuedDependencyResolves();
    }

    TEST_F(PathDependencyDeletionTest, ExistingSourceWithUnmetDependency_RemovedFromDB_DependentProductCreatedWithoutError)
    {
        using namespace AzToolsFramework::AssetDatabase;

        // Add a product to the db with an unmet dependency
        ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
        m_stateData->SetScanFolder(scanFolder);

        SourceDatabaseEntry source1, source2;
        JobDatabaseEntry job1, job2;
        ProductDatabaseEntry product1, product2;

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source1, job1, product1, "source1.txt", "product1.jpg");

        ProductDependencyDatabaseEntry dependency(product1.m_productID, AZ::Uuid::CreateRandom(), 0, 0, "pc", 0, "product2.jpg", ProductDependencyDatabaseEntry::DependencyType::ProductDep_ProductFile);
        m_stateData->SetProductDependency(dependency);

        AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

        // Delete the data from the database
        m_stateData->RemoveSource(source1.m_sourceID);

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source2, job2, product2, "source2.txt", "product2.jpg");

        manager.QueueSourceForDependencyResolution(source2);
        manager.ProcessQueuedDependencyResolves();
    }

    TEST_F(PathDependencyDeletionTest, NewSourceWithUnmetDependency_RemovedFromDB_DependentSourceCreatedWithoutError)
    {
        using namespace AzToolsFramework::AssetDatabase;

        AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

        // Add a product to the db with an unmet dependency
        ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
        m_stateData->SetScanFolder(scanFolder);

        SourceDatabaseEntry source1, source2;
        JobDatabaseEntry job1, job2;
        ProductDatabaseEntry product1, product2;

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source1, job1, product1, "source1.txt", "product1.jpg");

        AssetBuilderSDK::ProductPathDependencySet set;
        set.insert(AssetBuilderSDK::ProductPathDependency("source2.txt", AssetBuilderSDK::ProductPathDependencyType::SourceFile));

        manager.SaveUnresolvedDependenciesToDatabase(set, product1, "pc");

        // Delete the data from the database
        m_stateData->RemoveSource(source1.m_sourceID);

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source2, job2, product2, "source2.txt", "product2.jpg");

        manager.QueueSourceForDependencyResolution(source2);
        manager.ProcessQueuedDependencyResolves();
    }

    TEST_F(PathDependencyDeletionTest, NewSourceWithUnmetDependency_RemovedFromDB_DependentProductCreatedWithoutError)
    {
        using namespace AzToolsFramework::AssetDatabase;

        AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

        // Add a product to the db with an unmet dependency
        ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
        m_stateData->SetScanFolder(scanFolder);

        SourceDatabaseEntry source1, source2;
        JobDatabaseEntry job1, job2;
        ProductDatabaseEntry product1, product2;

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source1, job1, product1, "source1.txt", "product1.jpg");

        AssetBuilderSDK::ProductPathDependencySet set;
        set.insert(AssetBuilderSDK::ProductPathDependency("product2.jpg", AssetBuilderSDK::ProductPathDependencyType::ProductFile));

        manager.SaveUnresolvedDependenciesToDatabase(set, product1, "pc");

        // Delete the data from the database
        m_stateData->RemoveSource(source1.m_sourceID);

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source2, job2, product2, "source2.txt", "product2.jpg");

        manager.QueueSourceForDependencyResolution(source2);
        manager.ProcessQueuedDependencyResolves();
    }

    TEST_F(PathDependencyDeletionTest, NewSourceWithUnmetDependency_Wildcard_RemovedFromDB_DependentSourceCreatedWithoutError)
    {
        using namespace AzToolsFramework::AssetDatabase;

        AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

        // Add a product to the db with an unmet dependency
        ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
        m_stateData->SetScanFolder(scanFolder);

        SourceDatabaseEntry source1, source2;
        JobDatabaseEntry job1, job2;
        ProductDatabaseEntry product1, product2;

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source1, job1, product1, "source1.txt", "product1.jpg");

        AssetBuilderSDK::ProductPathDependencySet set;
        set.insert(AssetBuilderSDK::ProductPathDependency("sou*ce2.txt", AssetBuilderSDK::ProductPathDependencyType::SourceFile));

        manager.SaveUnresolvedDependenciesToDatabase(set, product1, "pc");

        // Delete the data from the database
        m_stateData->RemoveSource(source1.m_sourceID);

        Util::CreateSourceJobAndProduct(m_stateData.get(), scanFolder.m_scanFolderID, source2, job2, product2, "source2.txt", "product2.jpg");

        manager.QueueSourceForDependencyResolution(source2);
        manager.ProcessQueuedDependencyResolves();
    }

    using PathDependencyTests = PathDependencyDeletionTest;

    TEST_F(PathDependencyTests, SourceAndProductHaveSameName_OnlyOneEntryIsSavedToDatabase)
    {
        AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

        ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
        m_stateData->SetScanFolder(scanFolder);

        SourceDatabaseEntry source1, source2;
        JobDatabaseEntry job1, job2;
        ProductDatabaseEntry product1, product2;

        Util::CreateSourceJobAndProduct(
            m_stateData.get(), scanFolder.m_scanFolderID, source1, job1, product1, "source1.txt", "product1.jpg");

        AssetBuilderSDK::ProductPathDependencySet set;
        set.insert(AssetBuilderSDK::ProductPathDependency("*.xml", AssetBuilderSDK::ProductPathDependencyType::SourceFile));

        manager.SaveUnresolvedDependenciesToDatabase(set, product1, "pc");

        Util::CreateSourceJobAndProduct(
            m_stateData.get(), scanFolder.m_scanFolderID, source2, job2, product2, "source2.xml", "source2.xml");

        manager.QueueSourceForDependencyResolution(source2);
        manager.ProcessQueuedDependencyResolves();

        ProductDependencyDatabaseEntryContainer productDependencies;
        m_stateData->GetProductDependencies(productDependencies);

        EXPECT_EQ(productDependencies.size(), 2);
    }

    struct PathDependencyBenchmarks
        : UnitTest::ScopedAllocatorFixture
          , PathDependencyBase
    {
        static inline constexpr int NumTestDependencies = 4; // Must be a multiple of 4
        static inline constexpr int NumTestProducts = 2; // Must be a multiple of 2

        ProductDatabaseEntryContainer m_products;
        SourceDatabaseEntry m_source1, m_source2, m_source4;
        JobDatabaseEntry m_job1, m_job2, m_job4;
        ProductDatabaseEntry m_product1, m_product2, m_product4;
        ProductDependencyDatabaseEntryContainer m_dependencies;

        void SetupTestData()
        {
            ScanFolderDatabaseEntry scanFolder("folder", "test", "test", 0);
            m_stateData->SetScanFolder(scanFolder);

            Util::CreateSourceJobAndProduct(
                m_stateData.get(), scanFolder.m_scanFolderID, m_source1, m_job1, m_product1, "source1.txt", "product1.jpg");

            Util::CreateSourceJobAndProduct(
                m_stateData.get(), scanFolder.m_scanFolderID, m_source4, m_job4, m_product4, "source4.txt", "product4.jpg");

            for (int i = 0; i < NumTestDependencies / 2; ++i)
            {
                m_dependencies.emplace_back(
                    m_product1.m_productID, AZ::Uuid::CreateNull(), 0, 0, "pc", 0,
                    AZStd::string::format("folder/folder2/%d_*2.jpg", i).c_str());
                ++i;
                m_dependencies.emplace_back(
                    m_product1.m_productID, AZ::Uuid::CreateNull(), 0, 0, "mac", 0,
                    AZStd::string::format("folder/folder2/%d_*2.jpg", i).c_str());
            }

            for (int i = 0; i < NumTestDependencies / 2; ++i)
            {
                m_dependencies.emplace_back(
                    m_product4.m_productID, AZ::Uuid::CreateNull(), 0, 0, "pc", 0,
                    AZStd::string::format("folder/folder2/%d_*2.jpg", i).c_str());
                ++i;
                m_dependencies.emplace_back(
                    m_product4.m_productID, AZ::Uuid::CreateNull(), 0, 0, "mac", 0,
                    AZStd::string::format("folder/folder2/%d_*2.jpg", i).c_str());
            }

            m_stateData->SetProductDependencies(m_dependencies);

            Util::CreateSourceJobAndProduct(
                m_stateData.get(), scanFolder.m_scanFolderID, m_source2, m_job2, m_product2, "source2.txt", "product2.jpg");

            auto job3 = JobDatabaseEntry(
                m_source2.m_sourceID, "jobkey", 1111, "mac", AZ::Uuid::CreateRandom(), AzToolsFramework::AssetSystem::JobStatus::Completed,
                4444);
            EXPECT_TRUE(m_stateData->SetJob(job3));

            for (int i = 0; i < NumTestProducts; ++i)
            {
                m_products.emplace_back(
                    m_job2.m_jobID, i, AZStd::string::format("pc/folder/folder2/%d_product2.jpg", i).c_str(),
                    AZ::Data::AssetType::CreateRandom());
                ++i;
                m_products.emplace_back(
                    job3.m_jobID, i, AZStd::string::format("mac/folder/folder2/%d_product2.jpg", i).c_str(),
                    AZ::Data::AssetType::CreateRandom());
            }

            m_stateData->SetProducts(m_products);
        }

        void DoTest()
        {
            AssetProcessor::PathDependencyManager manager(m_stateData, m_platformConfig.get());

            manager.QueueSourceForDependencyResolution(m_source2);
            manager.ProcessQueuedDependencyResolves();
        }

        void VerifyResult()
        {
            ProductDependencyDatabaseEntryContainer productDependencies;
            m_stateData->GetProductDependencies(productDependencies);

            for (int i = 0; i < NumTestDependencies / 2 && i < NumTestProducts; ++i)
            {
                const auto& product = m_products[i];
                int found = 0;

                for (const auto& unresolvedProductDependency : productDependencies)
                {
                    if (unresolvedProductDependency.m_dependencySourceGuid == m_source2.m_sourceGuid &&
                        unresolvedProductDependency.m_dependencySubID == product.m_subID &&
                        unresolvedProductDependency.m_productPK == m_product1.m_productID)
                    {
                        ++found;
                    }

                    if (unresolvedProductDependency.m_dependencySourceGuid == m_source2.m_sourceGuid &&
                        unresolvedProductDependency.m_dependencySubID == product.m_subID &&
                        unresolvedProductDependency.m_productPK == m_product4.m_productID)
                    {
                        ++found;
                    }

                    if (found == 2)
                        break;
                }

                EXPECT_TRUE(found == 2) << product.m_productName.c_str() << " was not found";
            }

            EXPECT_EQ(productDependencies.size(), NumTestDependencies * 2);
        }
    };

    // For some reason, BENCHMARK_F doesn't seem to call the destructor
    // So we'll wrap the class and handle the new/delete ourselves
    struct PathDependencyBenchmarksWrapperClass : public ::benchmark::Fixture
    {
        void SetUp([[maybe_unused]] const benchmark::State& st) override
        {
            m_benchmarks = new PathDependencyBenchmarks();
            m_benchmarks->Init();
            m_benchmarks->SetupTestData();
        }

        void SetUp([[maybe_unused]] benchmark::State& st) override
        {
            m_benchmarks = new PathDependencyBenchmarks();
            m_benchmarks->Init();
            m_benchmarks->SetupTestData();
        }

        void TearDown([[maybe_unused]] benchmark::State& st) override
        {
            m_benchmarks->Destroy();
            delete m_benchmarks;
        }

        void TearDown([[maybe_unused]] const benchmark::State& st) override
        {
            m_benchmarks->Destroy();
            delete m_benchmarks;
        }

        PathDependencyBenchmarks* m_benchmarks = {};
    };

    struct PathDependencyTestValidation
        : PathDependencyBenchmarks, ::testing::Test
    {
        void SetUp() override
        {
            PathDependencyBase::Init();
        }
        void TearDown() override
        {
            PathDependencyBase::Destroy();
        }
    };

    TEST_F(PathDependencyTestValidation, DeferredWildcardDependencyResolution)
    {
        SetupTestData();
        DoTest();
        VerifyResult();
    }

    BENCHMARK_F(PathDependencyBenchmarksWrapperClass, BM_DeferredWildcardDependencyResolution)(benchmark::State& state)
    {
        for (auto _ : state)
        {
            m_benchmarks->m_stateData->SetProductDependencies(m_benchmarks->m_dependencies);

            m_benchmarks->DoTest();
        }
    }

}
