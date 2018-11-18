#ifndef OSHELPER_H
#define OSHELPER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#include <cstdlib>
#include <string>
#include <istream>
#include <sstream>
#include <vector>



namespace core {

namespace OSHelper {

/**
 * @brief checkFileExists will check if a file exists
 * @param path the path
 * @return true if exists | false otherwise
 */
inline bool
checkFileExists(const char* path);
inline bool
checkFileExists(const std::string& path);


/**
 * @brief check if a folder exists
 * @param path the path to the folder
 * @return true if it does, false otherwise
 */
inline bool
checkFolderExists(const std::string& path);

/**
 * @brief Normalizes a folder path
 * @param path the folder path
 * @return the normalized folder path
 */
inline std::string
normalizeFolder(const std::string& path);


/**
 * @brief Creates a folder if not exists
 * @param path the folder to be created
 * @param recursive_create the "-p" flag of mkdir
 * @return true on success | false therwise
 */
inline bool
createFolder(const std::string& path, bool recursive_create);


/**
 * @brief Returns a list of file names on the given directory
 * @param dir_path the directory path
 * @return the list of files on the given directory
 */
inline std::vector<std::string>
getFilesInDirectory(const std::string& dir_path);


/**
 * @brief Deletes a file from the system
 * @param file_path the file path to be deleted
 * @return true on succees, false otherwise
 */
inline bool
deleteFile(const std::string& file_path);




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

inline bool
checkFolderExists(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
      return false;
    }
    return info.st_mode & S_IFDIR;
}

inline std::string
normalizeFolder(const std::string& path)
{
  if (path.back() != '/') {
    return path + "/";
  }
  return path;
}

inline bool
createFolder(const std::string& path, bool recursive_create)
{
  std::stringstream ss;
  ss << "mkdir ";
  if (recursive_create) {
    ss << "-p ";
  }
  ss << path;
  const std::string command = ss.str();
  return std::system(command.c_str()) == 0;
}

inline std::vector<std::string>
getFilesInDirectory(const std::string& dir_path)
{
  std::vector<std::string> result;
  struct dirent *directory;
  DIR* dirp = opendir(dir_path.c_str());
  if (dirp != nullptr) {
      while ((directory = readdir(dirp)) != nullptr) {
        result.push_back(directory->d_name);
      }
      closedir(dirp);
  }
  return result;
}

inline bool
deleteFile(const std::string& file_path)
{
  std::remove(file_path.c_str());
  return !checkFileExists(file_path);
}



} // namespace OSHelper
} // namespace core

#endif // OSHELPER_H
