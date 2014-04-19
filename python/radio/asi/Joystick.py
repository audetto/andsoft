import struct

from asi import PS3Blue
from asi import MPDAlive


def execute(config, ps3, mpd, value):
    remote = config.mpd["remote"]
    if value in remote:
        cmd = remote[value]
        mpd.execute(cmd)
    elif value == config.detach:
        ps3.detach()


def joystick(config, state, filename):
    print("New joystick ({0}) @ {1}".format(state["mac"], filename))

    event = struct.Struct('IhBB')

    js = open(filename, 'rb')

    ps3 = PS3Blue.PS3Blue(state["mac"], state["hidraw"])
    mpd = MPDAlive.MPDAlive(config.mpd["host"], config.mpd["port"], config.mpd["timeout"], ps3)

    try:
        while True:
            a = js.read(8)
            (time, value, type, number) = event.unpack(a)
            if type == 1 and value == 1:
                execute(config, ps3, mpd, str(number))
    except IOError as e:
        print(e)
    except Exception as e:
        print(e)

    print("Done joystick ({0}) @ {1}".format(state["mac"], filename))
