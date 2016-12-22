# This application runs attempts to determine if a
# Mycroft Mark 1 device needs to upload new code to
# the connected Arduino.  Return codes:
#   1  needs to update
#   0  already loaded with the latest version
# < 0  error code, don't attempt to update
#
# Format: python verifyArduino [-savever]
#      -savever   when specified, the build/version.txt is recreated based
#                 on the Arduino's response

import sys
import serial
import time
import os.path
from mycroft.configuration import ConfigurationManager

pathVersion = "/opt/enclosure/build/version.txt"
pathSketch = "/opt/enclosure/build/enclosure.ino.hex"

if not os.path.isfile(pathSketch):
    sys.exit(-1)  # no Arduino Sketch to upload!


# Read the platform.  This only works with Mycroft Mark 1
config = ConfigurationManager.get().get("enclosure")
platform = config.get('platform')
if not platform == "mycroft_mark_1":
    print "Not a Mark 1 (" + str(platform) + ")"
    sys.exit(-2)  # no Arduino to talk to!

# Attempt to open the serial port connected to the Arduino
port = "Undefined"
try:
    port = config.get("port")
    rate = int(config.get("rate"))
    timeout = int(config.get("timeout"))
    print ("Connecting to: " + port + " rate: " + str(rate) +
            " timeout: " + str(timeout))
    tty = serial.serial_for_url(url=port, baudrate=rate, timeout=timeout)
except:
    print "It is not possible to connect to serial port: " + str(port)
    sys.exit(-3)  # don't try to update
    raise
    
# Now see if there is a valid "current version" for the Arudino sketch we
# have.  If not, we'll go ahead and upload and generate this later...
latestVersion = None
if os.path.isfile(pathVersion):
    # Only trust the version.txt if it was created after the Sketch,
    # otherwise consider it outdated and ignore it.
    if os.path.getmtime(pathVersion) > os.path.getmtime(pathSketch):
        with open(pathVersion, 'r') as myfile:
            latestVersion = myfile.read()

# Attempt to talk to the Arduino over the serial port get loaded Sketch
# version and react accordingly.
#
try:
    # Thoroughly flush the serial port.
    #xxx # Since this happens right after boot, sometimes there is junk
    #xxx # on the serial port line from electrical noise on a Pi, so sleep.
    #xxx # time.sleep(1)
    tty.flushInput()
    tty.flushOutput()

    # Write "system.ping"
    print "Pinging..."
    tty.write("system.version")
    time.sleep(0.1)

    # Now check to see if we got a response from the ping command
    # command we just sent.  Remember, there might not be a Mark 1
    # Arduino on the other side of the serial port.
    resp = tty.readline().rstrip()
    print "Reply= '" + resp + "'"
    if "Command: system.version" in resp:
        # Got Arduino ping response, attempting to extract version
        resp = tty.readline().rstrip()
        print "Reply= '" + resp + "'"
        if "Mycroft Mark 1 v" in resp:
            ver = tty.readline().rstrip()
            print "Reply= '" + ver + "'"

            # Save the version for subsequent testing
            if len(sys.argv) > 1 and sys.argv[1] == "--savever":
                with open(pathVersion, "w") as text_file:
                    text_file.write(ver)
                print "Version reported from Arduino saved"
                sys.exit(0)

            if latestVersion == ver:
                print "Arduino up to date!"
                sys.exit(0)
            else:
                print "Arduino needs to be updated"
                sys.exit(1)
        else:
            print "system.version not responded to, Arduino needs update"
            sys.exit(1)

    else:
        print "Arduino not responding, assuming it needs to be updated"
        sys.exit(1)

finally:
    tty.close()


sys.exit(-3)  # unknown state, don't try to update
