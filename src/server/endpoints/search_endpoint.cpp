#include <server/endpoints/search_endpoint.h>

#include <rapidjson/document.h>

#include <core/debug/Debug.h>
#include <core/utils/json_utils.h>
#include <server/server_utils.h>


namespace server {

EndpointInterface::ErrorCode
SearchEndpoint::execute(ServiceAPI* service_api,
                        const rapidjson::Document& query,
                        rapidjson::Document& response)
{
  ASSERT_PTR(service_api);

  // check that the query contains the fields we need
  if (!query.HasMember("query") || !query.HasMember("tags")) {
      ServerUtils::buildInvalidQueryResp(response,  "Invalid search query fields");
      return EndpointInterface::ErrorCode::INVALID_QUERY;
  }

  // now we perform the query
  ServiceAPI::SearchOptions searchOpt;

  searchOpt.query = core::JsonUtils::getString(query, "query");
  core::JsonUtils::getStrArray(query, "tags", searchOpt.tags);

  ServiceAPI::SearchResult searchResults;
  if (!service_api->search(searchOpt, searchResults)) {
      debugWARNING("We couldn't perform the search query for some reason");
      ServerUtils::buildInternalErrResp(response, "internal error performing the search query");
      return EndpointInterface::ErrorCode::INTERNAL_ERROR;
  }

  // transform the results into a json object
  auto& al = response.GetAllocator();
  rapidjson::Value tagArray(rapidjson::kArrayType);
  for (const Tag* t : searchResults.matched_tags) {
      tagArray.PushBack(rapidjson::StringRef(t->text()), al);
  }
  response.AddMember("matched_Tags", tagArray, al);

  // expanded Tags
  rapidjson::Value expTagArray(rapidjson::kArrayType);
  for (const Tag* t : searchResults.expanded_tags) {
      expTagArray.PushBack(rapidjson::StringRef(t->text()), al);
  }
  response.AddMember("expanded_Tags", expTagArray, al);

  // expanded results
  ///     expanded_results: {
  ///         Tag_1: [{id: elem_id, content: "text of the element id1}, ...],
  ///         //...
  ///     }
  rapidjson::Value expRes(rapidjson::kObjectType);
  for (auto it = searchResults.exp_results.begin(); it != searchResults.exp_results.end(); ++it) {
      const Tag* t = it->first;
      const std::set<const Element*>& elemSet = it->second;
      const char* keyName = (t == nullptr) ? "" : t->text().c_str();
      rapidjson::Value elemsArray(rapidjson::kArrayType);
      for (const Element* e : elemSet) {
          elemsArray.PushBack(e->toJsonValue(response), al);
      }
      expRes.AddMember(rapidjson::StringRef(keyName), elemsArray, al);
  }
  response.AddMember("expanded_results", expRes, al);

  return EndpointInterface::ErrorCode::NONE;
}

std::string
SearchEndpoint::name(void) const
{
  return "search";
}


}
