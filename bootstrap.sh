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
  echo "usage: bootstrap.sh [-p <custom python binary>] "
  echo
  echo "   -p: custom python3 to use      = (default: python3)"
  echo
  exit 1
}

get_current_dir

# import bash functions
. ${DIR}/scripts/utils.sh
. ${DIR}/scripts/buildsettings.sh

while getopts ":p:v" opt; do
  case "${opt}" in
    p) PYTHON3="${OPTARG}";;
    *) usage;;
  esac
done

if [[ "${PYTHON3}" =~ ^python3.* ]]; then
  export PYTHON3=${PYTHON3}
  info "bootstrap.sh: using python3 with binary name ${PYTHON3}"
else
  error "Invalid custom python3 binary. supplied = [${PYTHON3}]"
  exit 1
fi

# 1. install the python virtual environment
# exit if the virtual environment already exists
PYTHON3_VENV_FULLPATH="${DIR}/${PYTHON3_VENV_NAME}"
is_dir_valid_and_nonempty ${PYTHON3_VENV_FULLPATH}
if [ $? -eq 0 ] ; then
  info "python3 virtual environment already exists at ${PYTHON3_VENV_FULLPATH}"
else
  check_min_python_version 3 8

  info "creating a virtual environment under ${PYTHON3_VENV_FULLPATH}"
  ${PYTHON3} -m venv ${PYTHON3_VENV_NAME}
  check_result_abort_failed $? "Failed to initial the virtual environment"

  source ${PYTHON3_VENV_FULLPATH}/bin/activate
  check_result_abort_failed $? "Failed to activate the virtual environment"

  info "install required python modules into the virtual environment"
  ${PYTHON3} -m pip install -r ${DIR}/requirements.txt
  check_result_abort_failed $? "Failed to install the required packages"

  deactivate
fi
