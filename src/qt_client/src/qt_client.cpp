#include <qt_client/qt_client.h>

#include <QApplication>
#include <QTextStream>

#include <qxtglobalshortcut/qxtglobalshortcut.h>

#include <qt_client/mainwindow.h>


namespace qt_client {


int
QTClient::execute(int argc, char *argv[], service::ServiceAPI::Ptr service_api)
{
  QApplication app( argc, argv );
  MainWindow w(nullptr, service_api);
  w.showNow();

  QTextStream out(stdout);
  QTextStream err(stderr);

  const QKeySequence shortcut(Qt::ALT + Qt::SHIFT + Qt::Key_Return);
  const QxtGlobalShortcut globalShortcut(shortcut);

  if ( !globalShortcut.isValid() ) {
      err << QString("Error: Failed to set shortcut %1")
          .arg(shortcut.toString()) << endl;
      return 1;
  }

  out << QString("Press shortcut %1 (or CTRL+C to exit)").arg(shortcut.toString()) << endl;

  QObject::connect(
              &globalShortcut, &QxtGlobalShortcut::activated, &globalShortcut,
              [&]{
                  out << QLatin1String("Shortcut pressed!") << endl;
                  w.showNow();
//                  QApplication::quit();

              });


  return app.exec();
}


} // namespace qt_client
