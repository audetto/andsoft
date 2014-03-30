import pyudev
import re

from asi import Config
from asi import Joystick


def init_state(state):
    state["ps3"] = None
    state["joystick"] = None
    state["mac"] = None
    state["hidraw"] = None


def gone(state):
    init_state(state)
    print("PS3 detached")


# logic copied from https://github.com/chrippa/ds4drv
# ds4drv/backends/hidraw.py
def add_input(hidraw_device, config, state):
    hid_device = hidraw_device.parent
    if hid_device.subsystem != "hid":
        return False

    name = hid_device.get("HID_NAME")
    if name != "PLAYSTATION(R)3 Controller":
        return False

    ps3 = None
    for child in hid_device.parent.children:
        js_device = child.get("DEVNAME", "")
        if js_device.startswith("/dev/input/js"):
            ps3 = child
            break
    else:
        return False

    print("Using {0}".format(ps3))
    filename = ps3.get("DEVNAME")
    state["joystick"] = ps3
    state["mac"] = hid_device.get("HID_UNIQ", "").upper()
    # temporary
    state["hidraw"] = hidraw_device.get("DEVNAME", "")
    Joystick.joystick(config, state, filename)
    return True


def udev_input(action, device, config, state):
    if action == "add":
        add_input(device, config, state)
    elif action == "remove":
        if device == state["joystick"]:
            state["joystick"] = None
            gone(state)


def run():
    config = Config.Config("config.json")

    context = pyudev.Context()

    state = {}
    init_state(state)

    # first we check if there is already a joystick connected
    for device in pyudev.Enumerator(context).match_subsystem("hidraw"):
        if add_input(device, config, state):
            break # should we break? it only matters if we have 2 connected

    # then we wait for a joystick to be connected
    monitor = pyudev.Monitor.from_netlink(context)
    monitor.filter_by("hidraw")
    for action, device in monitor:
        udev_input(action, device, config, state)

run()
