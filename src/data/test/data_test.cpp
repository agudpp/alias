#include <ostream>
#include <istream>
#include <sstream>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include <toolbox/os/os_helper.h>
#include <toolbox/debug/debug.h>

#include <data/data_mapper.h>



// The fixture for testing class Foo.
class DataTest : public ::testing::Test {
 protected:
  inline DataTest() {}
  inline ~DataTest() override {}

  void SetUp() override
  {
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


TEST_F(DataTest, AddTagWorks)
{
  data::DataMapper mapper;
  auto t1 = tag("t1");
  mapper.addTag(t1);
  EXPECT_TRUE(mapper.hasTag("t1"));
  EXPECT_TRUE(mapper.hasTag(t1));
  EXPECT_TRUE(mapper.hasTag(t1->id()));
  EXPECT_EQ(mapper.tagFromName("t1"), t1);
}

//  - add content with tags we should be able to get that content from the each of the tags
TEST_F(DataTest, ContentWothTags)
{
  data::DataMapper mapper;
  std::vector<data::Tag::Ptr> tags = {tag("t1"), tag("t2"), tag("t3")};
  auto c1 = content("c1", ids({tags[0], tags[1]}));
  auto c2 = content("c2", ids({tags[1], tags[2]}));

  for (auto& t : tags) {
    mapper.addTag(t);
  }
  mapper.addContent(c1);
  mapper.addContent(c2);
  EXPECT_TRUE(mapper.hasContent(c1->id()));
  auto expected_c1 = mapper.contentsForTag(tags[0]->id());
  EXPECT_TRUE(expected_c1.size() == 1);
  EXPECT_EQ(*(expected_c1.begin()), c1);
  auto expected_c2 = mapper.contentsForTag(tags[2]->id());
  EXPECT_TRUE(expected_c2.size() == 1);
  EXPECT_TRUE((*expected_c2.begin()) == c2);
  auto expected_both = mapper.contentsForTag(tags[1]->id());
  EXPECT_EQ(expected_both.size(), 2);
}

//  - adding 2 content with overlapping tags, removing one content remove only tags associated
TEST_F(DataTest, RemoveTagsWork)
{
  data::DataMapper mapper;
  std::vector<data::Tag::Ptr> tags = {tag("t1"), tag("t2"), tag("t3")};
  auto c1 = content("c1", ids({tags[0], tags[1]}));
  auto c2 = content("c2", ids({tags[1], tags[2]}));

  for (auto& t : tags) {
    mapper.addTag(t);
  }
  mapper.addContent(c1);
  mapper.addContent(c2);
  EXPECT_TRUE(mapper.hasContent(c1->id()));
  mapper.removeContent(c1);
  EXPECT_FALSE(mapper.hasContent(c1));

  auto expect_none = mapper.contentsForTag(tags[0]->id());
  EXPECT_EQ(expect_none.size(), 0);

  auto expect_c2 = mapper.contentsForTag(tags[1]->id());
  EXPECT_EQ(expect_c2.size(), 1);
  EXPECT_TRUE((*expect_c2.begin()) == c2);
}


int main(int argc, char **argv)
{
  _CONFIG_BASIC_LOGGERS;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
