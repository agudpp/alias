#ifndef SERVICEAPI_H
#define SERVICEAPI_H

#include <string>
#include <vector>
#include <map>
#include <set>

#include <core/types/basics.h>
#include <core/types/id_type.h>


// FWD
class TagManager;
class ElementManager;
class tag;
class element;

class ServiceAPI
{
public:
    struct MainData {
        TagManager* tagMngr;
        ElementManager* elemMngr;

        MainData(TagManager* aTagMngr = 0, ElementManager* aElemMngr = 0) :
            tagMngr(aTagMngr)
        ,   elemMngr(aElemMngr)
        {}
    };

    struct SearchOptions {
        // the current user input
        std::string query;
        // the already setted tags (if there are some before)
        std::vector<std::string> tags;
    };

    struct SearchResult {
        // all the tags we are able to match with the current tags strings and
        // query
        std::set<const tag*> matchedTags;
        // expanded possible tags from last query
        std::set<const tag*> expandedTags;
        // the expanded possible resulting elements for each of the expanded
        // possibilities, note that the "" (null tag) will also contain the possible
        // results for the given matchedTags
        std::map<const tag*, std::set<const element*> > expResults;
    };

    struct TagElement {
        // the tag text
        std::string tagText;
        // the element text
        std::string elemText;
    };

public:
    ServiceAPI();
    ~ServiceAPI();

    ///
    /// \brief init
    /// \param data
    /// \return
    ///
    bool
    init(const MainData& data);

    ///
    /// \brief uninit
    /// \return
    ///
    bool
    uninit(void);

    ////////////////////////////////////////////////////////////////////////////
    // API
    //

    ///
    /// \brief search
    /// \param so
    /// \param result
    /// \return
    ///
    bool
    search(const SearchOptions& so, SearchResult& result) const;

    ///
    /// \brief addTagElement
    /// \param t
    /// \param e
    /// \return
    ///
    bool
    addTagElement(const tag& t, const element& e);
    bool
    addTagElement(const TagElement& d);

    ///
    /// \brief removeTag
    /// \param tagID
    /// \return
    ///
    bool
    removeTag(core::id_t tagID);
    bool
    removeElement(core::id_t elemID);

private:

    ///
    /// \brief normalizeWord
    /// \param w
    /// \return
    ///
    std::string
    normalizeWord(const std::string w) const;

    ///
    /// \brief mapElementIdsToElements
    /// \param in
    /// \param o
    ///
    void
    mapElementIdsToElements(const std::set<core::id_t>& in, std::set<const element*>& o) const;

private:
    ElementManager* m_elementMngr;
    TagManager* m_tagMngr;

};

#endif // SERVICEAPI_H
