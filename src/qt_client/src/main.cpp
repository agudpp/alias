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
};




//// TODO: remove this test

//static data::Content::Ptr
//createTextContent(const std::string& content, const std::set<data::Tag::ConstPtr>& tags)
//{
//  data::Content::Ptr result(new data::Content);
//  data::Metadata metadata;
//  metadata.setType(data::ContentType::TEXT);
//  result->setMetadata(metadata);
//  result->setData(content);

//  for (auto& tag : tags) {
//    result->addTag(tag->id());
//  }

//  return result;
//}

//static service::ServiceAPI::Ptr
//buildTestService(std::vector<data::Content::Ptr>& contents)
//{
//  std::vector<std::string> tag_names = {
//    "tag1",
//    "tag11",
//    "some-other-tag",
//    "reliquid"
//  };
//  data::DataMapper::Ptr data_mapper(new data::DataMapper);
//  for (auto& tag_name : tag_names) {
//    data::Tag::Ptr tag_ptr(new data::Tag(tag_name));
//    data_mapper->addTag(tag_ptr);
//  }

//  data::Content::Ptr c1 = createTextContent("content number 1", {data_mapper->tagFromName("tag1")});
//  data::Content::Ptr c2 = createTextContent("content number 2", {
//                                              data_mapper->tagFromName("tag1"),
//                                              data_mapper->tagFromName("reliquid"),
//                                            });
//  data_mapper->addContent(c1);
//  data_mapper->addContent(c2);

//  contents.push_back(c1);
//  contents.push_back(c2);

//  storage::DataStorage::Ptr data_storage(new storage::DummyStorage());
//  return service::ServiceAPI::Ptr(new service::ServiceAPI(data_mapper, data_storage));
//}



/**
 * @brief Returns the default config file path if none is set
 * @return the default config file path if none is set
 */
static std::string
getDefaultConfigFilePath()
{
  return toolbox::OSHelper::getHomeDir() + "alias/init.json";
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
    data_storage = buildFileStorage(toolbox::OSHelper::expandFilePath(storage_location), data_mapper);
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
            << "\t\t" << "--hidden" << "\t\t flag indicating if we want to execute it in a hidden mode." << std::endl;
}

static bool
parseParams(int argc, char *argv[], Params& result)
{
  // set default values
  result.config_file = getDefaultConfigFilePath();
  result.hidden = false;

  if (argc <= 1) {
    return true;
  }

  int idx = 1;
  while (idx < argc) {
    const std::string option = argv[idx];
    if (option == "--hidden") {
      result.hidden = true;
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

  return qt_client::QTClient::execute(argc, argv, service_api, config, params.hidden);
}
