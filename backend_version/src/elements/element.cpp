#include "element.h"

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>


#include <core/debug/Debug.h>


////////////////////////////////////////////////////////////////////////////////
void
element::setText(const std::string& t)
{
    // for now just assign
    m_text = t;
}


////////////////////////////////////////////////////////////////////////////////
void
element::addTagID(core::id_t id)
{
    m_tagIDs.insert(id);
}

void
element::removeTagID(core::id_t id)
{
    m_tagIDs.erase(id);
}

bool
element::hasTagID(core::id_t id) const
{
    return m_tagIDs.find(id) != m_tagIDs.end();
}

rapidjson::Value
element::toJSONValue(rapidjson::Document& d) const
{
    rapidjson::Value result(rapidjson::kObjectType);
    auto& al = d.GetAllocator();
    result.AddMember("id", m_id, al);
    result.AddMember("text", m_text, al);
    rapidjson::Value idsArray(rapidjson::kArrayType);
    for (const core::id_t id : m_tagIDs) {
        idsArray.PushBack(rapidjson::Value().SetInt(id), al);
    }
    result.AddMember("tag_ids", idsArray, al);
    return result;
}

std::string
element::toJSON(void) const
{
    rapidjson::Document result;
    result.SetObject() = toJSONValue(result);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    result.Accept(writer);
    return buffer.GetString();
}

bool
element::fromJSONValue(const rapidjson::Value& jo)
{
    if (!jo.IsObject() ||
        !jo.HasMember("text") ||
        !jo.HasMember("id") ||
        !jo.HasMember("tag_ids")) {
        return false;
    }
    // parse the data and set the elements
    const rapidjson::Value& txtVal = jo["text"];
    setText(txtVal.GetString());
    const rapidjson::Value& idVal = jo["id"];
    m_id = idVal.GetInt();

    const rapidjson::Value& tagIds = jo["tag_ids"];
    if (!tagIds.IsArray()) {
        return false;
    }
    m_tagIDs.clear();
    for (int i = 0; i < tagIds.Size(); ++i) {
        ASSERT(tagIds[i].IsInt());
        m_tagIDs.insert(tagIds[i].GetInt());
    }

    return true;
}

bool
element::fromJSON(const std::string& json)
{
    //    {
    //        text: 'text',
    //        id: id,
    //        tag_ids: [i1,i2]
    //    }
    rapidjson::Document jo;
    jo.Parse(json.c_str(), json.size());
    if (!fromJSONValue(jo)) {
        debugERROR("Invalid format of the json %s", json.c_str());
        return false;
    }
    return true;
}


