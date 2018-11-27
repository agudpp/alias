#include <gtest/gtest.h>

#include <ostream>
#include <istream>
#include <sstream>
#include <vector>

#include <elements/element.h>
#include <elements/simple_text_element.h>

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}


TEST(simple_text_element, can_add_tags)
{
  const std::vector<core::UID> tag_ids {
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom()
  };
  const core::UID id = core::UID::generateRandom();
  SimpleTextElement e1(id);
  EXPECT_FALSE(e1.hasTagID(tag_ids[0]));
  e1.addTagID(tag_ids[0]);
  EXPECT_TRUE(e1.hasTagID(tag_ids[0]));

  EXPECT_FALSE(e1.hasTagID(tag_ids[1]));
  e1.addTagID(tag_ids[1]);
  EXPECT_TRUE(e1.hasTagID(tag_ids[1]));

  EXPECT_FALSE(e1.hasTagID(tag_ids[2]));
  e1.addTagID(tag_ids[2]);
  EXPECT_TRUE(e1.hasTagID(tag_ids[2]));

  e1.removeTagID(tag_ids[1]);
  EXPECT_FALSE(e1.hasTagID(tag_ids[1]));
  EXPECT_TRUE(e1.hasTagID(tag_ids[0]));
  EXPECT_TRUE(e1.hasTagID(tag_ids[2]));
}


TEST(simple_text_element, can_serialize_deserialize)
{
  const core::UID id = core::UID::generateRandom();
  const std::string test_text = "hola!";
  const std::vector<core::UID> tag_ids {
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom()
  };

  SimpleTextElement e1(id);
  e1.setText(test_text);
  for (auto& uid : tag_ids) {
    e1.addTagID(uid);
  }

  std::stringstream ss;
  EXPECT_TRUE(e1.serialize(ss));

  SimpleTextElement e2, e3;
  EXPECT_TRUE(e2.deserialize(ss));

  EXPECT_EQ(e1, e2);
  EXPECT_EQ(test_text, e2.text());
  EXPECT_EQ(id, e2.id());
  for (auto& uid : tag_ids) {
    EXPECT_TRUE(e2.hasTagID(uid));
  }
}




int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
