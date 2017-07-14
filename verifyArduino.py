#########################################################################
#Copyright 2017, Mycroft AI, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#########################################################################
# This script determines if a Mycroft Mark 1 device needs to
# upload new code to the connected Arduino.  Return codes:
#   1  needs to update
#   0  already loaded with the latest version
# < 0  error code, don't attempt to update
#
# Format: python verifyArduino [-savever]
#      -savever   when specified, the build/version.txt is recreated based
#                 on the Arduino's response

import json
import sys
import serial
import time
import os
import os.path
import mycroft.configuration


def log(str):
    with open("/tmp/verifyOutput.txt", "a") as text_file:
        text_file.write(time.strftime("%Y-%m-%d %H:%M") + " - " + str + "\n")


def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))


def make_sure_path_exists(path):
    try:
        os.makedirs(path)
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise


# Attempt to open the serial port connected to the Arduino
def open_serial_port():
    port = "Undefined"
    try:
        port = config.get("port")
        rate = int(config.get("rate"))
        timeout = int(config.get("timeout"))
        print ("Connecting to: " + port + " rate: " + str(rate) +
               " timeout: " + str(timeout))
        return serial.serial_for_url(url=port, baudrate=rate, timeout=timeout)
    except:
        print "It is not possible to connect to serial port: " + str(port)
        sys.exit(-3)  # don't try to update
        raise


def detect_platform():
    global tty
    if tty is None:
        tty = open_serial_port()
    tty.flushInput()
    tty.flushOutput()
    time.sleep(1)

    # Write "system.version"
    # NOTE: Although 'system.version' didn't exist with older firmware, the
    #       behavior of the Mycroft firmware has to echo whatever was written
    #       to the serial port before attempting to act on it.  So we will
    #       rely on this to identify if a Mycroft Arduino is connected to the
    #       serial port.
    tty.write("system.version")
    time.sleep(1)

    resp = tty.readline().rstrip()
    print "Reply= '" + resp + "'"
    if "Command: system.version" in resp:
        # We have a Mark 1!

        # The serial port may or may not be getting a response from the
        # version command (depending on the Arduino version).  So wait
        # a few moments, then clear it out -- we don't need the response
        # at this point.
        time.sleep(5)
        tty.flushInput()
        tty.flushOutput()
        return "mycroft_mark_1"
    else:
        return "unknown"


log("==============================")
log("Starting verifyArduino.py")

# Files used by this script are kept relative to its own directory.
DIR = get_script_path()
pathVersion = DIR + "/build/version.txt"
pathSketch = DIR + "/build/enclosure.ino.hex"

tty = None  # the serial port interface

##########################################################################
# First, verify that we are running on a Mark 1 device

if not os.path.isfile(pathSketch):
    log("No sketch found")
    sys.exit(-1)  # no Arduino Sketch to upload!

# Read the platform.  We should only be attempting to verify/update
# the Arduino on a Mycroft Mark 1 platform.
config = mycroft.configuration.ConfigurationManager.get().get("enclosure")
platform = config.get('platform')
if platform is None:
    platform = detect_platform()

    if platform is "unknown":
        # Try it one more time before we commit this forever
        time.sleep(1)
        platform = detect_platform()

    # Save the detected platform for this machine so we never have to
    # autodetect again.
    make_sure_path_exists(os.path.dirname(mycroft.configuration.SYSTEM_CONFIG))
    with open(mycroft.configuration.SYSTEM_CONFIG, 'a+') as f:
        f.seek(0)
        try:
            conf = json.load(f)
        except:
            conf = {"enclosure": {"platform": "unknown",
                                  "port": "/dev/ttyAMA0",
                                  "rate": 9600,
                                  "timeout": 5.0}}
        conf.get("enclosure").update({"platform": platform})
        f.seek(0)
        f.truncate()
        json.dump(conf, f)

log("platform is '"+str(platform)+"'")

if not platform == "mycroft_mark_1":
    log("Not a Mark 1 (" + str(platform) + ")")
    sys.exit(-2)  # no Arduino to talk to!

##########################################################################
# Now see if there is a valid "current version" for the Arudino sketch we
# believe is loaded on the Arduino.  If not, we'll go ahead and upload the
# sketch and generate this later...
latestVersion = None
if os.path.isfile(pathVersion):
    # Only trust the version.txt if it was created after the Sketch,
    # otherwise consider it outdated and ignore it.
    if os.path.getmtime(pathVersion) > os.path.getmtime(pathSketch):
        with open(pathVersion, 'r') as myfile:
            latestVersion = myfile.read()

log("latestVersion is '"+str(latestVersion)+"'")

# Attempt to talk to the Arduino over the serial port get loaded Sketch
# version and react accordingly.
#
# We repeat this up to 3 times to deal with issues like the electrical
# line-noise that happens during a power-up sequence.  For the first
# several seconds the serial port often contains extra "bad characters"
# as a result of this environmental noise.
#
result = -3  # unknown state
try:
    if tty is None:
        tty = open_serial_port()

    for i in range(0, 3):
        log("Attempt #"+str(i))

        # Thoroughly flush the serial port.
        time.sleep(0.5)
        tty.flushInput()
        tty.flushOutput()

        # Write "system.version"
        print "Requesting version..."
        tty.write("system.version")
        time.sleep(0.1)

        # Now check to see if we got a response from the version command
        # command we just sent.  Remember, there might not be a Mark 1
        # Arduino on the other side of the serial port.
        resp = tty.readline().rstrip()
        log("Resp1='"+str(resp)+"'")
        print "Reply= '" + resp + "'"
        if "Command: system.version" in resp:
            # Got Arduino ping response, attempting to extract version
            resp = tty.readline().rstrip()
            log("Ver='"+str(resp)+"'")
            print "Reply= '" + resp + "'"
            if "Mycroft Mark 1 v" in resp:
                ver = resp
                print "Ver= '" + ver + "'"

                # Save the version for subsequent testing
                if len(sys.argv) > 1 and sys.argv[1] == "--savever":
                    with open(pathVersion, "w") as text_file:
                        text_file.write(ver)
                    log("Version reported from Arduino saved")
                    result = 0

                if latestVersion == ver:
                    log("Arduino up to date!")
                    result = 0
                else:
                    log("Arduino needs to be updated")
                    result = 1
            else:
                log("system.version not responded to, Arduino needs update")
                result = 1
        else:
            log("Arduino not responding, assuming it needs to be updated")
            result = 1

        if result == 0:
            break

finally:
    tty.close()


sys.exit(result)
