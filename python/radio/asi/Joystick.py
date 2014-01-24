import struct

from asi import PS3Blue


def execute(config, state, value):
    remote = config.remote
    if value in remote:
        cmd = remote[value]
        print(cmd)
    elif value == config.detach:
        print("Detaching joystick ({0}).".format(state["mac"]))
        PS3Blue.detach(state["mac"])


def joystick(config, state, filename):
    print("New joystick ({0}) @ {1}.".format(state["mac"], filename))

    event = struct.Struct('IhBB')

    js = open(filename, 'rb')
    try:
        while True:
            a = js.read(8)
            (time, value, type, number) = event.unpack(a)
            if type == 1 and value == 1:
                execute(config, state, number)
    except IOError as e:
        print(e)
    except Exception as e:
        print(e)

    print("Done joystick ({0}) @ {1}.".format(state["mac"], filename))
