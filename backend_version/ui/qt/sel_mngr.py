from tag_label import TagLabel

class SelectionManager:
    def __init__(self, layout):
        self.freeTags =  list()
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
        curr = self._getCurrent()
        if curr:
            curr.highlight(False)
        if self.selTagIdx == 0:
            self.selTagIdx = len(self.currentTags) - 1
        else:
            self.selTagIdx -= 1
        self.currentTags[self.selTagIdx].highlight(True)

    def current(self):
        curr = self._getCurrent()
        if curr:
            return curr.text()
        return None

    def clear(self):
        for elem in self.currentTags:
            self.layout.removeWidget(elem)
            self.freeTags.append(elem)
        self.currentTags = []
        self.selTagIdx = -1

    def setTags(self, strTags):
        for t in strTags:
            curr = None
            if len(self.freeTags) > 0:
                curr = self.freeTags.pop()
            else:
                curr = TagLabel()
            curr.setText(t)
            self.layout.addWidget(curr)
            self.currentTags.append(curr)

    def _getCurrent(self):
        if self.selTagIdx < 0 or self.selTagIdx >= len(self.currentTags):
            return None
        return self.currentTags[self.selTagIdx]


