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
class Tag;
class Element;
class DataStorage;

class ServiceAPI
{
public:
    struct MainData {
        TagManager* TagMngr;
        ElementManager* elemMngr;
        DataStorage* dataStg;

        MainData(TagManager* aTagMngr = 0,
                 ElementManager* aElemMngr = 0,
                 DataStorage* aDataStg = 0) :
            TagMngr(aTagMngr)
        ,   elemMngr(aElemMngr)
        ,   dataStg(aDataStg)
        {}
    };

    struct SearchOptions {
        // the current user input
        std::string query;
        // the already setted Tags (if there are some before)
        std::vector<std::string> Tags;
    };

    struct SearchResult {
        // all the Tags we are able to match with the current Tags strings and
        // query
        std::set<const Tag*> matchedTags;
        // expanded possible Tags from last query
        std::set<const Tag*> expandedTags;
        // the expanded possible resulting elements for each of the expanded
        // possibilities, note that the "" (null Tag) will also contain the possible
        // results for the given matchedTags
        std::map<const Tag*, std::set<const Element*> > expResults;
    };

    struct SearchTag {
        // the prefix used to get all the Tags that matches this prefix
        std::string prefix;
    };

    struct SearchTagResults {
        std::vector<const Tag*> Tags;
    };

    struct TagElement {
        // the Tag text
        std::vector<std::string> TagsText;
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
    /// \brief getTags
    /// \param st
    /// \param result
    /// \return
    ///
    bool
    getTags(const SearchTag& st, SearchTagResults& result) const;

    ///
    /// \brief addTagElement
    /// \param t
    /// \param e
    /// \return
    ///
    bool
    addTagElement(const TagElement& d);

    ///
    /// \brief removeTag
    /// \param TagID
    /// \return
    ///
    bool
    removeTag(const core::UID& TagID);
    bool
    removeElement(const core::UID& elemID);

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
    mapElementIdsToElements(const std::set<core::UID>& in, std::set<const Element*>& o) const;

private:
    ElementManager* m_elementMngr;
    TagManager* m_TagMngr;
    DataStorage* m_dataStorage;

};

#endif // SERVICEAPI_H
