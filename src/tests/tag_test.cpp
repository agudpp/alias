#include <gtest/gtest.h>

#include <ostream>
#include <istream>
#include <sstream>
#include <vector>

#include <tags/tagmanager.h>
#include <tags/tag.h>



TEST(tag_test, can_add_elements)
{
  const std::vector<core::UID> element_ids {
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom()
  };
  const core::UID id = core::UID::generateRandom();
  Tag t1(id);
  for (std::size_t i = 0; i < element_ids.size(); ++i) {
    EXPECT_FALSE(t1.hasElementID(element_ids[i])) << "for element " << i;
    t1.addElementID(element_ids[i]);
    EXPECT_TRUE(t1.hasElementID(element_ids[i])) << "for element " << i;
  }
}

TEST(tag_test, can_remove_elements)
{
  const std::vector<core::UID> element_ids {
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom(),
    core::UID::generateRandom()
  };
  const core::UID id = core::UID::generateRandom();
  Tag t1(id);
  for (std::size_t i = 0; i < element_ids.size(); ++i) {
    t1.addElementID(element_ids[i]);
  }
  for (std::size_t i = 0; i < element_ids.size(); ++i) {
    EXPECT_TRUE(t1.hasElementID(element_ids[i])) << "for element " << i;
    t1.removeElementID(element_ids[i]);
    EXPECT_FALSE(t1.hasElementID(element_ids[i])) << "for element " << i;
  }
}



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
