
#include <server/endpoints/add_element_endpoint.h>


#include <rapidjson/document.h>

#include <core/debug/Debug.h>
#include <core/utils/json_utils.h>
#include <server/server_utils.h>

#include <elements/element.h>
#include <elements/element_builder.h>


namespace server {


EndpointInterface::ErrorCode
AddElementEndpoint::execute(ServiceAPI* service_api,
                         const rapidjson::Document& query,
                         rapidjson::Document& response)
{
  ASSERT_PTR(service_api);

  // check that the query contains the fields we need
  if (!query.HasMember("tags") || !query.HasMember("element")) {
      ServerUtils::buildInvalidQueryResp(response, "Missing addTagElement arguments");
      return EndpointInterface::ErrorCode::INVALID_QUERY;
  }
  // now we perform the query
  const rapidjson::Value& elemVal = query["element"];
  const rapidjson::Value& tags = query["tags"];
  if (!elemVal.IsObject() || !elemVal.HasMember("text") ||
      !tags.IsArray()) {
      ServerUtils::buildInvalidQueryResp(response, "Invalid type of arguments?");
      return EndpointInterface::ErrorCode::INVALID_QUERY;
  }

  ServiceAPI::ElementData element_data;
  element_data.tags_text.reserve(tags.Size());

  for (std::size_t i = 0; i < tags.Size(); ++i) {
      if (!tags[i].IsObject() || !tags[i].HasMember("text")) {
          debugWARNING("invalid argument on the query, should be an object the Tag");
          continue;
      }
      element_data.tags_text.push_back(core::JsonUtils::getString(tags[i], "text"));
  }

  element_data.element_data = getString(elemVal, "text");
  if (!service_api->addTagElement(tagElemData)) {
      debugWARNING("Something went wrong when adding a new Tag element data");
      ServerUtils::buildInternalErrResp(response,"Couldn't add the Tag element...");
      return EndpointInterface::ErrorCode::INTERNAL_ERROR;
  }

  return EndpointInterface::ErrorCode::NONE;
}

std::string
AddElementEndpoint::name(void) const
{
  return "add_element";
}


}



