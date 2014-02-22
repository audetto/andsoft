import dbus


def detach(mac):
    if not mac:
        print("Cannot detach: missing MAC address")
        return

    bus = dbus.SystemBus()
    bluez_bus = bus.get_object("org.bluez", "/")
    objects = bluez_bus.GetManagedObjects(dbus_interface = "org.freedesktop.DBus.ObjectManager")

    for k in objects:
        ob = objects[k]
        if "org.bluez.Device1" in ob:
            device = ob["org.bluez.Device1"]
            if device["Name"] == 'PLAYSTATION(R)3 Controller' and device["Address"].upper() == mac.upper():
                device_bus = bus.get_object("org.bluez", k)
                idev = dbus.Interface(device_bus, dbus_interface = "org.bluez.Device1")
                print("Detaching joystick ({0})".format(mac))
                idev.Disconnect()
