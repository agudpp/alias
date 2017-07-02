# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'tag_handler.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_TagHandler(object):
    def setupUi(self, TagHandler):
        TagHandler.setObjectName("TagHandler")
        TagHandler.resize(400, 300)
        self.verticalLayout = QtWidgets.QVBoxLayout(TagHandler)
        self.verticalLayout.setObjectName("verticalLayout")
        self.optLayout = QtWidgets.QHBoxLayout()
        self.optLayout.setObjectName("optLayout")
        self.verticalLayout.addLayout(self.optLayout)
        self.lineEdit = QtWidgets.QLineEdit(TagHandler)
        self.lineEdit.setMinimumSize(QtCore.QSize(0, 70))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.lineEdit.setFont(font)
        self.lineEdit.setObjectName("lineEdit")
        self.verticalLayout.addWidget(self.lineEdit)
        self.selTagsLayout = QtWidgets.QHBoxLayout()
        self.selTagsLayout.setObjectName("selTagsLayout")
        self.verticalLayout.addLayout(self.selTagsLayout)

        self.retranslateUi(TagHandler)
        QtCore.QMetaObject.connectSlotsByName(TagHandler)

    def retranslateUi(self, TagHandler):
        _translate = QtCore.QCoreApplication.translate
        TagHandler.setWindowTitle(_translate("TagHandler", "Form"))
        self.lineEdit.setPlaceholderText(_translate("TagHandler", "tags"))

