#include "serviceapi.h"

#include <algorithm>

#include <core/debug/Debug.h>
#include <tags/tagmanager.h>
#include <elements/elementmanager.h>



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

    // now we try to expand the current query to get some possible tags
    const std::string normQuery = normalizeWord(so.query);
    std::vector<const tag*> suggestions;
    m_tagMngr->getSuggestedTags(normQuery, suggestions);
    for (const tag* t : suggestions) {
        result.expandedTags.insert(t);
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

    // now we have the associated elements for all the current tags
    // we now build the last part
    std::set<const element*> elemSet;
    mapElementIdsToElements(commonElemIds, elemSet);
    result.expResults[0] = elemSet;
    for (const tag* expTag : result.expandedTags) {
        // get the intersection for this case
        std::set<core::id_t> tmp;
        std::set_intersection(commonElemIds.begin(),
                              commonElemIds.end(),
                              expTag->elementIDsSet().begin(),
                              expTag->elementIDsSet().end(),
                              std::inserter(tmp, tmp.begin()));
        mapElementIdsToElements(commonElemIds, elemSet);
        result.expResults[expTag] = elemSet;
    }

    return true;
}
