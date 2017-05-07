#ifndef OSHELPER_H
#define OSHELPER_H

#include <string>
#include <istream>

namespace core {

namespace OSHelper {


///
/// \brief checkFileExists will check if a file exists
/// \param path
/// \return true if exists | false otherwise
///
inline bool
checkFileExists(const char* path);
inline bool
checkFileExists(const std::string& path);



















////////////////////////////////////////////////////////////////////////////////
// Inline implementation
////////////////////////////////////////////////////////////////////////////////

inline bool
checkFileExists(const char* path)
{
    std::ifstream pstream(path);
    return pstream.is_open();
}
inline bool
checkFileExists(const std::string& path)
{
    return checkFileExists(path.c_str());
}


}


}

#endif // OSHELPER_H
