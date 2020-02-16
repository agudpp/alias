#include <toolbox/config/config.h>

#include <fstream>

#include <toolbox/rapidjson/istreamwrapper.h>
#include <toolbox/rapidjson/ostreamwrapper.h>
#include <toolbox/rapidjson/stringbuffer.h>
#include <toolbox/rapidjson/writer.h>

#include <toolbox/debug/debug.h>
#include <toolbox/utils/json_utils.h>

namespace toolbox {

//LOG_ERROR("JSON: " << JsonUtils::toString(json_doc_))
#define PRECONDITION \
  ASSERT_CMD(json_doc_.IsObject(), dumpJson());\
  if (!json_doc_.HasMember(key.c_str())) {\
    LOG_ERROR("The json has no field " << key << " in it");\
    return false;\
  }

#define GET_VALUE(isType, getVal) \
  const rapidjson::Value& jval = json_doc_[key.c_str()];\
  if (!jval.isType()) {\
    LOG_ERROR("The value we are trying to get " #isType " has a different type for key " << key);\
    return false;\
  }\
  value = jval.getVal();\
  return true;



Config::Config(const rapidjson::Document& json_doc)
{
  json_doc_.CopyFrom(json_doc, json_doc_.GetAllocator());
}

Config::Config()
{
}

Config::Config(const Config& other)
{
  json_doc_.CopyFrom(other.json_doc_, json_doc_.GetAllocator());
}

Config& Config::operator=(const Config& other)
{
  json_doc_.CopyFrom(other.json_doc_, json_doc_.GetAllocator());
  return *this;
}

bool
Config::parse(std::istream& stream)
{
  rapidjson::IStreamWrapper isw(stream);
  json_doc_.ParseStream(isw);

  if (json_doc_.HasParseError()) {
    LOG_ERROR("Error parsing the json with error: " << json_doc_.GetParseError());
  }

  return !json_doc_.HasParseError();
}

bool
Config::parseFromFilePath(const std::string& file_path)
{
  std::ifstream config_file(file_path);
  if (!config_file.good()) {
    LOG_ERROR("cannot read config file " << file_path);
    return false;
  }
  if (!parse(config_file)) {
    LOG_ERROR("Cannot parse file " << file_path);
    config_file.close();
    return false;
  }
  return true;
}

bool
Config::getValue(const std::string& key, int& value) const
{
  PRECONDITION;
  GET_VALUE(IsInt, GetInt);
}

bool
Config::getValue(const std::string& key, float& value) const
{
  PRECONDITION;
  GET_VALUE(IsFloat, GetFloat);
}

bool
Config::getValue(const std::string& key, std::string& value) const
{
  PRECONDITION;
  GET_VALUE(IsString, GetString);
}

bool
Config::hasKey(const std::string& key) const
{
  return json_doc_.HasMember(key);
}

bool
Config::getConfig(const std::string& key, Config& config) const
{
  PRECONDITION;
  const rapidjson::Value& jval = json_doc_[key.c_str()];
  if (!jval.IsObject()) {
    LOG_ERROR("We cannot construct a config from a key (" << key << ") that is not an object");
    return false;
  }
  rapidjson::Document new_doc;
  rapidjson::Value& copy = new_doc.SetObject();
  copy.CopyFrom(jval, new_doc.GetAllocator());
  config = Config(new_doc);
  return true;
}

void
Config::dumpJson(void) const
{
  LOG_INFO("JSON: " << JsonUtils::toString(json_doc_));
}

}



