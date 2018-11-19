#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <string>
#include <rapidjson/document.h>

#include <core/debug/Debug.h>
#include <server/endpoint_interface.h>

namespace server {

class ServerUtils
{
  public:

    /**
     * @brief Builds an error response with the given error code and msg
     * @param d the doc to be built
     * @param err_code the code
     * @param msg the message
     */
    static void
    buildErrorResp(rapidjson::Document& d,
                   EndpointInterface::ErrorCode err_code = EndpointInterface::ErrorCode::INTERNAL_ERROR,
                   const std::string& msg = "unknown");
    static inline void
    buildInvalidQueryResp(rapidjson::Document& d, const std::string& msg = "unknown")
    {
      buildErrorResp(d, EndpointInterface::ErrorCode::INVALID_QUERY, msg);
    }
    static inline void
    buildInternalErrResp(rapidjson::Document& d, const std::string& msg = "unknown")
    {
      buildErrorResp(d, EndpointInterface::ErrorCode::INTERNAL_ERROR, msg);
    }


};

}

#endif // SERVER_UTILS_H
