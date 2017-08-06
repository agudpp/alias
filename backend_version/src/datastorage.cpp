/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2017 by Happen
 *
 *   File  created by agustin on 1.7.2017
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include "datastorage.h"

#include <fstream>
#include <vector>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <core/debug/Debug.h>
#include <elements/element.h>
#include <elements/elementmanager.h>
#include <tags/tag.h>
#include <tags/tagmanager.h>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DataStorage::DataStorage() :
    m_elementMngr(0)
,   m_tagMngr(0)
{

}

////////////////////////////////////////////////////////////////////////////////
DataStorage::~DataStorage()
{

}

////////////////////////////////////////////////////////////////////////////////
void
DataStorage::init(const MainData& md)
{
    ASSERT_PTR(md.elemMngr);
    ASSERT_PTR(md.tagMngr);
    m_elementMngr = md.elemMngr;
    m_tagMngr = md.tagMngr;
}

////////////////////////////////////////////////////////////////////////////////
bool
DataStorage::saveToFile(const std::string& fname)
{
    ASSERT_PTR(m_elementMngr);
    ASSERT_PTR(m_tagMngr);

    rapidjson::Document d;
    d.SetObject();
    auto& al = d.GetAllocator();

    // get the elements
    rapidjson::Value elemArray(rapidjson::kArrayType);
    std::vector<const element*> elems;
    m_elementMngr->getAllElements(elems);
    for (const element* elem : elems) {
        elemArray.PushBack(elem->toJSONValue(d), al);
    }
    d.AddMember("elements", elemArray, al);

    // get the tags
    rapidjson::Value tagsArray(rapidjson::kArrayType);
    std::vector<const tag*> tags;
    m_tagMngr->getAllTags(tags);
    for (const tag* tag : tags) {
        tagsArray.PushBack(tag->toJSONValue(d), al);
    }
   d.AddMember("tags", tagsArray, al);

   // now we write it into the file
   std::ofstream ofs(fname.c_str());
   rapidjson::OStreamWrapper osw(ofs);
   rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

   return d.Accept(writer);
}

////////////////////////////////////////////////////////////////////////////////
bool
DataStorage::loadFromFile(const std::string& fname)
{
    ASSERT_PTR(m_elementMngr);
    ASSERT_PTR(m_tagMngr);

    std::ifstream ifs(fname.c_str());
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    d.ParseStream(isw);

    if (!d.IsObject() ||
        !d.HasMember("elements") ||
        !d.HasMember("tags")) {
        debugERROR("Invalid format of the json %s", fname.c_str());
        return false;
    }

    // we need to read the tags and the elements
    const rapidjson::Value& elemArray = d["elements"];
    if (!elemArray.IsArray()) {
        debugERROR("invalid format, elements is not an array? %s", fname.c_str());
        return false;
    }
    for (int i = 0; i < elemArray.Size(); ++i) {
        ASSERT(elemArray[i].IsObject());
        element e(-1);
        if (!e.fromJSONValue(elemArray[i])) {
            debugERROR("Error parsing an element");
            continue;
        }
        m_elementMngr->addElement(e);
    }

    // now the tags
    const rapidjson::Value& tagsArray = d["tags"];
    if (!tagsArray.IsArray()) {
        debugERROR("invalid format, tags is not an array? %s", fname.c_str());
        return false;
    }
    for (int i = 0; i < tagsArray.Size(); ++i) {
        ASSERT(tagsArray[i].IsObject());
        tag t(-1);
        if (!t.fromJSONValue(tagsArray[i])) {
            debugERROR("Error parsing an tag");
            continue;
        }
        m_tagMngr->addTag(t);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
DataStorage::elementDirty(const element* e)
{
    ASSERT_PTR(e);
    debugTODO("IMPLEMENT THIS");
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
DataStorage::tagDirty(const tag* t)
{
    ASSERT_PTR(t);
    debugTODO("IMPLEMENT THIS");
    return true;
}

