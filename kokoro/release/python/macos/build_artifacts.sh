#!/bin/bash

set -ex

# change to repo root
pushd $(dirname $0)/../../../..

# Create stage dir
ORIGINAL_DIR=`pwd`
pushd ..
cp -R $ORIGINAL_DIR stage
export STAGE_DIR="`pwd`/stage"
popd

export REPO_DIR=protobuf
export BUILD_VERSION=`grep -i "version" python/google/protobuf/__init__.py | grep -o "'.*'" | tr -d "'"`
export BUILD_COMMIT=`git rev-parse HEAD`
export PLAT=x86_64
export UNICODE_WIDTH=32
export MACOSX_DEPLOYMENT_TARGET=10.9
export TRAVIS_OS_NAME="osx"

rm -rf artifacts/
rm -rf multibuild/
mkdir artifacts
export ARTIFACT_DIR=$(pwd)/artifacts

git clone https://github.com/matthew-brett/multibuild.git
cp kokoro/release/python/macos/config.sh config.sh

OLD_PATH=$PATH

build_artifact_version() {
  MB_PYTHON_VERSION=$1

  # Clean up env
  rm -rf venv
  sudo rm -rf $REPO_DIR
  cp -R $STAGE_DIR $REPO_DIR
  export PATH=$OLD_PATH

  source multibuild/common_utils.sh
  source multibuild/travis_steps.sh
  # before_install
  PIP_CMD="$PYTHON_EXE -m pip"
  $PIP_CMD install virtualenv
  VIRTUALENV_CMD="$(dirname $PYTHON_EXE)/virtualenv"
  export PATH="`dirname $PYTHON_EXE`:$PATH"

  clean_code $REPO_DIR $BUILD_COMMIT

  build_wheel $REPO_DIR/python $PLAT

  mv wheelhouse/* $ARTIFACT_DIR
}

export MB_PYTHON_OSX_VER=10.9
build_artifact_version $MB_PYTHON_VERSION