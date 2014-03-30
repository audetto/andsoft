import mpd

class MPDAlive(object):

    def __init__(self, host, port, ps3):
        self.host = host
        self.port = port
        self.client = mpd.MPDClient()
        self.oldvol = 0
        self.ps3 = ps3

        # do not synchronize now
        # /dev/hidra0 does not seem to be ready


    def connect(self):
        self.client.connect(self.host, self.port)


    def mpc(self):
        try:
            self.client.ping()
        except mpd.ConnectionError:
            self.connect()

        return self.client


    def get_volume(self):
        mpc = self.client
        s = mpc.status()
        vol = int(s["volume"])
        return vol


    def volume(self, change):
        mpc = self.client
        vol = self.get_volume()
        newvol = min(100, max(0, vol + change))
        mpc.setvol(newvol)


    def mute(self):
        mpc = self.client
        newvol = self.oldvol
        self.oldvol = self.get_volume()
        mpc.setvol(newvol)


    def get_song(self):
        mpc = self.client
        st = mpc.status()
        if "song" in st:
            song = int(st["song"])
            return song


    def execute(self, cmd):
        mpc = self.mpc()
        if cmd == "PLAY":
            mpc.play()
        elif cmd == "STOP":
            mpc.stop()
        elif cmd == "PREV":
            mpc.previous()
        elif cmd == "NEXT":
            mpc.next()
        elif cmd == "VOLUP":
            self.volume(2)
        elif cmd == "VOLDOWN":
            self.volume(-2)
        elif cmd == "RESET":
            mpc.play(0)
        elif cmd == "MUTE":
            self.mute()

        self.synchronize()


    def synchronize(self):
        song = self.get_song()
        if not song is None:
            self.ps3.set_leds(1 + song) # as they start from 0
