import dbus
import io


class PS3Blue:
    def __init__(self, mac, hidraw):
        self.mac = mac
        self.hidraw = hidraw

        b = bytearray(36)
        b[0] = 0x01
        b[11] = b[16] = b[21] = b[26] = 0xff # the total time the led is active (0xff means forever)
        b[12] = b[17] = b[22] = b[27] = 0x27 # duty_length: cycle time in deciseconds (0 - "blink very fast")
        b[13] = b[18] = b[23] = b[28] = 0x10
        b[14] = b[19] = b[24] = b[29] = 0x00 # % of duty_length led is off (0xff means 100%)
        b[15] = b[20] = b[25] = b[30] = 0x32 # % of duty_length led is on (0xff means 100%)

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

        self.fd = None

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

            # noflash
            self.leds[14] = self.leds[19] = self.leds[24] = self.leds[29] = 0x00

            # there are only 4 leds here: 2^4 = 16
            # 0 - 15
            self.leds[10] = min(max(number, 0), 15) * 2

            fd = self.connect()
            if fd:
                written = fd.write(self.leds)
                # written seems to be 0????? but it works
                self.number = number


    def set_flash(self):
        number = -2 # by convention
        if self.number != number:

            # all flashing
            self.leds[14] = self.leds[19] = self.leds[24] = self.leds[29] = 0x10
            # all on
            self.leds[10] = 15 * 2

            # this should add a rumble but it does not work
            #self.leds[2] = 0xfe
            #self.leds[3] = 0x00
            #self.leds[4] = 0xfe
            #self.leds[5] = 0xff

            fd = self.connect()
            if fd:
                written = fd.write(self.leds)
                # written seems to be 0????? but it works
                self.number = number
