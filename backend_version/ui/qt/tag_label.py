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
        self.baseStyle = 'border:2px solid grey; \nborder-radius: 5px;'
        self.setStyleSheet(self.baseStyle);

        self._isHighlighted = False

    def isHighlighted(self):
        return self._isHighlighted

    def highlight(self, shouldHighlight):
        if shouldHighlight == self._isHighlighted:
            return
        if shouldHighlight:
            style = self.baseStyle + 'background-color: black; color: white'
        else:
            style = self.baseStyle + 'color: black'
            
        self.setStyleSheet(style)
        self._isHighlighted = shouldHighlight
        