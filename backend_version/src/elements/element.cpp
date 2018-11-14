#include <elements/element.h>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>


#include <core/debug/Debug.h>



bool
Element::loadFromJsonValue(rapidjson::Value& json_value)
{
  if (!json_value.IsObject() ||
      !json_value.HasMember("id") ||
      !json_value.HasMember("tag_ids")) {
    return false;
  }
  // parse the data and set the Elements
  const rapidjson::Value& idVal = json_value["id"];
  id_.fromStr(idVal.GetString());

  const rapidjson::Value& tag_ids = json_value["tag_ids"];
  if (!tag_ids.IsArray()) {
    debugERROR("tag_ids field is not an array, wrong format?");
    return false;
  }
  tag_ids_.clear();
  for (unsigned int i = 0; i < tag_ids.Size(); ++i) {
    if (!tag_ids[i].IsString()) {
      debugERROR("Wrong format, expected string but it is not");
      return false;
    }
    tag_ids_.insert(core::UID(tag_ids[i].GetString()));
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
  return result;
}


