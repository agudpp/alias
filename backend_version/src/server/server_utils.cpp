#include "server_utils.h"

namespace server {

void
ServerUtils::buildErrorResp(rapidjson::Document& d,
                            EndpointInterface::ErrorCode err_code,
                            const std::string& msg)
{
    auto& al = d.GetAllocator();
    d.AddMember("error_code", static_cast<int>(err_code), al);
    d.AddMember("error_msg", msg, al);
}

}
