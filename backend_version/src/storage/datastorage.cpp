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
,   m_TagMngr(0)
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
    ASSERT_PTR(md.TagMngr);
    m_elementMngr = md.elemMngr;
    m_TagMngr = md.TagMngr;
}

////////////////////////////////////////////////////////////////////////////////
bool
DataStorage::saveToFile(const std::string& fname)
{
    ASSERT_PTR(m_elementMngr);
    ASSERT_PTR(m_TagMngr);

    rapidjson::Document d;
    d.SetObject();
    auto& al = d.GetAllocator();

    // get the elements
    rapidjson::Value elemArray(rapidjson::kArrayType);
    std::vector<const element*> elems;
    // TODO
//    m_elementMngr->getAllElements(elems);
    for (const element* elem : elems) {
//        elemArray.PushBack(elem->toJSONValue(d), al);
    }
    d.AddMember("elements", elemArray, al);

    // get the Tags
    rapidjson::Value TagsArray(rapidjson::kArrayType);
    std::vector<const Tag*> Tags;
    m_TagMngr->getAllTags(Tags);
    for (const Tag* Tag : Tags) {
        TagsArray.PushBack(Tag->toJSONValue(d), al);
    }
   d.AddMember("Tags", TagsArray, al);

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
    ASSERT_PTR(m_TagMngr);

    std::ifstream ifs(fname.c_str());
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    d.ParseStream(isw);

    if (!d.IsObject() ||
        !d.HasMember("elements") ||
        !d.HasMember("Tags")) {
        debugERROR("Invalid format of the json %s", fname.c_str());
        return false;
    }

    // we need to read the Tags and the elements
    const rapidjson::Value& elemArray = d["elements"];
    if (!elemArray.IsArray()) {
        debugERROR("invalid format, elements is not an array? %s", fname.c_str());
        return false;
    }
    for (std::size_t i = 0; i < elemArray.Size(); ++i) {
        ASSERT(elemArray[i].IsObject());
        // TODO:
//        element e(-1);
//        if (!e.fromJSONValue(elemArray[i])) {
//            debugERROR("Error parsing an element");
//            continue;
//        }
//        m_elementMngr->addElement(e);
    }

    // now the Tags
    const rapidjson::Value& TagsArray = d["Tags"];
    if (!TagsArray.IsArray()) {
        debugERROR("invalid format, Tags is not an array? %s", fname.c_str());
        return false;
    }
    for (std::size_t i = 0; i < TagsArray.Size(); ++i) {
        ASSERT(TagsArray[i].IsObject());
        // TODO
//        Tag t(-1);
//        if (!t.fromJSONValue(TagsArray[i])) {
//            debugERROR("Error parsing an Tag");
//            continue;
//        }
//        m_TagMngr->addTag(t);
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
DataStorage::TagDirty(const Tag* t)
{
    ASSERT_PTR(t);
    debugTODO("IMPLEMENT THIS");
    return true;
}

