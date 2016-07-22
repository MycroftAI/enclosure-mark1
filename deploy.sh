#!/usr/bin/env bash
MYCROFT_HOSTNAME=${1}

if [[ -z "$MYCROFT_HOSTNAME" ]]; then
    echo "You have to specify a deployment hostname. Usage: deploy.sh <hostname>"
    echo "E.g. deploy.sh pi@localhost"
    exit 1
fi

./compile.sh
echo "Deploying to" ${MYCROFT_HOSTNAME}
sshpass -p 'mycroft' ssh ${MYCROFT_HOSTNAME} "sudo mkdir -p /opt/enclosure"
sshpass -p 'mycroft' scp ${PWD}/build/arduinotest_clickbutton.ino.hex ${MYCROFT_HOSTNAME}:/tmp/arduinotest_clickbutton.ino.hex
sshpass -p 'mycroft' ssh ${MYCROFT_HOSTNAME} "sudo mv /tmp/arduinotest_clickbutton.ino.hex /opt/enclosure"
