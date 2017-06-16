#!/usr/bin/env bash
set -Ee

if [ ! -d '/opt/arduino' ]; then
    cd /tmp
    wget 'https://downloads.arduino.cc/arduino-1.6.8-linux64.tar.xz'
    tar -xf 'arduino-1.6.8-linux64.tar.xz' -C '/opt'
    cd /opt
    mv 'arduino-1.6.8' 'arduino'
fi
