# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'input_window.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_InputWindow(object):
    def setupUi(self, InputWindow):
        InputWindow.setObjectName("InputWindow")
        InputWindow.resize(400, 300)
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(InputWindow)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.contentText = QtWidgets.QPlainTextEdit(InputWindow)
        self.contentText.setObjectName("contentText")
        self.verticalLayout.addWidget(self.contentText)
        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.retranslateUi(InputWindow)
        QtCore.QMetaObject.connectSlotsByName(InputWindow)

    def retranslateUi(self, InputWindow):
        _translate = QtCore.QCoreApplication.translate
        InputWindow.setWindowTitle(_translate("InputWindow", "InputWindow"))

