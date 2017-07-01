#include "tag.h"

#include <sstream>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <core/debug/Debug.h>



////////////////////////////////////////////////////////////////////////////////
void
tag::setText(const std::string& t)
{
    // for now just assign
    m_text = t;
}


////////////////////////////////////////////////////////////////////////////////
void
tag::addElementID(core::id_t elemID)
{
    m_elementIDs.insert(elemID);
}

void
tag::removeElementID(core::id_t elemID)
{
    m_elementIDs.erase(elemID);
}

bool
tag::hasElementID(core::id_t elemID) const
{
    return m_elementIDs.find(elemID) != m_elementIDs.end();
}

rapidjson::Value
tag::toJSONValue(rapidjson::Document& d) const
{
    auto& al = d.GetAllocator();
    rapidjson::Value result(rapidjson::kObjectType);
    result.AddMember("id", m_id, al);
    result.AddMember("text", m_text, al);
    rapidjson::Value idsArray(rapidjson::kArrayType);
    for (const core::id_t id : m_elementIDs) {
        idsArray.PushBack(rapidjson::Value().SetInt(id), al);
    }
    result.AddMember("elem_ids", idsArray, al);
    return result;
}

std::string
tag::toJSON(void) const
{
    rapidjson::Document result;
    result.SetObject() = toJSONValue(result);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    result.Accept(writer);
    return buffer.GetString();
}

bool
tag::fromJSONValue(const rapidjson::Value& jo)
{
    if (!jo.IsObject() ||
        !jo.HasMember("text") ||
        !jo.HasMember("id") ||
        !jo.HasMember("elem_ids")) {
        return false;
    }
    // parse the data and set the elements
    const rapidjson::Value& txtVal = jo["text"];
    setText(txtVal.GetString());
    const rapidjson::Value& idVal = jo["id"];
    m_id = idVal.GetInt();

    const rapidjson::Value& elemIds = jo["elem_ids"];
    if (!elemIds.IsArray()) {
        return false;
    }
    m_elementIDs.clear();
    for (int i = 0; i < elemIds.Size(); ++i) {
        ASSERT(elemIds[i].IsInt());
        m_elementIDs.insert(elemIds[i].GetInt());
    }

    return true;
}

bool
tag::fromJSON(const std::string& json)
{
    //    {
    //        text: 'text',
    //        id: id,
    //        elem_ids: [i1,i2]
    //    }
    rapidjson::Document jo;
    jo.Parse(json.c_str(), json.size());
    if (!fromJSONValue(jo)) {
        debugERROR("Invalid format of the json %s", json.c_str());
        return false;
    }
    return true;
}

