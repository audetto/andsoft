import pyudev
import re

from asi import Config
from asi import PS3Blue
from asi import Joystick


def init_state(state):
    state["ps3"] = None
    state["joystick"] = None
    state["mac"] = None


def gone(state):
    init_state(state)
    print("PS3 detached")


def get_mac(device):
    p = device.parent
    if not p:
        return None
    attr = p.attributes
    if "name" in attr and attr["name"] == b"PLAYSTATION(R)3 Controller":
        if "uniq" in attr:
            mac = attr["uniq"]
            return mac


def udev_usb(action, device, config, state):
    if action == "add":
        attr = device.attributes
        if "product" in attr and attr["product"] == b"PLAYSTATION(R)3 Controller":
            if config.autopair:
                PS3Blue.pair(config)
            state["ps3"] = device
    elif action == "remove":
        if device == state["ps3"]:
            state["ps3"] = None
            gone(state)


def add_input(device, config, state):
    attr = device.attributes
    if "uevent" in attr:
        uevent = device.attributes["uevent"]
        r = re.compile(r"DEVNAME=(input/js\d+)")
        lines = uevent.splitlines()
        for line in lines:
            m = r.match(line.decode("ascii", "ignore"))
            if m:
                filename = "/dev/{0}".format(m.group(1))
                state["joystick"] = device
                state["mac"] = get_mac(device)
                Joystick.joystick(config, state, filename)
                return True
    return False # we did not process a Joystick


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
    for device in pyudev.Enumerator(context).match_subsystem("input"):
        if add_input(device, config, state):
            break # should we break? it only matters if we have 2 connected

    # then we wait for a joystick to be connected
    monitor = pyudev.Monitor.from_netlink(context)
    for action, device in monitor:
        subsystem = device.subsystem
        if subsystem == "usb":
            udev_usb(action, device, config, state)
        elif subsystem == "input":
            udev_input(action, device, config, state)

run()
