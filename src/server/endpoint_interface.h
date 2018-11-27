#ifndef ENDPOINT_INTERFACE_H
#define ENDPOINT_INTERFACE_H

#include <string>

#include <rapidjson/document.h>

#include <service_api/serviceapi.h>



namespace server {

class EndpointInterface {
  public:

    enum class ErrorCode {
      NONE = 0,
      INTERNAL_ERROR,
      INVALID_QUERY,
    };

  public:

    virtual ~EndpointInterface(void) {}

    /**
     * @brief process the endpoint
     * @param service_api the service api
     * @param query the user query
     * @param response the error response
     * @return the error code
     */
    virtual ErrorCode
    execute(ServiceAPI* service_api, const rapidjson::Document& query, rapidjson::Document& response) = 0;

    /**
     * @brief Returns the endpoint name associated to the caller
     * @return the name
     */
    virtual std::string
    name(void) const = 0;
};

} // namespace server

#endif // ENDPOINT_INTERFACE_H
