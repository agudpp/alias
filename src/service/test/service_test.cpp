#include <ostream>
#include <istream>
#include <sstream>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include <toolbox/os/os_helper.h>
#include <toolbox/debug/debug.h>

#include <data/data_mapper.h>
#include <storage/file_storage.h>
#include <service/service_api.h>


constexpr const char* FOLDER_DB_PATH = "./db/";


// The fixture for testing class Foo.
class ServiceTest : public ::testing::Test {
 protected:
  inline ServiceTest() {}
  inline ~ServiceTest() override {}

  void SetUp() override
  {
    toolbox::OSHelper::deleteFolder(FOLDER_DB_PATH);
    toolbox::OSHelper::createFolder(FOLDER_DB_PATH, false);
  }

  void TearDown() override
  {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }
};

struct Context {
    data::DataMapper::Ptr data_mapper;
    storage::DataStorage::Ptr storage;
    service::ServiceAPI::Ptr api;
};


static Context
context()
{
  Context result;
  result.storage.reset(new storage::FileStorage(FOLDER_DB_PATH));
  result.data_mapper.reset(new data::DataMapper());
  result.api.reset(new service::ServiceAPI(result.data_mapper, result.storage));
  return result;
}

TEST_F(ServiceTest, AddTagWorks)
{
  Context ctx = context();
  auto t1 = ctx.api->createTag("t1");
  EXPECT_TRUE(t1.get() != nullptr);

  service::SearchContext sc;
  sc.query = "t1";
  service::TagSearchReslut tsr;
  EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
  EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>({t1}));
}

TEST_F(ServiceTest, ExpandTagWorks)
{
  Context ctx = context();
  auto t1 = ctx.api->createTag("t1");
  auto t11 = ctx.api->createTag("t11");
  auto t2 = ctx.api->createTag("t2");

  service::SearchContext sc;
  sc.query = "t1";
  service::TagSearchReslut tsr;
  EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
  EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>({t1, t11}));
  sc.query = "t";
  EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
  EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>({t1, t11, t2}));
  sc.query = "t2";
  EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
  EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>({t2}));
}

TEST_F(ServiceTest, ExpandTagWorksWithSelectedTags)
{
  Context ctx = context();
  auto t1 = ctx.api->createTag("t1");
  auto t11 = ctx.api->createTag("t11");
  auto t2 = ctx.api->createTag("t2");
  auto c1 = ctx.api->createContent(data::ContentType::TEXT, false, "c1", {t1->id(), t11->id()});
  auto c2 = ctx.api->createContent(data::ContentType::TEXT, false, "c2", {t2->id(), t11->id()});

  service::SearchContext sc;
  sc.query = "t";
  sc.tags.insert(t1);
  {
    service::TagSearchReslut tsr;
    EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
    EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>({t11}));
  }
  {
    service::TagSearchReslut tsr;
    sc.query = "t";
    sc.tags = {t2};
    EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
    EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>({t11}));
  }
  {
    service::TagSearchReslut tsr;
    sc.query = "t2";
    EXPECT_TRUE(ctx.api->searchTags(sc, tsr));
    EXPECT_EQ(tsr.expanded_tags, std::set<data::Tag::ConstPtr>());
  }
}

TEST_F(ServiceTest, SearchContentWorksWithSelectedTags)
{
  Context ctx = context();
  auto t1 = ctx.api->createTag("t1");
  auto t11 = ctx.api->createTag("t11");
  auto t2 = ctx.api->createTag("t2");
  auto c1 = ctx.api->createContent(data::ContentType::TEXT, false, "c1", {t1->id(), t11->id()});
  auto c2 = ctx.api->createContent(data::ContentType::TEXT, false, "c2", {t2->id(), t11->id()});

  service::SearchContext sc;
  sc.query = "t";
  sc.tags.insert(t11);
  {
    service::ContentSearchResult csr;
    EXPECT_TRUE(ctx.api->searchContent(sc, csr));
    EXPECT_EQ(csr.tagged_contents, std::set<data::Content::ConstPtr>({c1, c2}));
    EXPECT_EQ(csr.exp_results.size(), 2);
  }

  {
    service::ContentSearchResult csr;
    sc.query = "t11";
    EXPECT_TRUE(ctx.api->searchContent(sc, csr));
    EXPECT_EQ(csr.tagged_contents, std::set<data::Content::ConstPtr>({c1, c2}));
    EXPECT_EQ(csr.exp_results.size(), 0);
  }

  {
    service::ContentSearchResult csr;
    sc.query = "t2";
    sc.tags.clear();
    EXPECT_TRUE(ctx.api->searchContent(sc, csr));
    EXPECT_EQ(csr.tagged_contents, std::set<data::Content::ConstPtr>({}));
    EXPECT_EQ(csr.exp_results.size(), 1);
    EXPECT_TRUE(csr.exp_results.find(t2) != csr.exp_results.end());
  }
}


int main(int argc, char **argv)
{
  _CONFIG_BASIC_LOGGERS;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
