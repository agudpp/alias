#include "serviceapi.h"

#include <algorithm>

#include <core/debug/Debug.h>
#include <tags/tagmanager.h>
#include <elements/elementmanager.h>
#include <consts.h>
#include <datastorage.h>


namespace {

///
/// \brief intersects will check if (s1 & s2) != 0
/// \param s1
/// \param s2
/// \return
///
bool
intersects(const std::set<core::UID>& s1, const std::set<core::UID>& s2)
{
    for (const core::UID& e1 : s1) {
        if (s2.find(e1) != s2.end()) {
            return true;
        }
    }
    return false;
}

}


////////////////////////////////////////////////////////////////////////////////
std::string
ServiceAPI::normalizeWord(const std::string w) const
{
    // TODO: here will be good if we normalize also special characters or
    // other things.
    std::string result = w;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

////////////////////////////////////////////////////////////////////////////////
void
ServiceAPI::mapElementIdsToElements(const std::set<core::UID>& in, std::set<const element*>& o) const
{
    o.clear();
    for (const core::UID id : in) {
        o.insert(m_elementMngr->getElement(id));
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ServiceAPI::ServiceAPI() :
    m_elementMngr(0)
,   m_TagMngr(0)
,   m_dataStorage(0)
{
}

////////////////////////////////////////////////////////////////////////////////
ServiceAPI::~ServiceAPI()
{

}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::init(const MainData& data)
{
    ASSERT_PTR(data.elemMngr);
    ASSERT_PTR(data.TagMngr);
    ASSERT_PTR(data.dataStg);

    m_TagMngr = data.TagMngr;
    m_elementMngr = data.elemMngr;
    m_dataStorage = data.dataStg;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::uninit(void)
{
    // nothing to do for now
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

bool
ServiceAPI::search(const SearchOptions& so, SearchResult& result) const
{
    ASSERT_PTR(m_TagMngr);
    ASSERT_PTR(m_elementMngr);

    // try to get the current Tags if any
    for(auto& TagStr : so.Tags) {
        const std::string normalizedTagStr = normalizeWord(TagStr);
        const Tag* t = m_TagMngr->getTag(normalizedTagStr);
        if (t == 0) {
            debugWARNING("the Tag with string %s was not found", TagStr.c_str());
            continue;
        }
        result.matchedTags.insert(t);
    }

    // now we need to filter all the resulting items
    std::set<core::UID> commonElemIds;
    bool isFirst = true;
    for (const Tag* currTag : result.matchedTags) {
        if (isFirst) {
            isFirst = false;
            commonElemIds = currTag->elementIDsSet();
        } else {
            std::set<core::UID> tmp;
            std::set_intersection(commonElemIds.begin(),
                                  commonElemIds.end(),
                                  currTag->elementIDsSet().begin(),
                                  currTag->elementIDsSet().end(),
                                  std::inserter(tmp, tmp.begin()));
            commonElemIds = tmp;
        }
    }


    // now we try to expand the current query to get some possible Tags
    const std::string normQuery = normalizeWord(so.query);
    std::vector<const Tag*> suggestions;
    m_TagMngr->getSuggestedTags(normQuery, suggestions);
    for (const Tag* t : suggestions) {
        // here we will filtered out the Tags that don't have any element in common
        // with the Tags already set by the user
        if (result.matchedTags.empty() ||
            (result.matchedTags.find(t) == result.matchedTags.end() &&
             intersects(t->elementIDsSet(), commonElemIds))) {
            result.expandedTags.insert(t);
        }
    }

    // now we have the associated elements for all the current Tags
    // we now build the last part
    std::set<const element*> elemSet;
    mapElementIdsToElements(commonElemIds, elemSet);
    result.expResults[0] = elemSet;
    for (const Tag* expTag : result.expandedTags) {
        // get the intersection for this case if and only if there are some
        // Tags already set
        std::set<core::UID> tmp;
        if (!result.matchedTags.empty()) {
            std::set_intersection(commonElemIds.begin(),
                                  commonElemIds.end(),
                                  expTag->elementIDsSet().begin(),
                                  expTag->elementIDsSet().end(),
                                  std::inserter(tmp, tmp.begin()));
        } else {
            tmp = expTag->elementIDsSet();
        }
        mapElementIdsToElements(tmp, elemSet);
        result.expResults[expTag] = elemSet;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::getTags(const SearchTag& st, SearchTagResults& result) const
{
    ASSERT_PTR(m_TagMngr);

    const std::string normPrefix = normalizeWord(st.prefix);
    std::vector<const Tag*> suggestions;
    m_TagMngr->getSuggestedTags(normPrefix, result.Tags);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::addTagElement(const TagElement& d)
{
    ASSERT_PTR(m_elementMngr);
    ASSERT_PTR(m_TagMngr);
    ASSERT_PTR(m_dataStorage);

    // create a new element and associate it
    element* elem = m_elementMngr->createElement(d.elemText);
    // get all the Tags
    for (const std::string& TagText : d.TagsText) {
        Tag* currTag = m_TagMngr->getTag(TagText);
        if (currTag == 0) {
            // we need to add it
            currTag = m_TagMngr->createTag(TagText);
        }
        elem->addTagID(currTag->id());
        currTag->addElementID(elem->id());
        m_dataStorage->TagDirty(currTag);
    }
    m_dataStorage->elementDirty(elem);
    debugTODO("We need to remove this later");
    m_dataStorage->saveToFile(DB_FILE_PATH);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::removeTag(const core::UID TagID)
{

    ASSERT(false && "Implement");
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::removeElement(const core::UID elemID)
{
    ASSERT(false && "Implement");
    return false;
}
