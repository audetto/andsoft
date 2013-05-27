import os
import json
import datetime

from xml.etree import ElementTree

from asi import Utils
from asi import Base
from asi import Config

configUrl = "http://app.mediaset.it/app/videomediaset/iPhone/2.0.2/videomediaset_iphone_config.plist"

def parseConfig(root):
    dic = root.find("dict")

    result = {}

    process = False
    for n in dic.iter():
        if n.tag == "key" and n.text == "Configuration":
            process = True
        elif n.tag == "dict" and process:
            process = False
            for nn in n.iter():
                if nn.tag == "key":
                    name = nn.text
                elif nn.tag == "string":
                    result[name] = nn.text
            
    return result


def processVideos(grabber, f, conf, folder, progress, downType, db):
    o = json.load(f)

    videos = o["episodi_interi"]["video"]

    for v in videos:
        title = v["brand"]["value"] + " " + v["title"]
        desc = v["desc"]
        channel = v["channel"]
        date = datetime.datetime.strptime(v["date"], "%d/%m/%Y")
        length = v["duration"]
        num = v["id"]

        pid = Utils.getNewPID(db, num)
        p = Program(grabber, conf, date, length, pid, title, desc, num, channel)
        Utils.addToDB(db, p)


def processGroup(grabber, f, conf, folder, progress, downType, db):
    o = json.load(f)

    for a in o["programmi"]["programma"]:
        print(a["brand"]["value"], a["urlxml"])


def downloadGroup(grabber, url, conf, folder, progress, downType, db):
    name = Utils.httpFilename(url)
    localName = os.path.join(folder, name)

    f = Utils.download(grabber, progress, url, localName, downType, "utf-8", True)

    processVideos(grabber, f, conf, folder, progress, downType, db)


def download(db, grabber, downType):
    progress = Utils.getProgress()
    name = Utils.httpFilename(configUrl)

    folder = Config.mediasetFolder
    localName = os.path.join(folder, name)

    f = Utils.download(grabber, progress, configUrl, localName, downType, None, True)
    s = f.read().strip()
    root = ElementTree.fromstring(s)
    conf = parseConfig(root)

#    programs = conf["ProgramListRequestUrl"]
#    downloadGroup(grabber, programs, folder, progress, downType, db)

    episodes = conf["FullVideoRequestUrl"].replace("http://ww.", "http://www.")
    downloadGroup(grabber, episodes, conf, folder, progress, downType, db)


def getMediasetLink(conf, num):
    url = conf["CDNSelectorRequestUrl"]
    url = url.replace("%@", num)
    return url


class Program(Base.Base):
    def __init__(self, grabber, conf, datetime, length, pid, title, desc, num, channel):
        super(Program, self).__init__()

        self.pid = pid
        self.title = title
        self.description = desc
        self.channel = channel
        self.num = num
        self.datetime = datetime

        self.length = length
        self.grabber = grabber

        self.url = getMediasetLink(conf, num)

        name = Utils.makeFilename(self.title)
        self.filename = self.pid + "-" + name


    def display(self, width):
        print("=" * width)
        print("PID:", self.pid)
        print("Channel:", self.channel)
        print("Title:", self.title)
        print("Description:", self.description)
        print("Date:", Utils.strDate(self.datetime))
        print("Length:", self.length)
        print("Filename:", self.filename)
        print()
        print("url:", self.url)

        m3 = self.getTabletPlaylist()
        Utils.displayM3U8(self.m3)
