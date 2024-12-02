#!/usr/bin/env bash

#------------------------------------------------------------------------------
# Copyright (c) Pourya Shirazian
# All rights reserved.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#------------------------------------------------------------------------------

get_current_dir() {
  SOURCE="${BASH_SOURCE[0]}"

  # resolve $SOURCE until the file is no longer a symlink
  while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    # if $SOURCE was a relative symlink, we need to resolve it relative to
    # the path where the symlink file was located
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
  done
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
}

usage() {
  echo "usage: build.sh [-b <build type debug | release>] [-d <build docs on | off]"
  echo
  echo "   -b: build type      = (default: release)"
  echo "   -b: build docs      = (default: off)"
  echo
  exit 1
}

build() {
  DISTRO=$(get_distro_name)
  if [ -z ${DISTRO} ]; then
    error "distro is not detected."
    return 1
  fi

  BUILD_LINK_NAME="build"
  BUILD_FOLDER_NAME=build-${DISTRO}-${BUILD_TYPE}
  BUILD_DIR=${DIR}/${BUILD_FOLDER_NAME}

  if [ ! -d "${DIR}/vcpkg" ]; then
    info "clone vcpkg"
    git clone https://github.com/Microsoft/vcpkg.git
    check_result_abort_failed $? "Failed when clonning vcpkg"
  fi

  # vcpkg
  ./vcpkg/bootstrap-vcpkg.sh
  check_result_abort_failed $? "Failed when bootstrapping vcpkg"

  # cmake config
  CMAKE_MAKE_PROGRAM="make"
  CMAKE_BUILD_TYPE=$(camelcase $BUILD_TYPE)
  debug "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}"

  cmake -G "Unix Makefiles" -B ${BUILD_DIR} -S . \
        -DCMAKE_TOOLCHAIN_FILE=${DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake \
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
        -DBUILD_DOCS=$(upper $BUILD_DOCS)
  check_result_abort_failed $? "Failed when configuring the build with cmake"

  # build
  cmake --build ${BUILD_DIR}
  check_result_abort_failed $? "build failed"

  # package
  # GIT_SHA=`git log --pretty=format:'%h' -n 1`
  # debug "GIT_SHA = ${GIT_SHA}"

  # update softlink
  if [ -L ${BUILD_LINK_NAME} ]; then
    unlink ${BUILD_LINK_NAME}
  fi
  ln -sf ${BUILD_FOLDER_NAME} ${BUILD_LINK_NAME}
}

get_current_dir
BUILD_TYPE=release
BUILD_DOCS=off

source $DIR/scripts/utils.sh

while getopts ":b:d:h" opt; do
  case "${opt}" in
    b) BUILD_TYPE="$(lower ${OPTARG})";;
    d) BUILD_DOCS="$(lower ${OPTARG})";;
    *) usage;;
  esac
done

# check received arguments
if [ "${BUILD_TYPE}" != "release" ] && [ "${BUILD_TYPE}" != "debug" ]; then
  error "Invalid value for the build argument. supplied = [${BUILD_TYPE}]"
  exit 1
fi

if [ "${BUILD_DOCS}" != "on" ] && [ "${BUILD_DOCS}" != "off" ]; then
  error "Invalid value for the build docs argument. supplied = [${BUILD_DOCS}]"
  exit 1
fi

if [ "${BUILD_DOCS}" == "on" ] && [ -z "$VIRTUAL_ENV" ]; then
  info "bootstrap python virtual environment if not exists"
  ${DIR}/bootstrap.sh

  info "activating python virtual environment if exists"
  source ${DIR}/activate_venv.sh
  check_result_abort_failed $? "failed activating the python virtual environment"
fi

build
check_result_abort_failed $? "build failed"

pushd . 2&> /dev/null
cd ${BUILD_DIR}
ctest
check_result_abort_failed $? "some unit-tests failed"
popd 2&> /dev/null

info "***volmesh build completes successfully***"

