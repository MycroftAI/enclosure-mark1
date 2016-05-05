#!/usr/bin/env bash
./compile.sh

cp build/enclosure.ino.hex enclosure.ino.hex
tar -czvf enclosure.tar.gz enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf

S3_HOST=s3://bootstrap.mycroft.ai/artifacts
_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public . ${S3_HOST}/amd64/latest/
_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public enclosure.tar.gz ${S3_HOST}/amd64/latest

_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public . ${S3_HOST}/armhf/latest/
_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public enclosure.tar.gz ${S3_HOST}/armhf/latest
