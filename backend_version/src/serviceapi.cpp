#include "serviceapi.h"

#include <algorithm>

#include <core/debug/Debug.h>
#include <tags/tagmanager.h>
#include <elements/elementmanager.h>


namespace {

///
/// \brief intersects will check if (s1 & s2) != 0
/// \param s1
/// \param s2
/// \return
///
bool
intersects(const std::set<core::id_t>& s1, const std::set<core::id_t>& s2)
{
    for (const core::id_t& e1 : s1) {
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
ServiceAPI::mapElementIdsToElements(const std::set<core::id_t>& in, std::set<const element*>& o) const
{
    o.clear();
    for (core::id_t id : in) {
        o.insert(m_elementMngr->getElement(id));
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ServiceAPI::ServiceAPI() :
    m_elementMngr(0)
,   m_tagMngr(0)
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
    ASSERT_PTR(data.tagMngr);

    m_tagMngr = data.tagMngr;
    m_elementMngr = data.elemMngr;

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
    ASSERT_PTR(m_tagMngr);
    ASSERT_PTR(m_elementMngr);

    // try to get the current tags if any
    for(auto& tagStr : so.tags) {
        const std::string normalizedTagStr = normalizeWord(tagStr);
        const tag* t = m_tagMngr->getTag(normalizedTagStr);
        if (t == 0) {
            debugWARNING("the tag with string %s was not found", tagStr.c_str());
            continue;
        }
        result.matchedTags.insert(t);
    }

    // now we need to filter all the resulting items
    std::set<core::id_t> commonElemIds;
    bool isFirst = true;
    for (const tag* currTag : result.matchedTags) {
        if (isFirst) {
            isFirst = false;
            commonElemIds = currTag->elementIDsSet();
        } else {
            std::set<core::id_t> tmp;
            std::set_intersection(commonElemIds.begin(),
                                  commonElemIds.end(),
                                  currTag->elementIDsSet().begin(),
                                  currTag->elementIDsSet().end(),
                                  std::inserter(tmp, tmp.begin()));
            commonElemIds = tmp;
        }
    }


    // now we try to expand the current query to get some possible tags
    const std::string normQuery = normalizeWord(so.query);
    std::vector<const tag*> suggestions;
    m_tagMngr->getSuggestedTags(normQuery, suggestions);
    for (const tag* t : suggestions) {
        // here we will filtered out the tags that don't have any element in common
        // with the tags already set by the user
        if (result.matchedTags.empty() ||
            (result.matchedTags.find(t) == result.matchedTags.end() &&
             intersects(t->elementIDsSet(), commonElemIds))) {
            result.expandedTags.insert(t);
        }
    }

    // now we have the associated elements for all the current tags
    // we now build the last part
    std::set<const element*> elemSet;
    mapElementIdsToElements(commonElemIds, elemSet);
    result.expResults[0] = elemSet;
    for (const tag* expTag : result.expandedTags) {
        // get the intersection for this case if and only if there are some
        // tags already set
        std::set<core::id_t> tmp;
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
    ASSERT_PTR(m_tagMngr);

    const std::string normPrefix = normalizeWord(st.prefix);
    std::vector<const tag*> suggestions;
    m_tagMngr->getSuggestedTags(normPrefix, result.tags);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::addTagElement(const tag& t, const element& e)
{
    ASSERT_PTR(m_elementMngr);
    ASSERT_PTR(m_tagMngr);

    tag* currTag = m_tagMngr->getTag(t.text());
    if (currTag == 0) {
        // we need to add it
        currTag = m_tagMngr->createTag(t.text());
    }
    // create a new element and associate it
    element* elem = m_elementMngr->createElement(e.text());

    elem->addTagID(currTag->id());
    currTag->addElementID(elem->id());
    return true;
}

bool
ServiceAPI::addTagElement(const TagElement& d)
{
    ASSERT_PTR(m_elementMngr);
    ASSERT_PTR(m_tagMngr);

    tag* currTag = m_tagMngr->getTag(d.tagText);
    if (currTag == 0) {
        // we need to add it
        currTag = m_tagMngr->createTag(d.tagText);
    }
    // create a new element and associate it
    element* elem = m_elementMngr->createElement(d.elemText);

    elem->addTagID(currTag->id());
    currTag->addElementID(elem->id());
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::removeTag(core::id_t tagID)
{

    ASSERT(false && "Implement");
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::removeElement(core::id_t elemID)
{
    ASSERT(false && "Implement");
    return false;
}
