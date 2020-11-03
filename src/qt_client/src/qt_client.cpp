#include <qt_client/qt_client.h>

#include <QApplication>

#include <qxtglobalshortcut/qxtglobalshortcut.h>

#include <toolbox/debug/debug.h>
#include <qt_client/mainwindow.h>
#include <qt_client/common/session_data.h>


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
  LOG_INFO("sequence to show main windows will be: " << show_window_sequence);
  result = QKeySequence(QString::fromStdString(show_window_sequence));

  return !result.isEmpty();
}


int
QTClient::execute(int argc,
                  char *argv[],
                  service::ServiceAPI::Ptr service_api,
                  const toolbox::Config& config,
                  bool start_hidden)
{
  QApplication app( argc, argv );
  SessionData session_data;
  MainWindow w(nullptr, service_api, &session_data);

  if (!start_hidden) {
    w.showNow();
  }

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
