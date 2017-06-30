import sys
from PyQt5.QtCore import Qt
import PyQt5.QtCore
from PyQt5 import QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QDialog, QLineEdit
from ui_mainwindow import Ui_MainWindow
from tag_label import TagLabel
from sel_mngr import SelectionManager
from be_conn import BEConnector




class MyMainWindow(QMainWindow):
    def __init__(self):
        super(MyMainWindow, self).__init__()

        # backend connector
        self.beConn = BEConnector()

        self.selTags = []

        # Set up the user interface from Designer.
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # tag ui managers
        self.selTagsMngr = SelectionManager(self.ui.selTagsLayout)
        self.optTagsSelMngr = SelectionManager(self.ui.optLayout)
        
        # Make some local modifications.
        # self.ui.colorDepthCombo.addItem("2 colors (1 bit per pixel)")

        # Connect up the buttons.
        self.ui.lineEdit.textChanged.connect(self.lineEditChanged)
        self.lineEdit = self.ui.lineEdit
        self.lineEdit.installEventFilter(self)
        # self.ui.cancelButton.clicked.connect(self.reject)
        self.ui.resultList.setFocusPolicy(Qt.NoFocus)
        self.ui.resultList.addItem("test item 1")
        self.ui.resultList.addItem("test item 2")
        self.ui.resultList.addItem("test item 3")



    def addSelectedTag(self, t):
        if t in self.selTags:
            return
        self.selTags.append(t)
        self.beConn.setTags(self.selTags)
        self.selTagsMngr.setTags(self.selTags)


    def lineEditChanged(self, theStr):
        print("sending: ", theStr)
        resp = self.beConn.getResults(theStr)
        # self.showCurrTags(resp["matched_tags"])
        self.optTagsSelMngr.setTags(resp["expanded_tags"])
        # we need here to get the current tags and the expansion


    def showExpandedResults(self):
        selTag = self.optTagsSelMngr.current()
        if selTag == None:
            return
        beResults = self.beConn.getLastResults()
        if not beResults or not 'expanded_results' in beResults:
            return
        expResults = beResults['expanded_results']
        self.ui.resultList.clear()
        if not selTag in expResults:
            return
        for er in expResults[selTag]:
            if not 'content' in er:
                continue
            self.ui.resultList.addItem(er['content'])



    def eventFilter(self, source, event):
        if source == self.lineEdit:
            evtType = type(event)
            if evtType == QtGui.QKeyEvent and event.type() == PyQt5.QtCore.QEvent.KeyRelease:
                # now we need to consider 2 cases, shift and not shift
                modifiers = QApplication.keyboardModifiers()
                isControl = modifiers == Qt.ControlModifier
                key = event.key()
                if key == Qt.Key_Tab:
                    if isControl:
                        self.optTagsSelMngr.selPrev()
                    else:
                        self.optTagsSelMngr.selNext()
                    # here now we need to show the expanded contet options for this
                    self.showExpandedResults()
                    event.accept()
                    return True
        return self.lineEdit.eventFilter(source, event)


app = QApplication(sys.argv)
window = MyMainWindow()
# ui = Ui_MainWindow()
# ui.setupUi(window)

window.show()
sys.exit(app.exec_())
