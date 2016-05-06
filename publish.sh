#!/usr/bin/env bash

function _init_vars() {
  TOP='.'
  PROJECT_NAME="enclosure"
  VERSION="$(basename $(git describe --abbrev=0 --tags) | sed -e 's/v//g')"
  ARCH="armhf"
  ARTIFACT_BASE="enclosure-${ARCH}-${VERSION}"
  ARTIFACT_DIR=${TOP}/build_artifacts
  S3_HOST=s3://bootstrap.mycroft.ai/artifacts
}

function _compile_arduino_sketch() {
  ./compile.sh
}


function _create_tarball() {
  mkdir -pv ${ARTIFACT_DIR}
  echo $VERSION > ${ARTIFACT_DIR}/latest
  tar -czvf $ARTIFACT_DIR/${ARTIFACT_BASE}.tar.gz enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf
}

function _upload() {
  _run_s3 s3cmd -c  ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public ${ARTIFACT_DIR}/${ARTIFACT_BASE}.tar.gz ${S3_HOST}/${ARCH}/${PROJECT_NAME}/${VERSION}/
  _run_s3 s3cmd -c  ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public ${ARTIFACT_DIR}/latest ${S3_HOST}/${ARCH}/${PROJECT_NAME}/latest
}

function _run_s3() {
  if [[ "$QUIET" ]]; then
    echo "$*"
  else
    eval "$@"
  fi
}

_init_vars
_compile_arduino_sketch
_create_tarball
#_upload
