#!/usr/bin/env bash
./compile.sh
MYCROFT_HOSTNAME=pi@10.42.0.2
PROJECT_HOME=/home/pi/git/enclosure
sshpass -p 'mycroft' scp ${PWD}/build/enclosure.ino.hex ${MYCROFT_HOSTNAME}:${PROJECT_HOME}/enclosure.hex
