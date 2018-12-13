#include "file_storage.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <core/os/OSHelper.h>
#include <core/debug/Debug.h>
#include <core/utils/string_utils.h>

#include <elements/element_builder.h>


#define TYPE_NAME_SEP_CHAR '_'





/**
 * @brief Gets the type and id for the element file
 * @param file_path the file path
 * @return first = type , second id
 */
static std::pair<std::string, std::string>
getTypeAndID(const std::string& file_path)
{
  std::pair<std::string, std::string> result;
  // get the filename
  std::vector<std::string> parts = core::StringUtils::splitStr(file_path, '/');
  if (parts.empty()) {
    return result;
  }
  // now we split by _
  parts = core::StringUtils::splitStr(parts.back(), TYPE_NAME_SEP_CHAR);
  result.first = parts.front();
  result.second = parts.back();
  return result;
}

/**
 * @brief Builds a filename from a given element encoding the type on it
 * @param element the element
 * @return the filename to be used to be able to decode it later
 */
static std::string
buildFileNameFromElement(const Element::Ptr& element)
{
  std::stringstream ss;
  ss << element->elementType() << TYPE_NAME_SEP_CHAR << element->id().toStr() << ".element";
  return ss.str();
}

/**
 * @brief Builds a filename from a given tag
 * @param tag the tag
 * @return the filename to be used to be able to decode it later
 */
static std::string
buildFileNameFromTag(const Tag::Ptr& tag)
{
  std::stringstream ss;
  ss << tag->id().toStr() << ".tag";
  return ss.str();
}




/**
 * @brief Builds an element from a flie path
 * @param file_path the file path
 * @return the element if was possible to be built, or nullptr
 */
static Element::Ptr
elementFromFile(const std::string& file_path)
{
  std::pair<std::string, std::string> encoding_type = getTypeAndID(file_path);
  if (encoding_type.first.empty()) {
    debugERROR("cannot get the encoding type for the element %s", file_path.c_str());
    return Element::Ptr();
  }

  std::ifstream filestream(file_path.c_str(), std::ifstream::in);
  Element::Ptr result = ElementBuilder::build(encoding_type.first, filestream);
  filestream.close();
  return result;
}

/**
 * @brief Builds a tag from a file path
 * @param file_path the file path
 * @return the tag ptr, or null if fail
 */
static Tag::Ptr
tagFromFile(const std::string& file_path)
{
  std::ifstream filestream(file_path.c_str(), std::ifstream::in);
  Tag* tag = new Tag;
  if (!tag->deserialize(filestream)) {
    delete tag;
    return Tag::Ptr();
  }
  filestream.close();
  return Tag::Ptr(tag);
}



void
FileStorage::clear(void)
{
  tags_folder_path_.clear();
  elements_folder_path_.clear();
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

  if (!core::OSHelper::checkFolderExists(path)) {
    debugERROR("The path %s doesnt exists!", path.c_str());
    return;
  }
  const std::string base_path = core::OSHelper::normalizeFolder(path);
  tags_folder_path_ = base_path + "tags/";
  elements_folder_path_ = base_path + "elements/";
  if (!core::OSHelper::createFolder(tags_folder_path_, true) ||
      !core::OSHelper::createFolder(elements_folder_path_, true)) {
    clear();
    debugERROR("Couldnt create the paths");
    return;
  }
  debugINFO("Reading tags from %s\nReading elements from %s\n",
            tags_folder_path_.c_str(),
            elements_folder_path_.c_str());
}

bool
FileStorage::loadAllElements(std::vector<Element::Ptr>& elements)
{
  ASSERT(!elements_folder_path_.empty());

  std::vector<std::string> file_names = core::OSHelper::getFilesInDirectory(elements_folder_path_);
  for (const std::string& fname : file_names) {
    if (fname.find(".element") == std::string::npos) {
      continue;
    }
    const std::string full_path = elements_folder_path_ + fname;
    Element::Ptr element = elementFromFile(full_path);
    if (element.get() != nullptr) {
      elements.push_back(element);
    } else {
      debugERROR("Error getting element for file %s", full_path.c_str());
    }
  }

  return true;
}

bool
FileStorage::loadAllTags(std::vector<Tag::Ptr>& tags)
{
  ASSERT(!tags_folder_path_.empty());

  std::vector<std::string> file_names = core::OSHelper::getFilesInDirectory(tags_folder_path_);
  for (const std::string& fname : file_names) {
    if (fname.find(".tag") == std::string::npos) {
      continue;
    }
    const std::string full_path = tags_folder_path_ + fname;
    Tag::Ptr tag = tagFromFile(full_path);
    if (tag.get() != nullptr) {
      tags.push_back(tag);
    } else {
      debugERROR("Error getting tag for file %s", full_path.c_str());
    }
  }

  return true;
}

bool
FileStorage::saveElement(const Element::Ptr& element)
{
  ASSERT(!elements_folder_path_.empty());
  if (element.get() == nullptr) {
    return false;
  }
  const std::string element_file_name = buildFileNameFromElement(element);
  const std::string full_path = elements_folder_path_ + element_file_name;
  std::ofstream filestream(full_path.c_str(), std::ofstream::out);
  const bool success = element->serialize(filestream);

  if (!success) {
    debugERROR("error deserializing element to be stored on %s", full_path.c_str());

  }

  filestream.close();

  return success;
}

bool
FileStorage::removeElement(const Element::Ptr& element)
{
  if (element.get() == nullptr) {
    return false;
  }
  const std::string element_file_name = buildFileNameFromElement(element);
  const std::string full_path = elements_folder_path_ + element_file_name;
  return core::OSHelper::deleteFile(full_path);
}

bool
FileStorage::saveTag(const Tag::Ptr& tag)
{
  ASSERT(!tags_folder_path_.empty());

  if (tag.get() == nullptr) {
    return false;
  }

  const std::string tag_file_name = buildFileNameFromTag(tag);
  const std::string full_path = tags_folder_path_ + tag_file_name;
  std::ofstream filestream(full_path.c_str(), std::ofstream::out);
  const bool success = tag->serialize(filestream);

  if (!success) {
    debugERROR("error deserializing tag to be stored on %s", full_path.c_str());

  }

  filestream.close();

  return success;
}

bool
FileStorage::removeTag(const Tag::Ptr& tag)
{
  if (tag.get() == nullptr) {
    return false;
  }

  const std::string tag_file_name = buildFileNameFromTag(tag);
  const std::string full_path = tags_folder_path_ + tag_file_name;
  return core::OSHelper::deleteFile(full_path);
}
