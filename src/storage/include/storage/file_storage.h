#ifndef STORAGE_FILE_STORAGE_H_
#define STORAGE_FILE_STORAGE_H_

#include <string>

#include <storage/data_storage.h>



namespace storage {


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
  loadAllContent(std::vector<data::Content::Ptr>& contents) override;

  virtual bool
  loadAllTags(std::vector<data::Tag::Ptr>& tags) override;

  virtual bool
  saveContent(const data::Content::Ptr& content) override;

  virtual bool
  removeContent(const data::Content::Ptr& content) override;

  virtual bool
  saveTag(const data::Tag::Ptr& tag) override;

  virtual bool
  removeTag(const data::Tag::Ptr& tag) override;

private:

  /**
   * @brief clear paths
   */
  void
  clear(void);


private:
  std::string tags_folder_path_;
  std::string content_folder_path_;

};

} // namespace storage

#endif // STORAGE_FILE_STORAGE_H_
