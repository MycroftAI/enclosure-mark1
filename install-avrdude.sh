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
