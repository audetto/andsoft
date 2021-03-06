import os
import datetime
import json

from asi import Utils
from asi import Config
from asi import Base

programsUrl = "http://api.tf1.fr/tf1-programs/iphone/limit/100/"
newsUrl = "http://api.tf1.fr/tf1-homepage-news/iphone/"
homepageUrl = "http://api.tf1.fr/tf1-homepage/iphone/"

def getDataUrl(progId, item):
    url = "http://api.tf1.fr/tf1-vods/iphone//integral/{0}/program_id/{1}".format(item, progId)
    return url


def getWatLink(watId):
    url = "http://www.wat.tv/get/iphone/{0}.m3u8?bwmin=100000&bwmax=490000".format(watId)
    return url


def parseItem(grabber, prog, name, db):
    pid      = str(prog["id"])
    desc     = prog["longTitle"]
    pubDate  = prog["publicationDate"]
    duration = prog["duration"]
    name     = name + " - " + prog["shortTitle"]
    wat      = prog["watId"]
    category = prog["videoCategory"]

    length = datetime.timedelta(seconds = duration)
    date = datetime.datetime.strptime(pubDate, "%Y-%m-%d %H:%M:%S")

    # ignore the countless "extract", "bonus", "short" which last just a few minutes
    if category == "fullvideo":
        pid = Utils.getNewPID(db, pid)
        p = Program(grabber, date, length, pid, name, desc, wat, category)
        Utils.addToDB(db, p)


def processGroup(grabber, f, name, db):
    o = json.load(f)

    for prog in o:
        parseItem(grabber, prog, name, db)


def processNews(grabber, f, folder, progress, downType, db):
    o = json.load(f)

    for prog in o:
        name = prog["programName"]
        groupId = prog["programId"]

        downloadGroup(grabber, name, groupId, folder, progress, downType, db)

        # this group contains the info of the most recent Item
        # we add an other item with the group name
        # some info will still be missing

        title = prog["title"]
        wat = prog["linkAttributes"]["watId"]
        category = prog["linkAttributes"]["videoCategory"]

        pid = Utils.getNewPID(db, groupId)
        p = Program(grabber, datetime.datetime.now(), None, pid, name, title, wat, category)
        Utils.addToDB(db, p)


def processPrograms(grabber, f, folder, progress, downType, db):
    o = json.load(f)

    for prog in o:
        name = prog["shortTitle"]
        groupId = prog["id"]

        # here, we do not know the most recent item
        # we simply have to go through them all

        downloadGroup(grabber, name, groupId, folder, progress, downType, db)


def downloadGroup(grabber, name, groupId, folder, progress, downType, db):

    # we set it to True as this is a group
    # and subject to continuous changes
    checkTimestamp = True

    # .0
    url_0 = getDataUrl(groupId, 0)
    localName_0 = os.path.join(folder, str(groupId) + ".0.json")
    f_0 = Utils.download(grabber, progress, url_0, localName_0, downType, "utf-8", checkTimestamp)

    if f_0:
        processGroup(grabber, f_0, name, db)

    # .1
    url_1 = getDataUrl(groupId, 1)
    localName_1 = os.path.join(folder, str(groupId) + ".1.json")
    f_1 = Utils.download(grabber, progress, url_1, localName_1, downType, "utf-8", checkTimestamp)

    if f_1:
        processGroup(grabber, f_1, name, db)


def download(db, grabber, downType):
    progress = Utils.getProgress()

    folder = Config.tf1Folder

    localName = os.path.join(folder, "news.json")
    f = Utils.download(grabber, progress, newsUrl, localName, downType, "utf-8", True)

    processNews(grabber, f, folder, progress, downType, db)

    localName = os.path.join(folder, "programs.json")
    f = Utils.download(grabber, progress, programsUrl, localName, downType, "utf-8", True)

    processPrograms(grabber, f, folder, progress, downType, db)


class Program(Base.Base):
    def __init__(self, grabber, datetime, length, pid, title, desc, wat, category):
        super(Program, self).__init__()

        self.pid = pid
        self.title = title
        self.description = desc
        self.channel = "tf1"
        self.wat = wat
        self.datetime = datetime
        self.category = category

        self.length = length
        self.grabber = grabber
        self.ts = getWatLink(self.wat)

        name = Utils.makeFilename(self.title)
        self.filename = self.pid + "-" + name


    def display(self, width):
        super(Program, self).display(width)

        print()
        print("Category:", self.category)
