#include <elements/element.h>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>


#include <core/debug/Debug.h>



bool
Element::loadFromJsonValue(rapidjson::Value& json_value)
{
  if (!json_value.IsObject()) {
    return false;
  }
  // parse the data and set the Elements
  if (json_value.HasMember("id")) {
    const rapidjson::Value& idVal = json_value["id"];
    id_.fromStr(idVal.GetString());
  }

  tag_ids_.clear();
  if (json_value.HasMember("tag_ids")) {
    const rapidjson::Value& tag_ids = json_value["tag_ids"];
    if (!tag_ids.IsArray()) {
      LOG_ERROR("tag_ids field is not an array, wrong format?");
      return false;
    }
    for (unsigned int i = 0; i < tag_ids.Size(); ++i) {
      if (!tag_ids[i].IsString()) {
        LOG_ERROR("Wrong format, expected string but it is not");
        return false;
      }
      tag_ids_.insert(core::UID(tag_ids[i].GetString()));
    }
  }

  return true;
}

rapidjson::Value
Element::toJsonValue(rapidjson::Document& d) const
{
  rapidjson::Value result(rapidjson::kObjectType);
  auto& al = d.GetAllocator();
  result.AddMember("id", id_.toStr(), al);
  rapidjson::Value idsArray(rapidjson::kArrayType);
  for (const core::UID& id : tag_ids_) {
    const std::string& str_id = id.toStr();
    rapidjson::Value s;
    s.SetString(str_id, al);
    idsArray.PushBack(s, al);
  }
  result.AddMember("tag_ids", idsArray, al);
  result.AddMember("type", elementType(), al);

  return result;
}


