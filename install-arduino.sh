#!/usr/bin/env bash
set -Ee

if [ ! -d '/opt/arduino' ]; then
    cd /tmp
    wget 'http://path/to/arduino/package'
    tar -xf 'arduino-1.6.8-linux64.tar.xz' -C '/opt'
    mv 'arduino-1.6.8' 'arduino'
fi
