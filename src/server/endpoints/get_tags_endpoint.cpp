#include <server/endpoints/get_tags_endpoint.h>


#include <rapidjson/document.h>

#include <core/debug/Debug.h>
#include <core/utils/json_utils.h>
#include <server/server_utils.h>


namespace server {

EndpointInterface::ErrorCode
GetTagsEndpoint::execute(ServiceAPI* service_api,
                         const rapidjson::Document& query,
                         rapidjson::Document& response)
{
  ASSERT_PTR(service_api);

  // check that the query contains the fields we need
  if (!query.HasMember("prefix")) {
      ServerUtils::buildInvalidQueryResp(response, "Invalid getTags fields, prefix missing");
      return EndpointInterface::ErrorCode::INVALID_QUERY;
  }

  // now we perform the query
  ServiceAPI::SearchTag searchOpt;
  ServiceAPI::SearchTagResults results;

  searchOpt.prefix = core::JsonUtils::getString(query, "prefix");
  if (!service_api->getTags(searchOpt, results)) {
      debugWARNING("We couldn't perform the get Tags query for some reason");
      ServerUtils::buildInternalErrResp(response, "internal error performing the get Tags query");
      return EndpointInterface::ErrorCode::INTERNAL_ERROR;
  }

  // transform the results into a json object
  auto& al = response.GetAllocator();
  rapidjson::Value tagArray(rapidjson::kArrayType);
  for (const Tag* t : results.tags) {
      tagArray.PushBack(rapidjson::StringRef(t->text()), al);
  }
  response.AddMember("tags", tagArray, al);

  return EndpointInterface::ErrorCode::NONE;
}

std::string
GetTagsEndpoint::name(void) const
{
  return "get_tags";
}


}



