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
    if "name" in attr:
        name = attr["name"].decode("ascii", "ignore")
        r = re.compile("PLAYSTATION\(R\)3 Controller \((.*)\)")
        m = r.match(name)
        if not m:
            return None
        mac = m.group(1)
        return mac


def udev_usb(action, device, config, state):
    if action == "add":
        attr = device.attributes
        if "product" in attr and attr["product"] == b"PLAYSTATION(R)3 Controller":
            if config.autopair:
                pair(config)
            state["ps3"] = device
    elif action == "remove":
        if device == state["ps3"]:
            state["ps3"] = None
            gone(state)


def udev_input(action, device, config, state):
    if action == "add":
        attr = device.attributes
        if "uevent" in attr:
            uevent = device.attributes["uevent"]
            r = re.compile("DEVNAME=(input/js\d+)")
            lines = uevent.splitlines()
            for line in lines:
                m = r.match(line.decode("ascii", "ignore"))
                if m:
                    filename = "/dev/{0}".format(m.group(1))
                    state["joystick"] = device
                    state["mac"] = get_mac(device)
                    Joystick.joystick(config, state, filename)
    elif action == "remove":
        if device == state["joystick"]:
            state["joystick"] = None
            gone(state)


def run():
    config = Config.Config("config.json")

    context = pyudev.Context()
    monitor = pyudev.Monitor.from_netlink(context)

    state = {}
    init_state(state)

    for action, device in monitor:
        subsystem = device.subsystem
        if subsystem == "usb":
            udev_usb(action, device, config, state)
        elif subsystem == "input":
            udev_input(action, device, config, state)

run()
