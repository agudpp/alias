#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include <core/types/basic_types.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //engine.load(QUrl(QStringLiteral("qrc:/core/elements/tag/Tag.qml")));

    QObject* root = engine.rootObjects().first();
    if (root != 0) {
        QObject* win = root->findChild<QObject*>("text1");
        if (win != 0) {
            qDebug() << "We found the main window!\n";
            win->setProperty("text", "Esto no es hello world");
        } else {
            qDebug() << "We DIDNT!!!! found the main window!\n";
        }
    }

    tl::ElementID id = 1;

    return app.exec();
}
