#!/usr/bin/env bash
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


function init_vars() {
  TOP='.'
  PROJECT_NAME="enclosure"
  VERSION="$(basename $(git describe --abbrev=0 --tags) | sed -e 's/v//g')"
  ARCH="armhf"
  ARTIFACT_BASE="enclosure-${ARCH}-${VERSION}"
  ARTIFACT_DIR=${TOP}/build_artifacts
  S3_HOST=s3://bootstrap.mycroft.ai/artifacts
}

function compile_arduino_sketch() {
  ./compile.sh
}


function create_tarball() {
  mkdir -pv ${ARTIFACT_DIR}
  echo $VERSION > ${ARTIFACT_DIR}/latest
  tar -czvf $ARTIFACT_DIR/${ARTIFACT_BASE}.tar.gz build/enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf verifyArduino.py
}

function upload() {
  _run s3cmd -c  ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public ${ARTIFACT_DIR}/${ARTIFACT_BASE}.tar.gz ${S3_HOST}/${ARCH}/${PROJECT_NAME}/${VERSION}/
  _run s3cmd -c  ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public ${ARTIFACT_DIR}/latest ${S3_HOST}/${ARCH}/${PROJECT_NAME}/latest
}

function _run() {
  if [[ "$QUIET" ]]; then
    echo "$*"
  else
    eval "$@"
  fi
}

init_vars
compile_arduino_sketch
create_tarball
upload
