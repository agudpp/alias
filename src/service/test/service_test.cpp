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

static data::Tag::Ptr
tag(const std::string& name)
{
  return data::Tag::Ptr(new data::Tag(name));
}

static data::Content::Ptr
content(const std::string& data, const std::set<toolbox::UID>& tag_ids = {})
{
  data::Content::Ptr result(new data::Content());
  result->setData(data);
  result->setTagIDs(tag_ids);
  return result;
}

static std::set<toolbox::UID>
ids(const std::vector<data::Tag::Ptr>& ts)
{
  std::set<toolbox::UID> result;
  for (auto& t : ts) {
    result.insert(t->id());
  }
  return result;
}


TEST_F(ServiceTest, AddTagWorks)
{
  data::DataMapper mapper;
  auto t1 = tag("t1");
  mapper.addTag(t1);
  EXPECT_TRUE(mapper.hasTag("t1"));
  EXPECT_TRUE(mapper.hasTag(t1));
  EXPECT_TRUE(mapper.hasTag(t1->id()));
  EXPECT_EQ(mapper.tagFromName("t1"), t1);
}


int main(int argc, char **argv)
{
  _CONFIG_BASIC_LOGGERS;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
