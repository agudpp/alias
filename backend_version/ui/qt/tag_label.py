from PyQt5.QtWidgets import QLabel
from PyQt5 import QtGui
from PyQt5.QtCore import Qt

class TagLabel(QLabel):
    def __init__(self):
        super(QLabel, self).__init__()
        self.setMinimumHeight(40)
        self.setFont(QtGui.QFont('Ubuntu', 12))
        self.setAlignment(Qt.AlignCenter)
        self.setAutoFillBackground(True)

        self._palette = self.palette()
        self._isHighlighted = False

    def isHighlighted(self):
        return self._isHighlighted

    def highlight(self, shouldHighlight):
        if shouldHighlight == self._isHighlighted:
            return
        if shouldHighlight:
            # self._palette.setColor(self.foregroundRole(), Qt.yellow)
            self._palette.setColor(self.backgroundRole(), Qt.yellow)
        else:
            self._palette.setColor(self.backgroundRole(), Qt.transparent)
            # self._palette.setColor(self.foregroundRole(), Qt.transparent)
        self.setPalette(self._palette)
        self._isHighlighted = shouldHighlight
        