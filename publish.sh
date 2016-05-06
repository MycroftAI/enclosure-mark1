#!/usr/bin/env bash

#./compile.sh

#cp build/enclosure.ino.hex enclosure.ino.hex
#tar -czvf $MYCROFT_ENCLOSURE_ARTIFACT_DIR/$ARTIFACT_BASE.tar.gz enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf

function _run_s3_upload() {
  if [[ "$QUIET" ]]; then
    echo "$*"
  else
    eval "$@"
  fi
}


function _init_vars() {
  TOP='.'
  PROJECT_NAME="enclosure"
  VERSION="$(basename $(git describe --abbrev=0 --tags) | sed -e 's/v//g')"
  ARCH="armhf"
  ARTIFACT_BASE="mycroft-enclosure-standalone-${ARCH}-${VERSION}"
  MYCROFT_ENCLOSURE_ARTIFACT_DIR=${TOP}/build_artifacts/${ARTIFACT_BASE}
    
  S3_HOST=s3://bootstrap.mycroft.ai/artifacts

}

function _compile_arduino_sketch() {
  ./compile.sh
}


function _create_tarball() {
  mkdir -pv ${MYCROFT_ENCLOSURE_ARTIFACT_DIR}
  tar -czvf $MYCROFT_ENCLOSURE_ARTIFACT_DIR/${ARTIFACT_BASE}-1.tar.gz enclosure.ino.hex upload.sh install-avrdude.sh avrdude-gpio.conf
}

function _upload() {
  echo "${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public ${MYCROFT_ENCLOSURE_ARTIFACT_DIR}/${ARTIFACT_BASE}-1.tar.gz ${S3_HOST}/${ARCH}/${PROJECT_NAME}/${VERSION}/"
  #_run_s3_upload s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public enclosure.tar.gz ${S3_HOST}/amd64/latest/enclosure.tar.gz
  #_run_s3_upload s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public enclosure.tar.gz ${S3_HOST}/amd64/latest/enclosure.tar.gz

  #_run_s3_upload s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer sync --acl-public enclosure.tar.gz ${S3_HOST}/armhf/latest/enclosure.tar.gz
  #_run_s3_upload s3cmd -c ${HOME}/.s3cfg.mycroft-artifact-writer put --acl-public enclosure.tar.gz ${S3_HOST}/armhf/latest/enclosure.tar.gz 

}

_init_vars
_compile_arduino_sketch
_create_tarball
_upload

echo "version=\"${VERSION}\""
echo $ARCH
echo $ARTIFACT_BASE



