#include "tag.h"

#include <sstream>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <core/debug/Debug.h>
#include <core/utils/json_utils.h>



////////////////////////////////////////////////////////////////////////////////
void
Tag::setText(const std::string& t)
{
  // for now just assign
  text_ = t;
}


////////////////////////////////////////////////////////////////////////////////

rapidjson::Value
Tag::toJSONValue(rapidjson::Document& d) const
{
  auto& al = d.GetAllocator();
  rapidjson::Value result(rapidjson::kObjectType);
  result.AddMember("id", uid_.toStr(), al);
  result.AddMember("text", text_, al);
  rapidjson::Value idsArray(rapidjson::kArrayType);
  for (const core::UID& id : element_ids_) {
    idsArray.PushBack(rapidjson::Value().SetString(id.toStr(), al), al);
  }
  result.AddMember("eleid_s", idsArray, al);
  return result;
}

std::string
Tag::toJSON(void) const
{
  rapidjson::Document result;
  result.SetObject() = toJSONValue(result);
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  result.Accept(writer);
  return buffer.GetString();
}

bool
Tag::fromJSONValue(const rapidjson::Value& jo)
{
  if (!jo.IsObject() ||
    !jo.HasMember("text") ||
    !jo.HasMember("id") ||
    !jo.HasMember("eleid_s")) {
    return false;
  }
  // parse the data and set the elements
  const rapidjson::Value& txtVal = jo["text"];
  setText(txtVal.GetString());
  const rapidjson::Value& idVal = jo["id"];
  uid_.fromStr(idVal.GetString());

  const rapidjson::Value& elemIds = jo["eleid_s"];
  if (!elemIds.IsArray()) {
    return false;
  }
  element_ids_.clear();
  for (unsigned int i = 0; i < elemIds.Size(); ++i) {
    ASSERT(elemIds[i].IsString());
    element_ids_.insert(core::UID(elemIds[i].GetString()));
  }

  return true;
}

bool
Tag::fromJSON(const std::string& json)
{
  //  {
  //    text: 'text',
  //    id: id,
  //    eleid_s: [i1,i2]
  //  }
  rapidjson::Document jo;
  jo.Parse(json.c_str(), json.size());
  if (!fromJSONValue(jo)) {
    LOG_ERROR("Invalid format of the json %s", json.c_str());
    return false;
  }
  return true;
}

bool
Tag::serialize(std::ostream& stream) const
{
  rapidjson::Document d;
  d.SetObject() = toJSONValue(d);
  core::JsonUtils::toStream(d, stream);
  return true;
}

bool
Tag::deserialize(std::istream& stream)
{
  rapidjson::Document doc = core::JsonUtils::fromStream(stream);
  return fromJSONValue(doc);
}


