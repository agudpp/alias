/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2017 by Happen
 *
 *   File  created by agustin on 14.5.2017
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include "server.h"

#include <vector>

#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <core/debug/Debug.h>
#include <tags/tag.h>
#include <elements/element.h>
#include "serviceapi.h"



namespace {

static Server* gServerInstance = 0;

static void
buildErrorResp(rapidjson::Document& d,
               int errCode = -1,
               const std::string& msg = "unknown")
{
    auto& al = d.GetAllocator();
    d.AddMember("error_code", errCode, al);
    d.AddMember("error_msg", msg, al);
}
static void
formatResp(rapidjson::Document& d)
{
    d.AddMember("error_code", 0, d.GetAllocator());
}

static void
sendResponse(struct mg_connection *nc, rapidjson::Document& d)
{
    rapidjson::StringBuffer buffer;
    /// TODO: change for a not pretty writter here if not debug
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    const char* response = buffer.GetString();
//    mg_send(nc, response, std::strlen(response));  // Echo message back
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "%s", response);
    mg_send_http_chunk(nc, "", 0); // Tell the client we're finished
}

template<typename T>
inline static std::string
getString(T& d, const char* fieldName)
{
    const rapidjson::Value& queryVal = d[fieldName];
    ASSERT(queryVal.IsString());
    return queryVal.GetString();
}
inline static void
getStrArray(const rapidjson::Document& d,
            const char* fieldName,
            std::vector<std::string>& result)
{
    const rapidjson::Value& val= d[fieldName];
    ASSERT(val.IsArray());
    result.clear();
    result.reserve(val.Size());
    for (int i = 0; i < val.Size(); ++i) {
        ASSERT(val[i].IsString());
        result.push_back(val[i].GetString());
    }
}

}




////////////////////////////////////////////////////////////////////////////////
void
Server::evtHandler(struct mg_connection *nc, int ev, void *p)
{

    switch (ev) {
    case MG_EV_HTTP_REQUEST: //MG_EV_RECV:
        {
            Server* instance = gServerInstance;
            ASSERT_PTR(instance);
            struct http_message *hm = (struct http_message *) p;

            // parse the json
            rapidjson::Document query, result;
            result.SetObject();

            debugINFO("got %s", hm->body.p);

            query.Parse(hm->body.p, hm->body.len);
//            mbuf_remove(io, io->len);       // Discard message from recv buffer
            if (!query.IsObject() || !query.HasMember("endpoint")) {
                debugINFO("discarding invalid formated query?");
                buildErrorResp(result, -1, "invalid query format");
                sendResponse(nc, result);
                return;
            }

            // else we have a known endpoint, check if we have it
            rapidjson::Value& val = query["endpoint"];
            const std::string endpoint = val.GetString();
            if (endpoint == "search") {
                instance->processSearch(query, result);
            } else if (endpoint == "add_tag_elem") {
                instance->addTagElement(query, result);
            } else {
                debugINFO("we don't support the endpoint %s?..", endpoint.c_str());
                buildErrorResp(result, -2, "no valid endpoint?");
                sendResponse(nc, result);
                return;
            }

            debugINFO("query processed properly...");
            formatResp(result);
            sendResponse(nc, result);
        }
        break;
    case MG_EV_SEND:
        debugINFO("DATA HAS BEEN WRITTEN ON TH BUFFER");
        break;
    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
Server::Server(ServiceAPI *aSvcAPI) :
    m_svcAPI(aSvcAPI)
{
}


////////////////////////////////////////////////////////////////////////////////
bool
Server::start(void)
{
    ASSERT_PTR(m_svcAPI);
    gServerInstance = this;

    const char *port1 = "1234";

    mg_mgr_init(&m_mgr, this);
    struct mg_connection *c = mg_bind(&m_mgr, port1, Server::evtHandler);
    mg_set_protocol_http_websocket(c);

    debugINFO("Starting echo mgr on ports %s\n", port1);
    for (;;) {
//        debugINFO("processing...");
        mg_mgr_poll(&m_mgr, 1000);
    }
    mg_mgr_free(&m_mgr);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
Server::processSearch(const rapidjson::Document& query, rapidjson::Document& response)
{
    ASSERT_PTR(m_svcAPI);

    // check that the query contains the fields we need
    if (!query.HasMember("query") || !query.HasMember("tags")) {
        buildErrorResp(response, -1, "Invalid search query fields");
        return false;
    }
    // now we perform the query
    ServiceAPI::SearchOptions searchOpt;

    searchOpt.query = getString(query, "query");
    getStrArray(query, "tags", searchOpt.tags);

    ServiceAPI::SearchResult searchResults;
    if (!m_svcAPI->search(searchOpt, searchResults)) {
        debugWARNING("We couldn't perform the search query for some reason");
        buildErrorResp(response, -2, "internal error performing the search query");
        return false;
    }

    // transform the results into a json object
    auto& al = response.GetAllocator();
    rapidjson::Value tagArray(rapidjson::kArrayType);
    for (const tag* t : searchResults.matchedTags) {
        tagArray.PushBack(rapidjson::StringRef(t->text()), al);
    }
    response.AddMember("matched_tags", tagArray, al);

    // expanded tags
    rapidjson::Value exptagArray(rapidjson::kArrayType);
    for (const tag* t : searchResults.expandedTags) {
        exptagArray.PushBack(rapidjson::StringRef(t->text()), al);
    }
    response.AddMember("expanded_tags", exptagArray, al);

    // expanded results
    ///     expanded_results: {
    ///         tag_1: [{id: elem_id, content: "text of the element id1}, ...],
    ///         //...
    ///     }
    rapidjson::Value expRes(rapidjson::kObjectType);
    for (auto it = searchResults.expResults.begin(); it != searchResults.expResults.end(); ++it) {
        const tag* t = it->first;
        const std::set<const element*>& elemSet = it->second;
        const char* keyName = (t == 0) ? "" : t->text().c_str();
        rapidjson::Value elemsArray(rapidjson::kArrayType);
        for (const element* e : elemSet) {
            rapidjson::Value elemObj(rapidjson::kObjectType);
            elemObj.AddMember("id", e->id(), al);
            elemObj.AddMember("content", e->text(), al);
            elemsArray.PushBack(elemObj, al);
        }
        expRes.AddMember(rapidjson::StringRef(keyName), elemsArray, al);
    }
    response.AddMember("expanded_results", expRes, al);

    return true;
}

bool
Server::addTagElement(const rapidjson::Document& query, rapidjson::Document& response)
{
    ASSERT_PTR(m_svcAPI);

    // check that the query contains the fields we need
    if (!query.HasMember("tag") || !query.HasMember("element")) {
        buildErrorResp(response, -1, "Missing addTagElement arguments");
        return false;
    }
    // now we perform the query
    const rapidjson::Value& tagVal = query["tag"];
    const rapidjson::Value& elemVal = query["element"];
    if (!tagVal.IsObject() || !tagVal.HasMember("text") ||
        !elemVal.IsObject() || !elemVal.HasMember("text")) {
        buildErrorResp(response, -1, "Invalid type of arguments?");
        return false;
    }

    ServiceAPI::TagElement tagElemData;
    tagElemData.tagText = getString(tagVal, "text");
    tagElemData.elemText = getString(elemVal, "text");
    if (!m_svcAPI->addTagElement(tagElemData)) {
        debugWARNING("Something went wrong when adding a new tag element data");
        buildErrorResp(response, -2, "Couldn't add the tag element...");
        return false;
    }
    // alles gut
    return true;
}


