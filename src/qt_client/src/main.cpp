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

static service::ServiceAPI::Ptr
buildTestService()
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
  qt_client::MainWindow w(nullptr, buildTestService());
  w.show();

//  w.testTags();
  w.testTagSearch();

  return app.exec();
}
