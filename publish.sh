#!/usr/bin/env bash

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
  tar -czvf $ARTIFACT_DIR/${ARTIFACT_BASE}.tar.gz enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf 
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
