#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <memory>
#include <fstream>

#include <QApplication>

#include <toolbox/debug/debug.h>

#include <service/service_api.h>
#include <storage/dummy_storage.h>
#include <data/data_mapper.h>

#include <qt_client/mainwindow.h>

// TODO: remove this test

static data::Content::Ptr
createTextContent(const std::string& content, const std::set<data::Tag::ConstPtr>& tags)
{
  data::Content::Ptr result(new data::Content);
  data::Metadata metadata;
  metadata.setType(data::ContentType::TEXT);
  result->setMetadata(metadata);
  result->setData(content);

  for (auto& tag : tags) {
    result->addTag(tag->id());
  }

  return result;
}

static service::ServiceAPI::Ptr
buildTestService(std::vector<data::Content::Ptr>& contents)
{
  std::vector<std::string> tag_names = {
    "tag1",
    "tag11",
    "some-other-tag",
    "reliquid"
  };
  data::DataMapper::Ptr data_mapper(new data::DataMapper);
  for (auto& tag_name : tag_names) {
    data::Tag::Ptr tag_ptr(new data::Tag(tag_name));
    data_mapper->addTag(tag_ptr);
  }

  data::Content::Ptr c1 = createTextContent("content number 1", {data_mapper->tagFromName("tag1")});
  data::Content::Ptr c2 = createTextContent("content number 2", {
                                              data_mapper->tagFromName("tag1"),
                                              data_mapper->tagFromName("reliquid"),
                                            });
  data_mapper->addContent(c1);
  data_mapper->addContent(c2);

  contents.push_back(c1);
  contents.push_back(c2);

  storage::DataStorage::Ptr data_storage(new storage::DummyStorage());
  return service::ServiceAPI::Ptr(new service::ServiceAPI(data_mapper, data_storage));
}

int
main(int argc, char *argv[])
{
  _CONFIG_BASIC_LOGGERS;

//  if (argc < 2) {
//    LOG_ERROR("Missing config file path as argument");
//    return -1;
//  }

  QApplication app( argc, argv );
  std::vector<data::Content::Ptr> contents;
  qt_client::MainWindow w(nullptr, buildTestService(contents));
  w.show();

//  w.testTags();
//  w.testTagSearch();
  w.testEdition(contents.back());

  return app.exec();
}
