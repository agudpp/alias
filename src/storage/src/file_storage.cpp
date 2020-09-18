#include <storage/file_storage.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <toolbox/os/os_helper.h>
#include <toolbox/debug/debug.h>
#include <toolbox/utils/string_utils.h>

#include <protos/convert_utils.h>

#include <content.pb.h>
#include <tag.pb.h>


namespace storage {


/**
 * @brief Builds a filename from a given content
 * @param contentthe content
 * @return the filename
 */
static std::string
buildFileNameFromContent(const data::Content& content)
{
  std::stringstream ss;
  ss << content.id().toStr() << ".content";
  return ss.str();
}

/**
 * @brief Builds a filename from a given tag
 * @param tag the tag
 * @return the filename to be used to be able to decode it later
 */
static std::string
buildFileNameFromTag(const data::Tag& tag)
{
  std::stringstream ss;
  ss << tag.id().toStr() << ".tag";
  return ss.str();
}


/**
 * @brief Builds an element from a flie path
 * @param file_path the file path
 * @return the element if was possible to be built, or nullptr
 */
static data::Content::Ptr
contentFromFile(const std::string& file_path)
{
  proto::Content proto;
  std::ifstream filestream(file_path.c_str(), std::ifstream::in);
  const bool success = proto.ParseFromIstream(&filestream);

  filestream.close();

  if (!success) {
    LOG_ERROR("we couldn't parse the file " << file_path);
    return data::Content::Ptr();
  }

  return data::Content::Ptr(new data::Content(protos::ConvertUtils::fromProto(proto)));
}

static bool
contentToFile(const data::Content& content, const std::string& file_path)
{
  const proto::Content proto = protos::ConvertUtils::toProto(content);
  const std::string serialized = proto.SerializeAsString();
  return toolbox::OSHelper::writeFileData(file_path, serialized);
}

/**
 * @brief Builds a tag from a file path
 * @param file_path the file path
 * @return the tag ptr, or null if fail
 */
static data::Tag::Ptr
tagFromFile(const std::string& file_path)
{
  proto::Tag proto;
  std::ifstream filestream(file_path.c_str(), std::ifstream::in);
  const bool success = proto.ParseFromIstream(&filestream);

  filestream.close();

  if (!success) {
    LOG_ERROR("Error parsing the file " << file_path);
    return data::Tag::Ptr();
  }

  return data::Tag::Ptr(new data::Tag(protos::ConvertUtils::fromProto(proto)));
}

static bool
tagToFile(const data::Tag& tag, const std::string& file_path)
{
  const proto::Tag proto = protos::ConvertUtils::toProto(tag);
  const std::string serialized = proto.SerializeAsString();
  return toolbox::OSHelper::writeFileData(file_path, serialized);
}


void
FileStorage::clear(void)
{
  tags_folder_path_.clear();
  content_folder_path_.clear();
}


FileStorage::FileStorage(const std::string& folder_path)
{
  if (!folder_path.empty()) {
    setFolderPath(folder_path);
  }
}

void
FileStorage::setFolderPath(const std::string& path)
{
  clear();

  if (!toolbox::OSHelper::checkFolderExists(path)) {
    LOG_ERROR("The path " << path << " doesnt exists!");
    return;
  }
  const std::string base_path = toolbox::OSHelper::normalizeFolder(path);
  tags_folder_path_ = base_path + "tags/";
  content_folder_path_ = base_path + "elements/";
  if (!toolbox::OSHelper::createFolder(tags_folder_path_, true) ||
      !toolbox::OSHelper::createFolder(content_folder_path_, true)) {
    clear();
    LOG_ERROR("Couldnt create the paths");
    return;
  }
  LOG_INFO("Reading tags from " << tags_folder_path_
           << "\nReading elements from " << content_folder_path_);
}

bool
FileStorage::loadAllContent(std::vector<data::Content::Ptr>& contents)
{
  ASSERT(!content_folder_path_.empty());

  std::vector<std::string> file_names = toolbox::OSHelper::getFilesInDirectory(content_folder_path_);
  for (const std::string& fname : file_names) {
    if (fname.find(".content") == std::string::npos) {
      continue;
    }
    const std::string full_path = content_folder_path_ + fname;
    data::Content::Ptr content = contentFromFile(full_path);
    if (content.get() != nullptr) {
      contents.push_back(content);
    }
  }

  return true;
}

bool
FileStorage::loadAllTags(std::vector<data::Tag::Ptr>& tags)
{
  ASSERT(!tags_folder_path_.empty());

  std::vector<std::string> file_names = toolbox::OSHelper::getFilesInDirectory(tags_folder_path_);
  for (const std::string& fname : file_names) {
    if (fname.find(".tag") == std::string::npos) {
      continue;
    }
    const std::string full_path = tags_folder_path_ + fname;
    data::Tag::Ptr tag = tagFromFile(full_path);
    if (tag.get() != nullptr) {
      tags.push_back(tag);
    }
  }

  return true;
}

bool
FileStorage::saveContent(const data::Content::Ptr& content)
{
  ASSERT(!content_folder_path_.empty());
  if (content.get() == nullptr) {
    return false;
  }
  const std::string content_file_name = buildFileNameFromContent(*content);
  const std::string full_path = content_folder_path_ + content_file_name;

  if (!contentToFile(*content, full_path)) {
    LOG_ERROR("error deserializing element to be stored on " << full_path);
    return false;
  }

  return true;
}

bool
FileStorage::removeContent(const data::Content::Ptr& content)
{
  if (content.get() == nullptr) {
    return false;
  }
  const std::string content_file_name = buildFileNameFromContent(*content);
  const std::string full_path = content_folder_path_ + content_file_name;
  return toolbox::OSHelper::deleteFile(full_path);
}

bool
FileStorage::saveTag(const data::Tag::Ptr& tag)
{
  ASSERT(!tags_folder_path_.empty());

  if (tag.get() == nullptr) {
    return false;
  }

  const std::string tag_file_name = buildFileNameFromTag(*tag);
  const std::string full_path = tags_folder_path_ + tag_file_name;

  if (!tagToFile(*tag, full_path)) {
    LOG_ERROR("error deserializing tag to be stored on " << full_path);
    return false;
  }

  return true;
}

bool
FileStorage::removeTag(const data::Tag::Ptr& tag)
{
  if (tag.get() == nullptr) {
    return false;
  }

  const std::string tag_file_name = buildFileNameFromTag(*tag);
  const std::string full_path = tags_folder_path_ + tag_file_name;
  return toolbox::OSHelper::deleteFile(full_path);
}


} // namespace storage

