import struct

from asi import PS3Blue
from asi import MPDAlive


def execute(config, state, mpd, value):
    remote = config.mpd["remote"]
    if value in remote:
        cmd = remote[value]
        mpd.execute(cmd)
    elif value == config.detach:
        PS3Blue.detach(state["mac"])


def joystick(config, state, filename):
    print("New joystick ({0}) @ {1}".format(state["mac"], filename))

    event = struct.Struct('IhBB')

    mpd = MPDAlive.MPDAlive(config.mpd["host"], config.mpd["port"])

    js = open(filename, 'rb')
    try:
        while True:
            a = js.read(8)
            (time, value, type, number) = event.unpack(a)
            if type == 1 and value == 1:
                execute(config, state, mpd, str(number))
    except IOError as e:
        print(e)
    except Exception as e:
        print(e)

    print("Done joystick ({0}) @ {1}".format(state["mac"], filename))
