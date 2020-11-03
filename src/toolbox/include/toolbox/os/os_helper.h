#ifndef TOOLBOX_OS_HELPER_H_
#define TOOLBOX_OS_HELPER_H_

#ifdef _WIN32
  #include <windows.h>
  #include <Shlobj.h>
#else
  #include <pwd.h>
#endif
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>



namespace toolbox::OSHelper {

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
 * @brief Return the home directory
 * @return the home directory
 */
inline std::string
getHomeDir(void);

/**
 * @brief Returns the current working directory (PWD)
 * @return the current working directory
 */
inline std::string
currentWorkingDir();

/**
 * @brief Executes a given command / binary with arguments
 * @param command the command / binary to execute
 * @param arguments the arguments
 * @param detached if we want to run the command detached from the parent process (&)
 * @return the return code of the execution
 */
int
execute(const std::string& command, const std::string& arguments, bool detached = false);

/**
 * @brief Kills a process by name
 * @param name the name of the process
 * @return 0 on success | error code otherwise
 */
int
killByName(const std::string& name);

/**
 * @brief Will get an environmental variable and return it over argument
 * @param env_var_name the name of the variable to get
 * @param result the resulting value of the variable
 * @return true if we found it, false otherwise
 */
inline bool
getEnvVar(const std::string& env_var_name, std::string& result);

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
 * @brief Normalizes a file path
 * @param folder the folder path
 * @param file_name the file name
 * @return the normalized path
 */
inline std::string
normalizeFilePath(const std::string& folder, const std::string& file_name);

/**
 * @brief Expands a file path into a full path if it contains the '~' for example
 * @param path  The path to expand
 * @return the expanded path
 */
inline std::string
expandFilePath(const std::string& path);

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

/**
 * @brief Deletes a folder with all its content
 * @param folder the folder path
 * @return true on success | false otherwise
 */
bool
deleteFolder(const std::string& folder);

/**
 * @brief Writes a buffer into a file
 * @param file_path the file path
 * @param data the data
 * @return true on success | false otherwise
 */
inline bool
writeFileData(const std::string& file_path, const std::string& data);

/**
 * @brief Read the file content into data parameter
 * @param file_path the file path to read
 * @param data the resulting data read from the file
 * @return true if we were able to read it, false otherwise
 */
inline bool
readFileData(const std::string& file_path, std::string& data);

/**
 * @brief Returns the unique system id
 * @return the unique system id
 */
std::string
getUniqueSystemID(void);


////////////////////////////////////////////////////////////////////////////////
// Inline implementation
////////////////////////////////////////////////////////////////////////////////

inline bool
checkFileExists(const char* path)
{
  return std::filesystem::exists(path);
}
inline bool
checkFileExists(const std::string& path)
{
  return checkFileExists(path.c_str());
}

inline std::string
getHomeDir(void)
{
  std::string result;
  #ifdef _WIN32 
    WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
      std::wstring wresult = path;
      result = std::string(wresult.begin(), wresult.end());
    } else {
      return result;
    }
  #else
    const struct passwd* pw = getpwuid(getuid());
    if (pw == nullptr) {
      return result;
    }
    const char* homedir = pw->pw_dir;
    if (homedir == nullptr) {
      return result;
    }
    result = homedir;
  #endif

  return normalizeFolder(result);
}

inline std::string
currentWorkingDir()
{
  return std::filesystem::current_path().string();
}

inline bool
getEnvVar(const std::string& env_var_name, std::string& result)
{
  const char* env_p = std::getenv(env_var_name.c_str());
  if (env_p == nullptr) {
    return false;
  }
  result = env_p;
  return true;
}

inline bool
checkFolderExists(const std::string& path)
{
  return std::filesystem::exists(path);
}

inline std::string
normalizeFolder(const std::string& path)
{
  if (path.back() != '/') {
    return path + "/";
  }
  return path;
}

inline std::string
normalizeFilePath(const std::string& folder, const std::string& file_name)
{
  const auto path = std::filesystem::path(folder) / file_name;
  return std::filesystem::canonical(path).string();
}

inline std::string
expandFilePath(const std::string& path)
{
  if (!path.empty() && path.front() == '~') {
    return getHomeDir() + path.substr(1);
  }
  return path;
}

inline bool
createFolder(const std::string& path, bool recursive_create)
{
  if (checkFolderExists(path)) {
    return true;
  }
  return recursive_create
    ? std::filesystem::create_directories(path)
    : std::filesystem::create_directory(path);
}

inline std::vector<std::string>
getFilesInDirectory(const std::string& dir_path)
{
  std::vector<std::string> result;
  for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
    if (entry.is_regular_file()) {
      result.push_back(entry.path().filename().string());
    }
  }

  return result;
}

inline bool
deleteFile(const std::string& file_path)
{
  return std::filesystem::remove(file_path);
}

inline bool
writeFileData(const std::string& file_path, const std::string& data)
{
  std::ofstream ofile(file_path.c_str());
  if (!ofile) {
    return false;
  }
  ofile.write(data.c_str(), data.size());
  const bool is_good = ofile.good();
  ofile.close();

  return is_good;
}

inline bool
readFileData(const std::string& file_path, std::string& data)
{
  data.clear();
  std::ifstream input_file(file_path.c_str());
  if (!input_file) {
    return false;
  }
  input_file.seekg(0, std::ios::end);
  data.reserve(input_file.tellg());
  input_file.seekg(0, std::ios::beg);

  data.assign((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
  input_file.close();

  return true;
}


} // namespace toolbox::OSHelper


#endif // TOOLBOX_OS_HELPER_H_
