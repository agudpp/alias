#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <memory>
#include <fstream>

#include <toolbox/debug/debug.h>
#include <toolbox/config/config.h>
#include <toolbox/os/os_helper.h>

#include <service/service_api.h>
#include <storage/dummy_storage.h>
#include <storage/file_storage.h>
#include <data/data_mapper.h>

#include <qt_client/qt_client.h>


/**
 * @brief The Params struct
 */
struct Params {
  std::string config_file;
  bool hidden = false;
  bool remove_unused_tags = true;
};



/**
 * @brief Returns the default config file path if none is set
 * @return the default config file path if none is set
 */
static std::string
getDefaultConfigFilePath()
{  
  return toolbox::OSHelper::normalizeFilePath(toolbox::OSHelper::currentWorkingDir(), "init.json");
}

/**
 * @brief buildFileStorage
 * @param storage_location
 * @param data_mapper
 * @return
 */
static storage::DataStorage::Ptr
buildFileStorage(const std::string& storage_location, data::DataMapper::Ptr& data_mapper)
{
  storage::FileStorage::Ptr result(new storage::FileStorage(storage_location));
  std::vector<data::Tag::Ptr> tags;
  std::vector<data::Content::Ptr> contents;

  if (!result->loadAllTags(tags)) {
    LOG_ERROR("Error loading the tags from the folder " << storage_location);
    return nullptr;
  }
  if (!result->loadAllContent(contents)) {
    LOG_ERROR("Error loading the contents from the folder " << storage_location);
    return nullptr;
  }

  LOG_INFO("Loaded " << tags.size() << " tags and " << contents.size() << " content objects");
  for (auto& tag : tags) {
    data_mapper->addTag(tag);
  }
  for (auto& content : contents) {
    data_mapper->addContent(content);
  }

  return result;
}

/**
 * @brief createFolderIfNotExists
 * @param path the path to create
 * @return true on success | false otherwise
 */
static bool
createFolderIfNotExists(const std::string& path)
{
  if (toolbox::OSHelper::checkFolderExists(path)) {
    return true;
  }
  return toolbox::OSHelper::createFolder(path, true);
}

/**
 * @brief buildServiceAPI
 * @param config
 * @return
 */
static service::ServiceAPI::Ptr
buildServiceAPI(const toolbox::Config& config)
{
  service::ServiceAPI::Ptr result;
  toolbox::Config storage_config;
  std::string storage_type;

  if (!config.getConfig("storage", storage_config) ||
      !storage_config.getValue("storageType", storage_type)) {
    LOG_ERROR("No storage section or storageType value found on the config");
    return result;
  }

  data::DataMapper::Ptr data_mapper(new data::DataMapper);
  storage::DataStorage::Ptr data_storage;

  if (storage_type == "MEMORY") {
    data_storage.reset(new storage::DummyStorage);
  } else if (storage_type == "PERSISTENT") {
    std::string storage_location;
    if (!storage_config.getValue("folder", storage_location)) {
      LOG_ERROR("Missing folder value on the config");
      return result;
    }
    // check if the folder exists otherwise we create it
    storage_location = toolbox::OSHelper::expandFilePath(storage_location);
    createFolderIfNotExists(storage_location);
    data_storage = buildFileStorage(storage_location, data_mapper);
  }


  result.reset(new service::ServiceAPI(data_mapper, data_storage));

  return result;
}

/**
 * @brief help
 */
static void
help()
{
  std::cout << "To execute the program: " << std::endl
            << "\t" << "./alias [options]" << std::endl
            << "\t" << "where options: " << std::endl
            << "\t\t" << "--config-file <config_file_path>" << "\t\t the location to the config file." << std::endl
            << "\t\t" << "--keep-unused-tags" << "\t\t Keep tags that dont have a content associated to." << std::endl
            << "\t\t" << "--hidden" << "\t\t flag indicating if we want to execute it in a hidden mode." << std::endl;
}

static bool
parseParams(int argc, char *argv[], Params& result)
{
  // set default values
  result.config_file = getDefaultConfigFilePath();  

  if (argc <= 1) {
    return true;
  }

  int idx = 1;
  while (idx < argc) {
    const std::string option = argv[idx];
    if (option == "--hidden") {
      result.hidden = true;
    } else if (option == "--keep-unused-tags") {
      result.remove_unused_tags = false;
    } else if (option == "--config-file") {
      if ((idx + 1) >= argc) {
        return false;
      }
      result.config_file = argv[idx+1];
      idx++;
    }
    idx++;
  }

  return true;
}

int
main(int argc, char *argv[])
{
  _CONFIG_BASIC_LOGGERS;
  Params params;
  if (!parseParams(argc, argv, params)) {
    help();
    return 1;
  }
  const std::string& config_file_path = params.config_file;
  toolbox::Config config;
  LOG_INFO("Using config file " << config_file_path);
  if (!config.parseFromFilePath(config_file_path)) {
    LOG_ERROR("Error parsing the config file " << config_file_path);
    return 1;
  }

  service::ServiceAPI::Ptr service_api = buildServiceAPI(config);

  if (params.remove_unused_tags) {
    service_api->performTagCleanup(service::TagCleanup());
  }

  return qt_client::QTClient::execute(argc, argv, service_api, config, params.hidden);
}
