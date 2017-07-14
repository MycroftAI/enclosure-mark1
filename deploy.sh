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
MYCROFT_HOSTNAME=${1}

if [[ -z "$MYCROFT_HOSTNAME" ]]; then
    echo "You have to specify a deployment hostname. Usage: deploy.sh <hostname>"
    echo "E.g. deploy.sh pi@localhost"
    exit 1
fi

echo "Compiling..."
./compile.sh
if [ $? -eq 0 ]; then
    echo "Success"
else
    echo "Compilation error, stopping"
    exit 1
fi

echo "Deploying to: " ${MYCROFT_HOSTNAME}
sshpass -p 'mycroft' ssh ${MYCROFT_HOSTNAME} "sudo mkdir -p /opt/enclosure"
sshpass -p 'mycroft' scp ${PWD}/build/enclosure.ino.hex ${MYCROFT_HOSTNAME}:/tmp/enclosure.ino.hex
sshpass -p 'mycroft' ssh ${MYCROFT_HOSTNAME} "sudo mv /tmp/enclosure.ino.hex /opt/enclosure"
echo "Burning..."
sshpass -p 'mycroft' ssh ${MYCROFT_HOSTNAME} "sudo avrdude enclosure.ino.hex -p atmega328p -C /opt/mycroft/enclosure/avrdude-gpio.conf -c linuxgpio -v -U flash:w:/opt/enclosure/enclosure.ino.hex"
echo "Cool! : D"
