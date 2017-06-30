import sys
import requests
from PyQt5.QtCore import Qt
import PyQt5.QtCore
from PyQt5 import QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QDialog, QLineEdit
from ui_mainwindow import Ui_MainWindow
from tag_label import TagLabel
from sel_mngr import SelectionManager


def builldQuery(q):
    return {

        "endpoint": "search",
        "query": q,
        "tags": []
    }

def sendReq(d):
    print ("sending request: ", d)
    r = requests.post('http://localhost:1234', json = d)
    result = r.json()
    print("req result: ", result)
    return result


class MyMainWindow(QMainWindow):
    def __init__(self):
        super(MyMainWindow, self).__init__()

        # Set up the user interface from Designer.
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.optTagsSelMngr = SelectionManager(self.ui.optLayout)
        self.optTagsSelMngr.setTags(["t1", "tag_2", "whatever_else_3"])

        # Make some local modifications.
        # self.ui.colorDepthCombo.addItem("2 colors (1 bit per pixel)")

        # Connect up the buttons.
        self.ui.lineEdit.textChanged.connect(self.lineEditChanged)
        self.lineEdit = self.ui.lineEdit
        self.lineEdit.installEventFilter(self)
        # self.ui.cancelButton.clicked.connect(self.reject)

        self.installEventFilter(self)

    def lineEditChanged(self, str):
        #self.ui.pushButton.setText(str)
        print("changed to ", str)

    def eventFilter(self, source, event):
        if source == self.lineEdit:
            evtType = type(event)
            if evtType == QtGui.QKeyEvent:
                # now we need to consider 2 cases, shift and not shift
                modifiers = QApplication.keyboardModifiers()
                isControl = modifiers == Qt.ControlModifier
                key = event.key()
                if event.type() == PyQt5.QtCore.QEvent.KeyRelease:
                    isTab = event.key() == Qt.Key_Tab
                    print('keyevent ', isControl, isTab)
                    if event.key() == Qt.Key_Tab and (isControl or not event.isAutoRepeat()):
                        print('tab pressed')
                        if isControl:
                            self.optTagsSelMngr.selPrev()
                        else:
                            self.optTagsSelMngr.selNext()
                        event.accept()
                        return True
        return QLineEdit.eventFilter(self, source, event)


app = QApplication(sys.argv)
window = MyMainWindow()
# ui = Ui_MainWindow()
# ui.setupUi(window)

window.show()
sys.exit(app.exec_())
