#ifndef ADD_ELEMENT_ENDPOINT_H
#define ADD_ELEMENT_ENDPOINT_H

#include <server/endpoint_interface.h>

namespace server {

///
/// \brief endpoint: add_element
/// \param query
/// {
///     "tags": [
///         {
///             "text": "the text to be added"
///         },
///         {
///             "text": "the text to be added"
///         }
///     ],
///     "element": {
///         "type": "the type of the element"
///         "data": "json to serialized from",
///     }
/// }
/// \param response
/// {
///
/// }
/// \return
///
class AddElementEndpoint : public EndpointInterface
{
  public:
    AddElementEndpoint();
    virtual ~AddElementEndpoint(void) {}

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

#endif // ADD_ELEMENT_ENDPOINT_H


