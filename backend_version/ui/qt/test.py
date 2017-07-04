import sys
from PyQt5.QtCore import Qt
import PyQt5.QtCore
from PyQt5 import QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QDialog, QLineEdit
from ui_mainwindow import Ui_MainWindow
from tag_handler import TagHandler
from be_conn import BEConnector


class MyMainWindow(QMainWindow):
    def __init__(self):
        super(MyMainWindow, self).__init__()

        # backend connector
        self.beConn = BEConnector()
        # Set up the user interface from Designer.
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.resultList.setFocusPolicy(Qt.NoFocus)
        self.setWindowFlags(Qt.Window | Qt.FramelessWindowHint)

        # add the tag handler
        self.tagHandler = TagHandler()
        self.ui.verticalLayout.addWidget(self.tagHandler)

        # connect signals
        # inputTextChanged = pyqtSignal(str)
        self.tagHandler.inputTextChanged.connect(self.inputTextChanged)

        # inputKeyPressed = pyqtSignal(int)
        self.tagHandler.inputKeyPressed.connect(self.inputKeyPressed)
        
        # newTagHighlighted = pyqtSignal(bool, str)
        self.tagHandler.newTagHighlighted.connect(self.newTagHighlighted)

        # tagRemoved = pyqtSignal(str)
        self.tagHandler.tagRemoved.connect(self.tagRemoved)

        # optionalTagSelected = pyqtSignal(str)
        self.tagHandler.optionalTagSelected.connect(self.optionalTagSelected)



    # Here we will define all slots that will define the logic depending on
    # the tag manager
    def inputTextChanged(self, theStr):
        # query the backend
        if not self._queryBackend(theStr):
            print('Error hitting the backend?')
            return
        self._configureFromLastResults()

    def inputKeyPressed(self, key):
        # we need check what is happening
        if key == Qt.Key_Escape:
            self.close()
        elif key == Qt.Key_Up or key == Qt.Key_Down:
            self._moveResults(key == Qt.Key_Down)
        elif key == Qt.Key_Return:
            modifiers = QApplication.keyboardModifiers()
            isCtrl = modifiers == Qt.ControlModifier
            if isCtrl:
                # we will open the input window
                self._showInputWindow()
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
        else:
            print('Key not handled?')

    def newTagHighlighted(self, isSelected, t):
        if isSelected:
            # nothing to do for now?
            return
        # else we need to update the results on the result list
        self._showExpandedResults(t)

    def tagRemoved(self, t):
        # we need to update the data from the BE
        self.beConn.setTags(self.tagHandler.getSelectedTags())
        if not self._queryBackend(self.tagHandler.currentText()):
            return
        self._configureFromLastResults()

    def optionalTagSelected(self, t):
        self.beConn.setTags(self.tagHandler.getSelectedTags())
        if not self._queryBackend(self.tagHandler.currentText()):
            return
        self._configureFromLastResults()


    def _moveResults(self, next):
        nindex = self.ui.resultList.currentRow()
        total = self.ui.resultList.count()
        if total == 0:
            return
        if next:
            nindex = (nindex + 1) % total
        else:
            nindex = total-1 if nindex <= 0 else nindex-1
        self.ui.resultList.setCurrentRow(nindex)

    def _queryBackend(self, q):
        return self.beConn.getResults(q)

    def _configureFromLastResults(self):
        results = self.beConn.getLastResults()
        if not results or not 'expanded_tags' in results:
            print('Invalid data from the backend when configuring tag handler')
            return False
        self.tagHandler.setPossibleTags(results["expanded_tags"])
        return True

    def _showExpandedResults(self, t):
        beResults = self.beConn.getLastResults()
        if not beResults or not 'expanded_results' in beResults:
            return
        expResults = beResults['expanded_results']
        self.ui.resultList.clear()
        self.ui.resultList.setCurrentRow(-1)
        if not t in expResults:
            return
        for er in expResults[t]:
            if not 'content' in er:
                continue
            self.ui.resultList.addItem(er['content'])

    def _showInputWindow(self):
        return


def center(win):
    frameGm = win.frameGeometry()
    screen = QApplication.desktop().screenNumber(QApplication.desktop().cursor().pos())
    centerPoint = QApplication.desktop().screenGeometry(screen).center()
    frameGm.moveCenter(centerPoint)
    win.move(frameGm.topLeft())


app = QApplication(sys.argv)
window = MyMainWindow()
# ui = Ui_MainWindow()
# ui.setupUi(window)

window.show()
center(window)
sys.exit(app.exec_())
