#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <memory>
#include <fstream>

#include <toolbox/debug/debug.h>

#include <QApplication>
#include <QTextStream>

#include <qt_client/mainwindow.h>

int
main(int argc, char *argv[])
{
  _CONFIG_BASIC_LOGGERS;

//  if (argc < 2) {
//    LOG_ERROR("Missing config file path as argument");
//    return -1;
//  }

  QApplication app( argc, argv );
  qt_client::MainWindow w(nullptr);
  w.show();

  return app.exec();
}
