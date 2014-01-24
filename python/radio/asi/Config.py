import json

class Config(object):

    def __init__(self, filename):
        f = open(filename)
        o = json.load(f)

        self.sixpair    = o["sixpair"]
        self.autopair   = o["autopair"]
        self.detach     = o["detach"]

        self.remote = {}

        remote = o["remote"]
        for k in remote:
            self.remote[int(k)] = remote[k]
