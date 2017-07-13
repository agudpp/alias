import sys
from PyQt5.QtCore import Qt
import PyQt5.QtCore
from PyQt5 import QtGui
from PyQt5.QtWidgets import QApplication, QWidget
from ui_input_window import Ui_InputWindow
from tag_handler import TagHandler
from be_conn import BEConnector


class InputWindow(QWidget):
    def __init__(self, beConn):
        super(InputWindow, self).__init__()

        # backend connector
        self.beConn = beConn
        # Set up the user interface from Designer.
        self.ui = Ui_InputWindow()
        self.ui.setupUi(self)

        self.setWindowFlags(Qt.Window | Qt.FramelessWindowHint)

        # add the tag handler
        self.tagHandler = TagHandler()
        self.tagHandler.setEnableCreateTags(True)
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
        self.tagHandler.newTagSelected.connect(self.newTagSelected)

        self.ui.contentText.installEventFilter(self)


    def resetConfiguration(self):
        self.ui.plainTextEdit.clear()
        self.ui.plainTextEdit.setFocus()

    def eventFilter(self, source, event):
        if source != self.ui.contentText:
            return self.ui.contentText.eventFilter(source, event)
        evtType = type(event)
        if evtType == QtGui.QKeyEvent and event.type() == PyQt5.QtCore.QEvent.KeyRelease:
            # now we need to consider 2 cases, shift and not shift
            modifiers = QApplication.keyboardModifiers()
            isShift = modifiers == Qt.ControlModifier
            key = event.key()
            if key == Qt.Key_Tab:
                # return the focus to tabs
                print('tab')
                event.accept()
                return True
            elif key == Qt.Key_Escape:
                self.close()
        return self.ui.contentText.eventFilter(source, event)

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
            # self._moveResults(key == Qt.Key_Down)
            print('TODO')
        elif key == Qt.Key_Return:
            # we should save there the information and close
            print('TODO')
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
        if not self._queryBackend(self.tagHandler.currentText()):
            return
        self._configureFromLastResults()

    def optionalTagSelected(self, t):
        self.beConn.setTags(self.tagHandler.getSelectedTags())
        if not self._queryBackend(self.tagHandler.currentText()):
            return
        self._configureFromLastResults()

    def newTagSelected(self, t):
        self.beConn.setTags(self.tagHandler.getSelectedTags())
        if not self._queryBackend(self.tagHandler.currentText()):
            return
        self._configureFromLastResults()

    def _queryBackend(self, q):
        return self.beConn.performGetTags(q)

    def _configureFromLastResults(self):
        results = self.beConn.getLastGetTagsResult()
        if not results or not 'tags' in results:
            print('Invalid data from the backend when configuring tag handler')
            return False
        self.tagHandler.setPossibleTags(results["tags"])
        return True

    def _saveCurrentDataOnBE(self):
        tags = self.tagHandler.getSelectedTags()
        if len(tags) == 0:
            print("Error saving the current data, no tags?")
            return False
        textToSave = self.ui.plainTextEdit.toPlainText()
        if len(textToSave) == 0:
            print("Error saving the current data, no text?")
            return False


# TODO: remove this

def center(win):
    frameGm = win.frameGeometry()
    screen = QApplication.desktop().screenNumber(QApplication.desktop().cursor().pos())
    centerPoint = QApplication.desktop().screenGeometry(screen).center()
    frameGm.moveCenter(centerPoint)
    win.move(frameGm.topLeft())


app = QApplication(sys.argv)
window = InputWindow(BEConnector())
# ui = Ui_MainWindow()
# ui.setupUi(window)

window.show()
center(window)
sys.exit(app.exec_())
