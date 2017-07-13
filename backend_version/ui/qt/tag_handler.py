from PyQt5.QtWidgets import QWidget, QApplication
import PyQt5.QtCore
from PyQt5 import QtGui
from PyQt5.QtCore import Qt, pyqtSignal
from ui_tag_handler import Ui_TagHandler
from tag_label import TagLabel
from sel_mngr import SelectionManager



# helper method that will get return the biggetst common substring for a given
# list of strings
# assuming that prefix is in all already
def getCommonSubstring(prefix, strList):
    if len(strList) == 0:
        return prefix
    cidx = len(prefix)
    shouldContinue = True
    while shouldContinue:
        chars = set()
        for s in strList:
            if cidx >= len(s):
                shouldContinue = False
                break
            chars.add(s[cidx])
        if len(chars) != 1 or not shouldContinue:
            shouldContinue = False
        else:
            # we have the same char
            prefix += chars.pop()
            cidx += 1
    return prefix



class TagHandler(QWidget):
    # define the signals here this class will provide

    # This signal will be called when the text of the label has changed but
    # not in all cases (autocomplete for example will not trigger this)
    inputTextChanged = pyqtSignal(str)

    # when we detect a keypress and we don't know what to do with it
    inputKeyPressed = pyqtSignal(int)

    # when a new tag is highlighted:
    # args: 1) isSelectedTag (False means is one of the optionals)
    #       2) the name of the tag
    newTagHighlighted = pyqtSignal(bool, str)

    # when a tag is removed
    tagRemoved = pyqtSignal(str)

    # when an optional tag is selected and moved to current tag
    optionalTagSelected = pyqtSignal(str)

    # whenever a new tag is created (from space for example)
    newTagSelected = pyqtSignal(str)



    def __init__(self):
        super(QWidget, self).__init__()
        # Set up the user interface from Designer.
        self.ui = Ui_TagHandler()
        self.ui.setupUi(self)

        self.possibleTags = []
        self.selTags = []
        # tag ui managers
        self.selTagsMngr = SelectionManager(self.ui.selTagsLayout)
        self.optTagsSelMngr = SelectionManager(self.ui.optLayout)
        self.currSelMngr = self.optTagsSelMngr

        # Connect up the buttons.
        self.ui.lineEdit.textChanged.connect(self.lineEditChanged)
        self.ui.lineEdit.returnPressed.connect(self.lineEditReturnPressed)
        self.lineEdit = self.ui.lineEdit
        self.lineEdit.installEventFilter(self)

        self.skipTextChanged = False
        self.enableNewTags = False


    # this will set the possibility of adding new tags using space
    def setEnableCreateTags(self, enable):
        self.enableNewTags = enable

    # This method should be called to set the current possible tags to be
    # selected
    def setPossibleTags(self, pt):
        self.possibleTags = pt
        self.optTagsSelMngr.setTags(pt)

    def getPossibleTags(self):
        return self.possibleTags

    def getSelectedTags(self):
        return self.selTags

    def clearTextInput(self):
        if len(self.lineEdit.text()) == 0:
            return
        self.skipTextChanged = True
        self.lineEdit.setText('')

    def currentText(self):
        return self.lineEdit.text()

    # helper internal methods to control events
    def _unselectCurrent(self, selMngr):
        if selMngr.hasSelection():
            selMngr.unselCurrent()

    def _highlightTag(self, selMngr, next):
        if next:
            selMngr.selNext()
        else:
            selMngr.selPrev()
        if selMngr.hasSelection():
            isSelectedTag = selMngr == self.selTagsMngr
            self.newTagHighlighted.emit(isSelectedTag, selMngr.current())

    def _selectNewTag(self, t):
        if len(t) <= 0:
            return
        self.selTags.append(t)
        self.selTagsMngr.setTags(self.selTags)
        self.newTagSelected.emit(t)

    def _selectCurrentTag(self):
        if not self.optTagsSelMngr.hasSelection():
            return
        curr = self.optTagsSelMngr.current()
        if curr in self.selTags:
            return
        self.selTags.append(curr)
        self.selTagsMngr.setTags(self.selTags)
        # emit event
        self.optionalTagSelected.emit(curr)

    def _removeCurrentTag(self):
        if not self.selTagsMngr.hasSelection():
            return
        curr = self.selTagsMngr.current()
        if not curr in self.selTags:
            return
        self.selTags.remove(curr)
        self.selTagsMngr.removeCurrent()
        # emit event
        self.tagRemoved.emit(curr)
        # we now highlight the last one
        self.selTagsMngr.selLast()
        if self.selTagsMngr.hasSelection():
            self.newTagHighlighted.emit(True, self.selTagsMngr.current())

    def lineEditChanged(self, theStr):
        self.selTagsMngr.unselCurrent()
        # check if it is an space
        if self.enableNewTags and len(theStr) > 0 and theStr[-1] == ' ':
            # it is an space so we need to actually select a new tag
            self._selectNewTag(theStr[0:-1])
            self.skipTextChanged = True
            self.lineEdit.setText('')
            return

        if self.skipTextChanged:
            self.skipTextChanged = False
            return
        self.inputTextChanged.emit(theStr)

    def lineEditReturnPressed(self):
        print('enter pressed')
        # check if we have selected any of the tags so we continue
        currTag = self.optTagsSelMngr.current()
        if currTag: 
            self._selectCurrentTag()
            return
        # else we emit the event
        self.inputKeyPressed.emit(Qt.Key_Return)

    def escapePressed(self):
        # if we have some sel active we deactivate it
        if self.selTagsMngr.hasSelection():
            self.selTagsMngr.unselCurrent()
            return
        if self.optTagsSelMngr.hasSelection():
            self.optTagsSelMngr.unselCurrent()
            return
        # else we emit event
        self.inputKeyPressed.emit(Qt.Key_Escape)
        

    def eventFilter(self, source, event):
        if source == self.lineEdit:
            evtType = type(event)
            if evtType == QtGui.QKeyEvent and event.type() == PyQt5.QtCore.QEvent.KeyRelease:
                # now we need to consider 2 cases, shift and not shift
                modifiers = QApplication.keyboardModifiers()
                isShift = modifiers == Qt.ControlModifier
                key = event.key()
                if key == Qt.Key_Tab:
                    if self.selTagsMngr.hasSelection():
                        self._highlightTag(self.selTagsMngr, not isShift)
                    if self.optTagsSelMngr.hasSelection():
                        self._highlightTag(self.optTagsSelMngr, not isShift)
                    else:
                        # check if we can do a better local autocomplete
                        currTxt = self.lineEdit.text()
                        commonPrefix = getCommonSubstring(currTxt, self.possibleTags)
                        if len(currTxt) != len(commonPrefix):
                            # we can do a better autocomplete
                            # avoid notifying up
                            self.skipTextChanged = True
                            self.lineEdit.setText(commonPrefix)
                        else:
                            self._highlightTag(self.optTagsSelMngr, not isShift)
                    event.accept()
                    return True
                elif key == Qt.Key_Backspace:
                    if len(self.lineEdit.text()) == 0:
                        if self.selTagsMngr.hasSelection():
                            # we remove this one and select last one again
                            self._removeCurrentTag()
                        else:
                            self._highlightTag(self.selTagsMngr, isShift)
                    event.accept()
                    return True
                elif key == Qt.Key_Escape:
                    self.escapePressed()
                    event.accept()
                    return True
                # here we will explicity say which keys we want to emit
                elif key == Qt.Key_Up or key == Qt.Key_Down:
                    self.inputKeyPressed.emit(key)
                    event.accept()
                    return True

        return self.lineEdit.eventFilter(source, event)


