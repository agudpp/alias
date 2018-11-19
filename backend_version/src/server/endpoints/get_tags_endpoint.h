#ifndef GET_TAGS_ENDPOINT_H
#define GET_TAGS_ENDPOINT_H

#include <server/endpoint_interface.h>

namespace server {

///
/// \brief getTags will return the list of Tags associated to a prefix (query)
///        This method will not do any filtering, just Tag prefix matching
/// \param query
/// {
///     "prefix": "Tag_prefix"
/// }
/// \param response
/// {
///     Tags: [t1,...,tN]
/// }
/// \return
///
class GetTagsEndpoint : public EndpointInterface
{
  public:
    GetTagsEndpoint();
    virtual ~GetTagsEndpoint(void) {}

    virtual EndpointInterface::ErrorCode
    execute(ServiceAPI* service_api,
            const rapidjson::Document& query,
            rapidjson::Document& response) override;

    /**
     * @brief Returns the endpoint name associated to the caller
     * @return the name
     */
    virtual std::string
    name(void) const override;
};

}

#endif // GET_TAGS_ENDPOINT_H



