#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <memory>
#include <fstream>

#include <core/types/trie.h>
#include <core/logger/LoggerManager.h>
#include <core/logger/Loggers.h>
#include <core/utils/json_utils.h>
#include <core/debug/Debug.h>

#include <tags/tagmanager.h>
#include <tags/tag.h>
#include <elements/element.h>
#include <elements/elementmanager.h>
#include <elements/simple_text_element.h>

#include <storage/file_storage.h>

#include <service_api/serviceapi.h>

#include <ui_client/qt_client.h>

#include <backend_config.h>
#include <consts.h>


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
    LOG_ERROR("Problem loading all the elements");
    return false;
  }

  std::vector<Tag::Ptr> tags;
  if (!storage->loadAllTags(tags)) {
    LOG_ERROR("Problem loading the tags");
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

/**
 * @brief Loads a backend configuration file
 * @param config_file the file path
 * @param config config to be filled on success
 * @return true on success, false otherwise
 */
static bool
loadBackendConfig(const std::string& config_file, BackendConfig& config)
{
  std::ifstream json_config(config_file.c_str(), std::fstream::in);
  if (!json_config.is_open()) {
    LOG_ERROR("Couldnt load the config file %s", config_file.c_str());
    return false;
  }

  rapidjson::Document doc = core::JsonUtils::fromStream(json_config);
  if (!doc.IsObject() || !doc.HasMember("backend") || !doc["backend"].IsObject()) {
    LOG_ERROR("Invalid config file %s, is not an object or backend field is missing or invalid",
              config_file.c_str());
    return false;
  }

  const rapidjson::Value& backend = doc["backend"];
  if (!backend.HasMember("db_path")) {
    LOG_ERROR("db_path member not found on config file %s", config_file.c_str());
    return false;
  }

  config.db_path = backend["db_path"].GetString();

  return true;
}


int
main(int argc, char *argv[])
{
  _CONFIG_BASIC_LOGGERS;

  if (argc < 2) {
    LOG_ERROR("Missing config file path as argument");
    return -1;
  }

  BackendConfig be_config;
  if (!loadBackendConfig(argv[1], be_config)) {
    return -2;
  }

  FileStoragePtr file_storage = loadFileStorage(be_config.db_path);
  ElementManager element_manager;
  TagManager tag_manager;


  if (!loadElements(file_storage.get(), &element_manager, &tag_manager)) {
    return -1;
  }

  ServiceAPI service_api(&element_manager, &tag_manager, file_storage.get());


  return QTClient::execute(argc, argv, &service_api);
}
