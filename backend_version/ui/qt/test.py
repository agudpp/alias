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
        self.currSelMngr = self.optTagsSelMngr

        # Make some local modifications.
        # self.ui.colorDepthCombo.addItem("2 colors (1 bit per pixel)")

        # Connect up the buttons.
        self.ui.lineEdit.textChanged.connect(self.lineEditChanged)
        self.ui.lineEdit.returnPressed.connect(self.lineEditReturnPressed)
        self.lineEdit = self.ui.lineEdit
        self.lineEdit.installEventFilter(self)
        # self.ui.cancelButton.clicked.connect(self.reject)
        self.ui.resultList.setFocusPolicy(Qt.NoFocus)
        self.ui.resultList.addItem("test item 1")
        self.ui.resultList.addItem("test item 2")
        self.ui.resultList.addItem("test item 3")


    def performCheck(self):
        theStr = self.lineEdit.text()
        resp = self.beConn.getResults(theStr)
        # self.showCurrTags(resp["matched_tags"])
        self.optTagsSelMngr.setTags(resp["expanded_tags"])
        self.currSelMngr = self.optTagsSelMngr

    def addSelectedTag(self, t):
        if t in self.selTags:
            return
        self.selTags.append(t)
        self.beConn.setTags(self.selTags)
        self.selTagsMngr.setTags(self.selTags)

    def removeSelectedTag(self, t):
        if not t in self.selTags:
            return
        self.selTags.remove(t)
        self.beConn.setTags(self.selTags)
        self.selTagsMngr.removeTag(t)


    def lineEditChanged(self, theStr):
        self.selTagsMngr.unselCurrent()
        self.performCheck()
        
        # we need here to get the current tags and the expansion


    def showExpandedResults(self):
        selTag = self.optTagsSelMngr.current()
        if selTag == None:
            selTag = ''
        beResults = self.beConn.getLastResults()
        if not beResults or not 'expanded_results' in beResults:
            return
        expResults = beResults['expanded_results']
        self.ui.resultList.clear()
        self.ui.resultList.setCurrentRow(-1)
        if not selTag in expResults:
            return
        for er in expResults[selTag]:
            if not 'content' in er:
                continue
            self.ui.resultList.addItem(er['content'])

    def lineEditReturnPressed(self):
        print('enter pressed')
        # check if we have selected any of the tags so we continue
        currTag = self.optTagsSelMngr.current()
        if currTag: 
            print('selecting tag: ', currTag)
            self.addSelectedTag(currTag)
            self.lineEdit.setText('')
            return
        # check if we want to select one of the items
        currentRow = self.ui.resultList.currentRow()
        if currentRow >= 0:
            # copy the item
            print('currentRow', currentRow)
            currItem = self.ui.resultList.currentItem()
            if currItem:
                print('current item copied: ', currItem.text())
            return

        # we want to select a item?
        if self.ui.resultList.count() > 0:
            self.ui.resultList.setCurrentRow(0)

    def moveResults(self, next):
        nindex = self.ui.resultList.currentRow()
        total = self.ui.resultList.count()
        if next:
            nindex = (nindex + 1) % total
        else:
            nindex = total-1 if nindex <= 0 else nindex-1
        self.ui.resultList.setCurrentRow(nindex)

    # Control functions for the current selection manager
    def changeSelMngr(self, isNext):
        if isNext:
            self.currSelMngr.selNext()
        else:
            self.currSelMngr.selPrev()


    def eventFilter(self, source, event):
        if source == self.lineEdit:
            evtType = type(event)
            if evtType == QtGui.QKeyEvent and event.type() == PyQt5.QtCore.QEvent.KeyRelease:
                # now we need to consider 2 cases, shift and not shift
                modifiers = QApplication.keyboardModifiers()
                isShift = modifiers == Qt.ControlModifier
                key = event.key()
                if key == Qt.Key_Tab:
                    self.changeSelMngr(not isShift)
                    # here now we need to show the expanded contet options for this
                    self.showExpandedResults()
                    event.accept()
                    return True
                elif key == Qt.Key_Backspace:
                    if len(self.lineEdit.text()) == 0:
                        self.currSelMngr = self.selTagsMngr
                        currTag = self.selTagsMngr.current()
                        if currTag:
                            # we remove this one and select last one again
                            self.removeSelectedTag(currTag)
                        self.selTagsMngr.selLast()
                    event.accept()
                    return True
                elif key == Qt.Key_Up or key == Qt.Key_Down:
                    self.moveResults(key == Qt.Key_Down)
                    event.accept()
                    return True


        return self.lineEdit.eventFilter(source, event)


app = QApplication(sys.argv)
window = MyMainWindow()
# ui = Ui_MainWindow()
# ui.setupUi(window)

window.show()
sys.exit(app.exec_())
