import requests
import json

class BEConnector:
    def __init__(self):
        self.cachedResult = None
        self.tags = []
        self.isDirty = False
        self.lastQ = ''
        self.lastTagsResult = None

    def setTags(self, tags):
        self.isDirty = True
        self.tags = tags

    def getResults(self, q):
        if self.isDirty or self.lastQ != q:
            self.cachedResult = self._sendReq(self._builldQuery(q))
            self.lastQ = q
            self.isDirty = False
        return self.cachedResult

    def performGetTags(self, prefix):
        q = {
            "endpoint": "get_tags",
            "prefix": prefix
        }
        self.lastTagsResult = self._sendReq(q)
        return self.lastTagsResult

    def getLastGetTagsResult(self):
        return self.lastTagsResult

    def getLastResults(self):
        return self.cachedResult

    def addTagElement(self, tags, elem):
        if tags == None or len(tags) == 0 or elem == None:
            return
        d = {
            "endpoint": "add_tag_elem",
            "tags": [],
            "element": {
                "text": elem
            }
        }
        for tagText in tags:
            d["tags"].append({ "text": tagText })
        self._sendReq(d)
        return True

    def _sendReq(self, d):
        print ("sending request: ", d)
        r = requests.post('http://localhost:1234', json = d)
        result = r.json()
        print("req result: ", json.dumps(result, indent=3))
        return result


    def _builldQuery(self, q):
        return {
            "endpoint": "search",
            "query": q,
            "tags": self.tags
        }
