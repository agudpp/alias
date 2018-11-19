#ifndef SEARCH_ENDPOINT_H
#define SEARCH_ENDPOINT_H

#include <server/endpoint_interface.h>

namespace server {

///
/// \brief processSearch
/// \param query
/// {
///     "q": "user current query",
///     "Tags": [t1,t2,t3] // list of current user Tags (if already selected some)
/// }
/// \param response
/// {
///     matched_Tags: [t1,t2,...],
///     expanded_Tags: [et1, et2, ...],
///     expanded_results: {
///         Tag_1: [{id: elem_id, content: "text of the element id1}, ...],
///         //...
///     }
/// }
/// \return
///
class SearchEndpoint : public EndpointInterface
{
  public:
    SearchEndpoint();
    virtual ~SearchEndpoint(void) {}

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

#endif // SEARCH_ENDPOINT_H
