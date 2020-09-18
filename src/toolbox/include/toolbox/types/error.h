#ifndef TOOLBOX_ERROR_H_
#define TOOLBOX_ERROR_H_

#include <string>

namespace toolbox {

enum class ErrorCode {
  NO_ERROR              = 0,
  INTERNAL_ERROR        = 1,
  INVALID_ARGS          = 2,
  NOT_AUTHORIZED        = 3,
};


namespace ErrorCodeUtils {

/**
 * @brief Convert the error to string
 * @param code the error code to convert to string
 * @return the associated string version of the error code
 */
static inline std::string
toStr(const ErrorCode& code)
{
#define DEF_CASE(c_name) case ErrorCode::c_name: return #c_name
  switch(code) {
    DEF_CASE(NO_ERROR);
    DEF_CASE(INTERNAL_ERROR);
    DEF_CASE(INVALID_ARGS);
    DEF_CASE(NOT_AUTHORIZED);
  }
#undef DEF_CASE
  return "UNKNOWN ERROR!";
}


}

}

#endif // TOOLBOX_ERROR_H_
