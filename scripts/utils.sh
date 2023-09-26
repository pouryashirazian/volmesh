#!/usr/bin/env bash

# Return values
SUCCESS=0
FAILURE=1

# Colored output
NC='\033[0m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
CYAN='\033[0;36m'

message() { echo "$1"; }
info()    { echo -e "${GREEN}$1${NC}"; }
warning() { echo -e "${YELLOW}WARNING: $1${NC}"; }
error()   { echo -e "${RED}ERROR: $1${NC}"; }
debug() {
  if [ "$VERBOSE_DEBUG" == "ON" ]; then
    echo -e "${CYAN}$1${NC}"
  fi
}

lower() {
  result=$(echo ${1} | tr [:upper:] [:lower:] )
  echo $result
}

upper() {
  result=$(echo ${1} | tr [:lower:] [:upper:] )
  echo $result
}

camelcase() {
  input=$(lower ${1})
  result="$(tr [:lower:] [:upper:] <<< ${input:0:1})${input:1}"
  echo $result
}

get_distro_name() {
  # Determine OS platform
  UNAME=$(uname | tr "[:upper:]" "[:lower:]")
  # If Linux, try to determine specific distribution
  if [ "$UNAME" == "linux" ]; then
      # If available, use LSB to identify distribution
      if [ -f /etc/lsb-release -o -d /etc/lsb-release.d ]; then
          OS_DIST_NAME=$(lsb_release -i | cut -d: -f2 | sed s/'^\t'//)
          OS_REL_VER=$(lsb_release -r | cut -d: -f2 | sed s/'^\t'//)
          export DISTRO=${OS_DIST_NAME}${OS_REL_VER}
      # Otherwise, use release info file
      else
          export DISTRO=$(ls -d /etc/[A-Za-z]*[_-][rv]e[lr]* | grep -v "lsb" | cut -d'/' -f3 | cut -d'-' -f1 | cut -d'_' -f1)
      fi
  fi
  # For everything else (or if above failed), just use generic identifier
  [ "$DISTRO" == "" ] && export DISTRO=$UNAME
  export DISTRO=$(lower $DISTRO)

  unset UNAME

  echo "$DISTRO"
}

check_result_abort_failed() {
  msg="The previous command failed with non-zero exit code $1."
  if [ -n "$2" ]; then
    msg="$2"
  fi

  if [ $1 -ne 0 ]; then
    error "$msg"
    exit 1
  fi
}

check_result_continue_failed() {
  msg="The previous command failed with non-zero exit code $1."
  if [ -n "$2" ]; then
    msg="$2"
  fi

  if [ $1 -ne 0 ]; then
    error "$msg"
  fi
}

check_exact_python_version() {
  local _major=$1
  local _minor=$2
  local _cmd="import sys; sys.exit(0) if (sys.version_info.major == ${_major}) and (sys.version_info.minor == ${_minor}) else sys.exit(1)"
  ${PYTHON3} -c "${_cmd}";
  if [ $? -ne 0 ]; then
    local _installed_version=`${PYTHON3} --version`
    error "Invalid python version [${_installed_version}]"
    exit 1
  else
    info "The correct python version is installed"
  fi
}

check_min_python_version() {
  local _major=$1
  local _minor=$2
  local _cmd="import sys; sys.exit(0) if (sys.version_info.major == ${_major}) and (sys.version_info.minor >= ${_minor}) else sys.exit(1)"
  ${PYTHON3} -c "${_cmd}";
  if [ $? -ne 0 ]; then
    local _installed_version=`${PYTHON3} --version`
    error "Invalid python version [${_installed_version}]"
    exit 1
  else
    info "The correct python version is installed"
  fi
}

check_application_exist() {
  result=0
  for _app in "$@"; do
    hash $_app &> /dev/null
    if [ $? -ne 0 ]; then
      error "${_app} is required but does not exist."
      result=1
    fi
  done
  return $result
}

get_cpu_cores_count() {
  # Determine number of core(s) available.  If NUM_CORES is already defined
  # as environment variable, take it.
  if [ -z "${NUM_CORES}" ]; then
    NUM_CORES=1
    if [ -x "$(command -v nproc)" ]; then
      # Linux
      NUM_CORES=`nproc --all`
    elif [ -x "$(command -v sysctl)" ]; then
      # Mac
      NUM_CORES=`sysctl -n hw.ncpu`
    fi
  fi

  echo ${NUM_CORES}
}

is_git_repo() {
  if git rev-parse --git-dir > /dev/null 2>&1; then
    echo "true"
  else
    echo "false"
  fi
}

get_git_last_commit() {
  echo "$(git log -1 --format='%h')"
}

get_git_branch_name() {
  branch_name=$(git symbolic-ref -q HEAD)
  branch_name=${branch_name##refs/heads/}
  branch_name=${branch_name:-HEAD}
  echo $branch_name
}

is_dir_valid_and_nonempty() {
  local _directory=$1
  if [ -d ${_directory} 2> /dev/null ]; then
    if [ -n "$(find ${_directory} -prune -empty -type d 2>/dev/null)" ]; then
      # valid but empty directory
      return 1
    else
      # valid and contains files
      return 0
    fi
  else
    # invalid directory
    return 1
  fi
}

check_file_exists_remote_server() {
  if [ "$#" -eq 4 ]; then
    local _username=$1
    local _password=$2
    local _hostname=$3
    local _filepath=$4

    sshpass -p ${_password} ssh ${_username}@${_hostname} "[ -f ${_filepath} ] && exit 0 || exit 1" > /dev/null 2>&1
    return $?
  else
    error "invalid number of input arguments expected [4] received [$#]"
    return 1
  fi
}

download_file_from_remote_server() {
  if [ "$#" -eq 5 ]; then
    local _username=$1
    local _password=$2
    local _hostname=$3
    local _filepath=$4
    local _destdir=$5

    check_file_exists_remote_server $_username $_password $_hostname $_filepath
    if [ $? -eq 0 ]; then
      sshpass -p ${_password} rsync -r -v --progress -e ssh ${_username}@${_hostname}:${_filepath} ${_destdir} > /dev/null 2>&1
      return $?
    else
      return 1
    fi
  else
    error "invalid number of input arguments expected [5] received [$#]"
    return 1
  fi
}

copy_local_file() {
  if [ "$#" -eq 2 ]; then
    local _filepath=$1
    local _destdir=$2
    if [ -f ${_filepath} ]; then
      cp ${_filepath} ${_destdir} > /dev/null 2>&1
      return $?
    else
      error "The required file [${_filepath}] does not exist"
      return 1
    fi
  else
    error "invalid number of input arguments expected [2] received [$#]"
    return 1
  fi
}

file_exists_matches_md5sum() {
  result=1
  if [ "$#" -eq 2 ]; then
    local _filepath=$1
    local _md5sum=$2
    if [ -f ${_filepath} ]; then
      computed_hash=$(md5sum ${_filepath} | cut -d\  -f1)
      if [ "$computed_hash" = "$_md5sum" ]; then
        result=0
      fi
    fi
  else
    error "invalid number of input arguments expected [2] received [$#]"
    return 1
  fi
  return $result
}

run_cmd() {
	if [ "$#" -ne 1 ]; then
		error "Illegal number of parameters: $#, should be 1"
		exit $FAILURE
	fi
	eval $1
	ret_code=$?
	if [ $ret_code != 0 ]; then
		error "$ret_code when executing command: ${Cyan} $1"
		exit $ret_code
	fi
}

try_cmd() {
	if [ "$#" -ne 1 ]; then
		error "Illegal number of parameters: $#, should be 1"
		exit $FAILURE
	fi
	eval $1
}