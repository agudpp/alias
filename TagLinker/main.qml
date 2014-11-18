import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    objectName: "mainWindow"
    title: qsTr("Hello World")

    menuBar: MenuBar {
        objectName: "menubar"
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Text {
        id: text1
        objectName: "text1"
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}
