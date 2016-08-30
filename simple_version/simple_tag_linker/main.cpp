/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File main.cpp created by agustin on 29.8.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

#include "connector.h"


#define TAG_LINKER_JSON_FILE "/home/agustin/.tag_linker_defs.json"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // check if we are running already
    Connector connector;

    if (connector.checkConnection()) {
        // there is someone already
        qDebug() << "there already someone runninig";
        return -1;
    }
    if (!connector.startServer()) {
        qWarning() << "Couldn't create the server so we will have multiple instances :(";
        return -2;
    }

    // read the json
    QFile loadFile(QStringLiteral(TAG_LINKER_JSON_FILE));

   if (!loadFile.open(QIODevice::ReadOnly)) {
       qWarning("Couldn't open save file.");
       return -1;
   }

   QByteArray saveData = loadFile.readAll();
   QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

   // read the doc
   QJsonObject json = loadDoc.object();
   QJsonArray keyValues = json["data"].toArray();
   for (int levelIndex = 0; levelIndex < keyValues.size(); ++levelIndex) {
       QJsonArray kv = keyValues[levelIndex].toArray();
       QString keyStr = kv[0].toString();
       QString valueStr = kv[1].toString();
       // we will split here the keys by spaces and add multiple options
       QStringList keyOptions = keyStr.split(" ");
       for (const QString& key : keyOptions) {
           connector.addTagEntry(key, valueStr);
           qDebug() << "adding " << key << " -> " << valueStr;
       }
   }

   connector.createDialog();

    return a.exec();
}
