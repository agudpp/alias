#include <qt_client/common/converter_utils.h>


namespace qt_client {


std::vector<std::string>
ConverterUtils::toTextVec(const std::set<data::Tag::ConstPtr>& tags)
{
  std::vector<std::string> result;
  for (auto& tag : tags) {
    result.push_back(tag->name());
  }
  return result;
}

std::vector<std::string>
ConverterUtils::toTextVec(const std::vector<TagWidget*>& tags)
{
  std::vector<std::string> result;
  for (auto& tag : tags) {
    result.push_back(tag->tag()->name());
  }
  return result;
}

std::vector<data::Content::ConstPtr>
ConverterUtils::toVec(const std::set<data::Content::ConstPtr>& contents)
{
  std::vector<data::Content::ConstPtr> result;
  for (auto& content : contents) {
    result.push_back(content);
  }
  return result;
}




} // namespace qt_client
