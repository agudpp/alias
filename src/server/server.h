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

#include <server/mongoose.h>
#include <rapidjson/document.h>

class ServiceAPI;

class Server
{
public:
    Server(ServiceAPI* aSvcAPI = nullptr);

    bool
    start(void);

    // endpoints / functions


    bool
    processSearch(const rapidjson::Document& query, rapidjson::Document& response);


    bool
    getTags(const rapidjson::Document& query, rapidjson::Document& response);

    bool
    addElement(const rapidjson::Document& query, rapidjson::Document& response);

    bool
    updateElement(const rapidjson::Document& query, rapidjson::Document& response);

    bool
    removeElement(const rapidjson::Document& query, rapidjson::Document& response);

private:

    static void
    evtHandler(struct mg_connection *nc, int ev, void *p);

private:
    ServiceAPI* m_svcAPI;
    struct mg_mgr m_mgr;
};

#endif // SERVER_H
