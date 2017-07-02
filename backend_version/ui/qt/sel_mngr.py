from tag_label import TagLabel

class SelectionManager:
    def __init__(self, layout):
        # self.freeTags =  list()
        self.layout = layout
        self.currentTags = []
        self.selTagIdx = -1

    def selNext(self):
        if len(self.currentTags) == 0:
            return
        curr = self._getCurrent()
        if curr:
            curr.highlight(False)
        self.selTagIdx = (self.selTagIdx + 1) % len(self.currentTags)
        self.currentTags[self.selTagIdx].highlight(True)

    def selPrev(self):
        if len(self.currentTags) == 0:
            return
        self.unselCurrent()
        if self.selTagIdx < 0:
            self.selTagIdx = len(self.currentTags) - 1
        else:
            self.selTagIdx -= 1
        self.currentTags[self.selTagIdx].highlight(True)

    def selLast(self):
        if len(self.currentTags) == 0:
            return
        self.unselCurrent()
        self.selTagIdx = len(self.currentTags) - 1
        self.currentTags[self.selTagIdx].highlight(True)

    def unselCurrent(self):
        curr = self._getCurrent()
        if curr:
            curr.highlight(False)
        self.selTagIdx = -1

    def hasSelection(self):
        return self.selTagIdx >= 0

    def current(self):
        curr = self._getCurrent()
        if curr:
            return curr.text()
        return None

    def removeTag(self, t):
        idx = self._getIdx(t)
        if idx < 0:
            return
        curr = self.currentTags[idx]
        self.layout.removeWidget(curr)
        curr.deleteLater()
        self.currentTags.remove(curr)
        curr = None
        self.layout.update()
        self.selPrev()

    def removeCurrent(self):
        curr = self._getCurrent()
        if not curr:
            return
        self.layout.removeWidget(curr)
        curr.deleteLater()
        self.currentTags.remove(curr)
        curr = None
        self.layout.update()
        self.selPrev()

    def clear(self):
        for elem in self.currentTags:
            self.layout.removeWidget(elem)
            elem.deleteLater()
            elem = None
            # self.freeTags.append(elem)
        self.currentTags = []
        self.selTagIdx = -1
        self.layout.update()

    def setTags(self, strTags):
        self.clear()
        for t in strTags:
            curr = TagLabel()
            # curr = None
            # if len(self.freeTags) > 0:
            #     curr = self.freeTags.pop()
            # else:
            #     curr = TagLabel()
            curr.setText(t)
            curr.highlight(False)
            self.layout.addWidget(curr)
            self.currentTags.append(curr)
        self.layout.update()

    def count(self):
        return len(self.currentTags)

    def _getCurrent(self):
        if self.selTagIdx < 0 or self.selTagIdx >= len(self.currentTags):
            return None
        return self.currentTags[self.selTagIdx]

    def _getIdx(self, t):
        idx = 0
        for lt in self.currentTags:
            if lt.text() == t:
                return idx
            idx += 1
        return -1


