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

    GridView {
        id: gridView1
        x: 72
        y: 39
        width: 474
        height: 358
        cellWidth: 70
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        cellHeight: 70
        delegate: Item {
            x: 5
            height: 50
            Column {
                spacing: 5
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    x: 5
                    text: name
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: true
                }
            }
        }

        Text {
            id: text1
            x: 204
            y: 199
            objectName: "text1"
            text: qsTr("Hello World")
            anchors.centerIn: parent
        }
    }
}
