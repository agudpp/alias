#include <gtest/gtest.h>

#include <ostream>
#include <istream>
#include <sstream>
#include <vector>

#include <core/debug/Debug.h>
#include <elements/simple_text_element.h>
#include <storage/file_storage.h>
#include <tags/tagmanager.h>
#include <tags/tag.h>

#define FS_FOLDER "test_file_storage_db/"

class FileStorageTest : public ::testing::Test {
 protected:
  void SetUp() override
  {
    std::cout << "running command: " << "rm -rf " FS_FOLDER "elements" << "\n";
    std::system("rm -rf " FS_FOLDER "elements");
    std::cout << "running command: " << "rm -rf " FS_FOLDER "tags" << "\n";
    std::system("rm -rf " FS_FOLDER "tags");
    std::cout << "running command: " << "mkdir -p " FS_FOLDER << "\n";
    std::system("mkdir -p " FS_FOLDER);
  }

  // void TearDown() override {}

};



TEST_F(FileStorageTest, simple_save_load_tags)
{
  const core::UID t1_id = core::UID::generateRandom();
  const core::UID t2_id = core::UID::generateRandom();

  FileStorage fs;
  fs.setFolderPath(FS_FOLDER);
  Tag::Ptr t1(new Tag(t1_id));
  t1->setText("test_tag_text");
  t1->addElementID(core::UID::generateRandom());
  t1->addElementID(core::UID::generateRandom());
  t1->addElementID(core::UID::generateRandom());
//  Tag::Ptr t2(new Tag(t2_id));

  EXPECT_TRUE(fs.saveTag(t1));
//  EXPECT_TRUE(fs.saveTag(t2));


  FileStorage fs2;
  fs2.setFolderPath(FS_FOLDER);

  std::vector<Tag::Ptr> tags;
  EXPECT_TRUE(fs2.loadAllTags(tags));
  EXPECT_EQ(1, tags.size());
  EXPECT_EQ(*t1, *tags[0]);

}


TEST_F(FileStorageTest, simple_remove_tag_works)
{
  const core::UID t1_id = core::UID::generateRandom();

  FileStorage fs;
  fs.setFolderPath(FS_FOLDER);
  Tag::Ptr t1(new Tag(t1_id));
  t1->setText("test_tag_text");
  t1->addElementID(core::UID::generateRandom());

  EXPECT_TRUE(fs.saveTag(t1));

  {
    FileStorage fs2;
    fs2.setFolderPath(FS_FOLDER);

    std::vector<Tag::Ptr> tags;
    EXPECT_TRUE(fs2.loadAllTags(tags));
    EXPECT_EQ(1, tags.size());
    EXPECT_EQ(*t1, *tags[0]);
  }

  EXPECT_TRUE(fs.removeTag(t1));
  {
    FileStorage fs2;
    fs2.setFolderPath(FS_FOLDER);

    std::vector<Tag::Ptr> tags;
    EXPECT_TRUE(fs2.loadAllTags(tags));
    EXPECT_EQ(0, tags.size());
  }
}


TEST_F(FileStorageTest, simple_save_load_elements)
{
  const core::UID id1 = core::UID::generateRandom();
  const core::UID id2 = core::UID::generateRandom();

  FileStorage fs;
  fs.setFolderPath(FS_FOLDER);
  SimpleTextElement* s1 = new SimpleTextElement(id1, "text_tst_1");
//  SimpleTextElement* s2 = new SimpleTextElement(id2, "text_tst_2");
  Element::Ptr e1(s1);
  e1->addTagID(id1);
  e1->addTagID(id2);
//  Tag::Ptr t2(new Tag(t2_id));

  EXPECT_TRUE(fs.saveElement(e1));
//  EXPECT_TRUE(fs.saveTag(t2));


  FileStorage fs2;
  fs2.setFolderPath(FS_FOLDER);

  std::vector<Element::Ptr> elements;
  EXPECT_TRUE(fs2.loadAllElements(elements));
  EXPECT_EQ(1, elements.size());
  EXPECT_EQ(SimpleTextElement::NAME, elements[0]->elementType());
  EXPECT_EQ(*s1, *static_cast<SimpleTextElement*>(elements[0].get()));

}


TEST_F(FileStorageTest, simple_remove_element_works)
{
  const core::UID id1 = core::UID::generateRandom();

  FileStorage fs;
  fs.setFolderPath(FS_FOLDER);
  SimpleTextElement* s1 = new SimpleTextElement(id1, "text_tst_1");
  Element::Ptr e1(s1);
  e1->addTagID(id1);

  EXPECT_TRUE(fs.saveElement(e1));

  {
    FileStorage fs2;
    fs2.setFolderPath(FS_FOLDER);

    std::vector<Element::Ptr> elements;
    EXPECT_TRUE(fs2.loadAllElements(elements));
    EXPECT_EQ(1, elements.size());
    EXPECT_EQ(SimpleTextElement::NAME, elements[0]->elementType());
    EXPECT_EQ(*s1, *static_cast<SimpleTextElement*>(elements[0].get()));
  }

  fs.removeElement(e1);
  {
    FileStorage fs2;
    fs2.setFolderPath(FS_FOLDER);

    std::vector<Element::Ptr> elements;
    EXPECT_TRUE(fs2.loadAllElements(elements));
    EXPECT_EQ(0, elements.size());
  }
}




int main(int argc, char **argv)
{
    _CONFIG_BASIC_LOGGERS;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
