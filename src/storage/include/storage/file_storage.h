#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <string>

#include <storage/datastorage.h>


class FileStorage : public DataStorage
{
public:
  FileStorage(const std::string& folder_path = "");
  virtual ~FileStorage(void) {}

  /**
   * @brief Sets the folder path to load / store the data
   * @param path the path
   */
  void
  setFolderPath(const std::string& path);

  virtual bool
  loadAllElements(std::vector<Element::Ptr>& elements) override;

  virtual bool
  loadAllTags(std::vector<Tag::Ptr>& tags) override;

  virtual bool
  saveElement(const Element::Ptr& element) override;

  virtual bool
  removeElement(const Element::Ptr& element) override;

  virtual bool
  saveTag(const Tag::Ptr& tag) override;

  virtual bool
  removeTag(const Tag::Ptr& tag) override;

private:

  /**
   * @brief clear paths
   */
  void
  clear(void);


private:
  std::string tags_folder_path_;
  std::string elements_folder_path_;

};

#endif // FILE_STORAGE_H
