#!/usr/bin/env bash
set -Ee

if [ ! -d '/opt/avrdude' ]; then
    sudo apt-get install bison flex libusb-dev
    cd /tmp
    wget 'http://download.savannah.gnu.org/releases/avrdude/avrdude-6.1.tar.gz'
    tar -xf 'avrdude-6.1.tar.gz'
    cd avrdude-6.1
    ./configure --prefix=/opt/avrdude --enable-linuxgpio
    make
    sudo make install
fi
