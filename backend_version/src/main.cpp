#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <memory>

#include <core/types/trie.h>
#include <core/logger/LoggerManager.h>
#include <core/logger/Loggers.h>
#include <core/debug/Debug.h>

#include <tags/tagmanager.h>
#include <tags/tag.h>
#include <elements/element.h>
#include <elements/elementmanager.h>
#include <elements/simple_text_element.h>

#include <storage/file_storage.h>

#include <service_api/serviceapi.h>

#include <ui_client/qt_client.h>

#include "consts.h"

// TODO
#define FOLDER_DB_TAG_LINKER "/home/agustin/tag_linker_db/"


typedef std::shared_ptr<core::Logger> LoggerPtr;
typedef std::shared_ptr<FileStorage> FileStoragePtr;


static FileStoragePtr
loadFileStorage(const std::string& folder)
{
  FileStoragePtr file_storage = std::make_shared<FileStorage>(folder);
  return file_storage;
}

static bool
loadElements(DataStorage* storage, ElementManager* elem_mngr, TagManager* tag_mngr)
{
  ASSERT_PTR(storage);
  ASSERT_PTR(elem_mngr);
  ASSERT_PTR(tag_mngr);

  std::vector<Element::Ptr> elements;
  if (!storage->loadAllElements(elements)) {
    debugERROR("Problem loading all the elements");
    return false;
  }

  std::vector<Tag::Ptr> tags;
  if (!storage->loadAllTags(tags)) {
    debugERROR("Problem loading the tags");
    return false;
  }

  for (Element::Ptr& e : elements) {
    elem_mngr->addElement(e);
  }

  for (Tag::Ptr& t : tags) {
    tag_mngr->addTag(t);
  }

  return true;
}

void
to_remove(FileStorage* fs)
{
//  std::vector<std::string> tag_txt {
//    "agustin", "agu", "aguperez", "aguperezpala"
//  };
  std::vector<Tag::Ptr> tags;
//  for (const std::string& s : tag_txt) {
//    tags.push_back(std::make_shared<Tag>(core::UID::generateRandom(), s));
//  }
  fs->loadAllTags(tags);

  {
    auto elem = std::make_shared<SimpleTextElement>(core::UID::generateRandom(), "example number 2");
    for (auto t : tags) {
      elem->addTagID(t->id());
      t->addElementID(elem->id());
      fs->saveTag(t);
    }
      fs->saveElement(elem);
  }

  {
    auto elem = std::make_shared<SimpleTextElement>(core::UID::generateRandom(), "yet another example");
    for (auto t : tags) {
      elem->addTagID(t->id());
      t->addElementID(elem->id());
      fs->saveTag(t);
    }
      fs->saveElement(elem);
  }


}


int
main(int argc, char *argv[])
{
  _CONFIG_BASIC_LOGGERS;
  FileStoragePtr file_storage = loadFileStorage(FOLDER_DB_TAG_LINKER);
  ElementManager element_manager;
  TagManager tag_manager;

  // TODO : REMOVE THIS
//  to_remove(file_storage.get());

  if (!loadElements(file_storage.get(), &element_manager, &tag_manager)) {
    return -1;
  }

  ServiceAPI service_api(&element_manager, &tag_manager, file_storage.get());


  return QTClient::execute(argc, argv, &service_api);
}
