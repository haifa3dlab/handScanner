import gi, re, serial, time
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class Arduino(object):

    @staticmethod
    def list_ports():
        result = []
        for i in range(256):
            try:
                pt = serial.Serial(i)
                pt.close()
                result.append("COM%d" % (i+1))
            except (OSError, serial.SerialException):
                pass
            # for linux ports:
            try:
                pname  = '/dev/ttyACM%d' % i
                pt = serial.Serial(pname)
                pt.close()
                result.append(pname)
            except (OSError, serial.SerialException):
                pass
            try:
                pname  = '/dev/ttyUSB%d' % i
                pt = serial.Serial(pname)
                pt.close()
                result.append(pname)
            except (OSError, serial.SerialException):
                pass
        return result

    def __init__(self, port = 5, baud_rate = 9600, timeout = 1):
        self.port = port
        self.ser = serial.Serial(self.port, baud_rate, timeout = timeout)

    def interact(self, msg):
        if self.ser:
            self.ser.write(msg)
            time.sleep(3)
            resp = []
            while self.ser.inWaiting():
                l = self.ser.readline().strip()
                if l:
                    resp.append(l)
            return "\n".join(resp).strip()
        else:
            return "Serial interface not available!"

class UI(object):

    def __init__(self):
        self.ac = None
        ports = Arduino.list_ports()
        self.builder = Gtk.Builder()
        self.builder.add_from_file("scanner_control.uirc")
        self.builder.connect_signals(self)
        self.top = self.builder.get_object("ui_top")
        # Rotation speed
        self.rot_speed = self.builder.get_object("rot_speed")
        # Elevation speed
        self.elev_speed = self.builder.get_object("elev_speed")
        # Elevation
        self.elevation = self.builder.get_object("elevation")
        # Rotation
        self.rotation = self.builder.get_object("rotation")
        self.rotation.set_range(0, 300)
        # Command
        self.command = self.builder.get_object("command")
        # Feedback
        self.feedback = self.builder.get_object("feedback")
        self.feedback_buffer = self.builder.get_object("feedback_buffer")
        self.fb_end = self.feedback_buffer.get_mark("insert")
        # Port list
        self.ports = self.builder.get_object("ports")
        if ports:
            for p in ports:
                self.ports.append_text(p)
        self.ports.append_text("Offline")
        self.ports.set_active(0)
        # Initial control values
        self.elevation_val = self.elevation.get_value()
        self.rotation_val = self.rotation.get_value()
        self.rot_speed_val = self.rot_speed.get_value()
        self.elev_speed_val = self.elev_speed.get_value()

    def onSetPort(self, *args, **kwds):
        com = self.ports.get_active_text()
        self.fb("onSetPort on %s" % com)
        m = re.match("COM([0-9]+)", com)
        if m:
            p = int(m.group(1)) - 1
            self.fb("Selecting Arduino on port %s" % com)
            self.ac = Arduino(port = p)
        else:
          if not m:
              m = re.match("/dev/ttyUSB([0-9]+)", com)
          if not m:
              m = re.match("/dev/ttyACM([0-9]+)", com)
          if m:
              p = m.group(0)
              self.fb("Selecting Arduino on port %s" % com)
              self.ac = Arduino(port = p)
          else:
              self.ac = None

    def show(self):
        self.top.show_all()

    def fb(self, t):
        self.feedback_buffer.insert_at_cursor(t + "\n")
        self.feedback.scroll_to_mark(self.fb_end, 0.1, False, 0, 0.5)

    def send(self, cmd):
        if self.ac:
            self.fb("Sending: %s" % cmd)
            resp = self.ac.interact("#%s# " % cmd)
            self.fb("Received: %s" % resp)
        else:
            self.fb("No Arduino detected. Skipping command: %s" % cmd)

    def onPortSelect(self, *args, **kwds):
        pass

    def onDestroy(self, *args, **kwds):
        Gtk.main_quit()

    def send_command(self):
        cmd = self.command.get_text()
        if cmd:
            self.command.set_text("")
            self.send(cmd)
            return True

    def update_rot(self):
        v = self.rotation.get_value()
        if v != self.rotation_val:
            self.rotation_val = v
            self.send("BT %d" % self.rotation_val)
            return True

    def update_rot_speed(self):
        v = self.rot_speed.get_value()
        if v != self.rot_speed_val:
            self.rot_speed_val = v
            self.send("SAS %d" % self.rot_speed_val)
            return True

    def update_elv(self):
        v = self.elevation.get_value()
        if v != self.elevation_val:
            self.elevation_val = v
            self.send("CM %d" % self.elevation_val)
            return True

    def update_elev_speed(self):
        v = self.elev_speed.get_value()
        if v != self.elev_speed_val:
            self.elev_speed_val = v
            self.send("SHS %d" % self.elev_speed_val)
            return True

    def onInit(self, *args, **kwds):
        self.send("INI 0")

    def onReset(self, *args, **kwds):
        self.send("RM 0")
        self.elevation.set_value(0)
        self.rotation.set_value(0)
        self.rot_speed.set_value(10)
        self.elev_speed.set_value(10)
        self.elevation_val = self.elevation.get_value()
        self.rotation_val = self.rotation.get_value()
        self.rot_speed_val = self.rot_speed.get_value()
        self.elev_speed_val = self.elev_speed.get_value()

    def onSend(self, *args, **kwds):
        if self.send_command():
            return
        if self.update_elv():
            return
        if self.update_rot():
            return

    def onRotRelease(self, *args, **kwds):
        if self.update_rot():
            return

    def onElevRelease(self, *args, **kwds):
        if self.update_elv():
            return

    def onRotSpeedRelease(self, *args, **kwds):
        if self.update_rot_speed():
            return

    def onElevSpeedRelease(self, *args, **kwds):
        if self.update_elev_speed():
            return


def onRotChange(*args, **kwds):
    print args, kwds

if __name__ == "__main__":
    ui = UI()
    ui.show()
    Gtk.main()

