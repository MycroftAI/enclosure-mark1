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
#!/usr/bin/env bash

if ! grep -qF "mycroft_mark_1" /etc/mycroft/mycroft.conf; then exit; fi

# Default script behavior
STOPSERVICE=1   # auto stop and restart mycroft-enclosure-client
UPDATE=0        # only upload when version info indicates necessary
SCRIPT_DIR=${1:-$(pwd)}

# Parse any command line options
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -s|--skipService)
    STOPSERVICE=0
    ;;
    -f|--force)
    UPDATE=1
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done

# First feedback that boot is progressing.  Change color to soft
# gray and fill eyes.
echo "eyes.color=7365993" > /dev/ttyAMA0  # color=soft gray, #706569
echo "mouth.text=" > /dev/ttyAMA0

if [[ $STOPSERVICE -eq 1 ]]; then
    # stop service to allow reading off the serial port without interference
    sudo service mycroft-enclosure-client stop
fi

if [[ $UPDATE -ne 1 ]]; then
    # Run Python script to determine if upgrade is needed
    python ${SCRIPT_DIR}/verifyArduino.py

    if [ $? -eq 1 ]; then
       UPDATE=1
    else
       # Arduino is up-to-date, no need to update
       echo "Arduino already up to date"
    fi
fi

if [[ $UPDATE -eq 1 ]]; then
    # Provide visual feedback that Arduino is being updated
    echo "eyes.color=16747520" > /dev/ttyAMA0  # color=orange
    sleep 0.25
    echo "mouth.text=UPDATE" > /dev/ttyAMA0
    sleep 0.25

    # Upload new code to the Arduino
    sudo avrdude -p atmega328p -C ${SCRIPT_DIR}/avrdude-gpio.conf -c linuxgpio -v -U flash:w:${SCRIPT_DIR}/build/enclosure.ino.hex
    if [ $? -eq 0 ]; then
        # Read and save the version info from the Arduino if write succeeded
        echo "Upload succeeded, saving version info"
        sudo python ${SCRIPT_DIR}/verifyArduino.py --savever
    fi
fi

# Post-flash stage we go yellow, indicating pre-connection
sleep 0.25
echo "eyes.color=16768768" > /dev/ttyAMA0   # color=yellow
sleep 0.25
echo "mouth.text=" > /dev/ttyAMA0

if [[ $STOPSERVICE  -eq 1 ]]; then
    # restart service if we stopped it
    sudo service mycroft-enclosure-client start
fi
