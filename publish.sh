#!/usr/bin/env bash
./compile.sh

cp build/enclosure.ino.hex enclosure.ino.hex
tar -czvf enclosure.tar.gz enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf

function _run() {
  if [[ "$QUIET" ]]; then
    echo "$*"
  else
    eval "$@"
  fi
}

S3_HOST=s3://bootstrap.mycroft.ai/artifacts
_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public enclosure.tar.gz ${S3_HOST}/amd64/latest/enclosure.tar.gz
_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public enclosure.tar.gz ${S3_HOST}/amd64/latest/enclosure.tar.gz

_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public enclosure.tar.gz ${S3_HOST}/armhf/latest/enclosure.tar.gz
_run s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public enclosure.tar.gz ${S3_HOST}/armhf/latest/enclosure.tar.gz
