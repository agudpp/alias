#include <qt_client/qt_client.h>

#include <QApplication>

#include <qxtglobalshortcut/qxtglobalshortcut.h>

#include <toolbox/debug/debug.h>
#include <qt_client/mainwindow.h>


namespace qt_client {


static bool
getKeySequence(const toolbox::Config& config, QKeySequence& result)
{
  toolbox::Config key_bindings_config;
  std::string show_window_sequence;
  if (!config.getConfig("keyBindings", key_bindings_config) ||
      !key_bindings_config.getValue("showMainScreen", show_window_sequence)) {
    LOG_ERROR("No keyBendings section or showMainScreen value found");
    return false;
  }

  result = QKeySequence(QString::fromStdString(show_window_sequence));

  return !result.isEmpty();
}


int
QTClient::execute(int argc,
                  char *argv[],
                  service::ServiceAPI::Ptr service_api,
                  const toolbox::Config& config)
{
  QApplication app( argc, argv );
  MainWindow w(nullptr, service_api);
  w.showNow();

  QKeySequence shortcut;
  if (!getKeySequence(config, shortcut)) {
    return -2;
  }

  const QxtGlobalShortcut globalShortcut(shortcut);

  if (!globalShortcut.isValid()) {
    LOG_ERROR("Failed to set the shortcut " << shortcut.toString().toStdString());
    return -2;
  }

  LOG_INFO("Shortcut for showing main window set: " << shortcut.toString().toStdString());

  QObject::connect(&globalShortcut,
                   &QxtGlobalShortcut::activated,
                   &globalShortcut,
                   [&] {
                     w.showNow();
                   });

  return app.exec();
}


} // namespace qt_client