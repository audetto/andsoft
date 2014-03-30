import dbus
import io


class PS3Blue:
    def __init__(self, mac, hidraw):
        self.mac = mac
        self.hidraw = hidraw

        b = bytearray(36)
        b[0] = 0x01
        b[11] = b[16] = b[21] = b[26] = 0xff
        b[12] = b[17] = b[22] = b[27] = 0x27
        b[13] = b[18] = b[23] = b[28] = 0x10
        b[14] = b[19] = b[24] = b[29] = 0x00
        b[15] = b[20] = b[25] = b[30] = 0x32

        self.leds = b
        self.number = -1
        self.fd = None


    def connect(self):
        # if we open /dev/hidraw0 too early
        # everything dies
        if not self.fd and self.hidraw:
            self.fd = io.FileIO(self.hidraw, "wb")
        return self.fd


    def detach(self):
        if not self.mac:
            print("Cannot detach: missing MAC address")
            return

        bus = dbus.SystemBus()
        bluez_bus = bus.get_object("org.bluez", "/")
        objects = bluez_bus.GetManagedObjects(dbus_interface = "org.freedesktop.DBus.ObjectManager")

        for k in objects:
            ob = objects[k]
            if "org.bluez.Device1" in ob:
                device = ob["org.bluez.Device1"]
                if device["Address"].upper() == self.mac:
                    # k = "/org/bluez/hci0/dev_00_1B_FB_63_F2_64"
                    device_bus = bus.get_object("org.bluez", k)
                    idev = dbus.Interface(device_bus, dbus_interface = "org.bluez.Device1")
                    print("Detaching joystick ({0})".format(self.mac))
                    idev.Disconnect()


    def set_leds(self, number):
        if self.number != number:
            # there are only 4 leds here: 2^4 = 16
            # 0 - 15
            self.leds[10] = min(max(number, 0), 15) * 2

            fd = self.connect()
            if fd:
                written = fd.write(self.leds)
                # written seems to be 0????? but it works
                self.number = number
