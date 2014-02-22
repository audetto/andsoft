import json

class Config(object):

    def __init__(self, filename):
        f = open(filename)
        o = json.load(f)

        self.detach = o["detach"]

        self.mpd = {}

        mpd = o["mpd"]
        for k in mpd:
            self.mpd[k] = mpd[k]
