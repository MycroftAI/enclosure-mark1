#!/usr/bin/env bash

# Default script behavior
STOPSERVICE=1   # auto stop and restart mycroft-enclosure-client
FORCE=0         # only upload when version info indicates necessary
SCRIPT_DIR=${1:-$(pwd)}

# Parse any command line options
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -s|--skipService)
    STOPSERVICE=0
    ;;
    -f|--force)
    FORCE=1
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done



if [[ $STOPSERVICE -eq 1 ]]; then
    sudo service mycroft-enclosure-client stop
fi

if [[ $FORCE -ne 1 ]]; then
    # Run Python script to determine if upgrade is needed
    python ${SCRIPT_DIR}/verifyArduino.py
    
    if [ $? -ne 1 ]
       if [[ $STOPSERVICE  -eq 1 ]]; then
          sudo service mycroft-enclosure-client start
       fi
      
       # Arduino is up-to-date, no need to update
       echo "Arduino already up to date"
       exit
    fi
fi


# Upload new code to the Arduino
sudo avrdude -p atmega328p -C ${SCRIPT_DIR}/avrdude-gpio.conf -c linuxgpio -v -U flash:w:${SCRIPT_DIR}/build/enclosure.ino.hex
if [ $? -eq 0 ]
then
    # Read and save the version info from the Arduino if write succeeded
    echo "Upload succeeded, saving version info"
    sudo python ${SCRIPT_DIR}/verifyArduino.py --savever
fi


if [[ $STOPSERVICE  -eq 1 ]]; then
    sudo service mycroft-enclosure-client start
fi
