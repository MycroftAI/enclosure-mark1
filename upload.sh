#!/usr/bin/env bash

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
    # Upload new code to the Arduino
    sudo avrdude -p atmega328p -C ${SCRIPT_DIR}/avrdude-gpio.conf -c linuxgpio -v -U flash:w:${SCRIPT_DIR}/build/enclosure.ino.hex
    if [ $? -eq 0 ]; then
        # Read and save the version info from the Arduino if write succeeded
        echo "Upload succeeded, saving version info"
        sudo python ${SCRIPT_DIR}/verifyArduino.py --savever
    fi
fi

if [[ $STOPSERVICE  -eq 1 ]]; then
    # restart service if we stopped it
    sudo service mycroft-enclosure-client start
fi
