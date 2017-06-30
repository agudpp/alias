import requests


class BEConnector:
    def __init__(self):
        self.cachedResult = None
        self.tags = []
        self.isDirty = False
        self.lastQ = ''

    def setTags(self, tags):
        self.isDirty = self.isDirty or (tags != self.tags)
        self.tags = tags

    def getResults(self, q):
        if self.isDirty or self.lastQ != q:
            self.cachedResult = self._sendReq(self._builldQuery(q))
            self.lastQ = q
            self.isDirty = False
        return self.cachedResult

    def getLastResults(self):
        return self.cachedResult

    def _sendReq(self, d):
        print ("sending request: ", d)
        r = requests.post('http://localhost:1234', json = d)
        result = r.json()
        print("req result: ", result)
        return result


    def _builldQuery(self, q):
        return {
            "endpoint": "search",
            "query": q,
            "tags": self.tags
        }
