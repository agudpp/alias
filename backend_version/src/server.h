/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2017 by Happen
 *
 *   File  created by agustin on 14.5.2017
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef SERVER_H
#define SERVER_H

#include "mongoose.h"
#include "rapidjson/document.h"

class ServiceAPI;

class Server
{
public:
    Server(ServiceAPI* aSvcAPI = 0);

    bool
    start(void);

    // endpoints / functions

    ///
    /// \brief processSearch
    /// \param query
    /// {
    ///     "q": "user current query",
    ///     "tags": [t1,t2,t3] // list of current user tags (if already selected some)
    /// }
    /// \param response
    /// {
    ///     matched_tags: [t1,t2,...],
    ///     expanded_tags: [et1, et2, ...],
    ///     expanded_results: {
    ///         tag_1: [{id: elem_id, content: "text of the element id1}, ...],
    ///         //...
    ///     }
    /// }
    /// \return
    ///
    bool
    processSearch(const rapidjson::Document& query, rapidjson::Document& response);

    ///
    /// \brief getTags will return the list of tags associated to a prefix (query)
    ///        This method will not do any filtering, just tag prefix matching
    /// \param query
    /// {
    ///     "prefix": "tag_prefix"
    /// }
    /// \param response
    /// {
    ///     tags: [t1,...,tN]
    /// }
    /// \return
    ///
    bool
    getTags(const rapidjson::Document& query, rapidjson::Document& response);

    ///
    /// \brief addTagElement
    /// \param query
    /// {
    ///     "tag": {
    ///         "text": "the text to be added"
    ///     },
    ///     "element": {
    ///         "text": "the text content of the element",
    ///     }
    /// }
    /// \param response
    /// {
    ///
    /// }
    /// \return
    ///
    bool
    addTagElement(const rapidjson::Document& query, rapidjson::Document& response);

private:

    static void
    evtHandler(struct mg_connection *nc, int ev, void *p);

private:
    ServiceAPI* m_svcAPI;
    struct mg_mgr m_mgr;
};

#endif // SERVER_H
