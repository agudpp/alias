#include <ostream>
#include <istream>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
#include <map>

#include <gtest/gtest.h>

#include <toolbox/os/os_helper.h>
#include <toolbox/debug/debug.h>

#include <data/data_mapper.h>
#include <storage/file_storage.h>


constexpr const char* FOLDER_DB_PATH = "./db/";


// The fixture for testing class Foo.
class StorageTest : public ::testing::Test {
 protected:
  inline StorageTest() {}
  inline ~StorageTest() override {}

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

static std::map<std::string, data::Tag::Ptr>
toMap(const std::vector<data::Tag::Ptr>& tags)
{
  std::map<std::string, data::Tag::Ptr> result;
  for (auto& t : tags) {
    result[t->name()] = t;
  }
  return result;
}



TEST_F(StorageTest, PersistenceTagWorks)
{
  std::vector<data::Tag::Ptr> tags = {tag("t1"), tag("t2"), tag("t3")};
  {
    storage::FileStorage storage(FOLDER_DB_PATH);
    for (auto& tag : tags) {
      EXPECT_TRUE(storage.saveTag(tag));
    }
  }
  // check was correctly stored all
  {
    storage::FileStorage storage(FOLDER_DB_PATH);
    std::vector<data::Tag::Ptr> loaded_tags;
    EXPECT_TRUE(storage.loadAllTags(loaded_tags));
    auto tags_map = toMap(loaded_tags);
    auto t1 = tags_map.find("t1");
    EXPECT_TRUE(t1 != tags_map.end());
    EXPECT_EQ(*(t1->second), *(tags[0]));
    auto t2 = tags_map.find("t2");
    EXPECT_TRUE(t2 != tags_map.end());
    EXPECT_EQ(*(t2->second), *(tags[1]));
    auto t3 = tags_map.find("t3");
    EXPECT_TRUE(t3 != tags_map.end());
    EXPECT_EQ(*(t3->second), *(tags[2]));
  }
}

TEST_F(StorageTest, PersistenceContentWorks)
{
  std::vector<data::Tag::Ptr> tags = {tag("t1"), tag("t2")};
  auto c1 = content("c1", ids({tags[0], tags[1]}));
  {
    storage::FileStorage storage(FOLDER_DB_PATH);
    for (auto& tag : tags) {
      EXPECT_TRUE(storage.saveTag(tag));
    }
    EXPECT_TRUE(storage.saveContent(c1));
  }
  // check was correctly stored all
  {
    storage::FileStorage storage(FOLDER_DB_PATH);
    std::vector<data::Content::Ptr> loaded_content;
    EXPECT_TRUE(storage.loadAllContent(loaded_content));
    EXPECT_EQ(loaded_content.size(), 1);
    EXPECT_EQ(*c1, *(loaded_content.front()));
  }
}



int main(int argc, char **argv)
{
  _CONFIG_BASIC_LOGGERS;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
