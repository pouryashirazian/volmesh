if [[ $0 == $BASH_SOURCE ]]; then
  echo "This script is supposed to be sourced only"
  exit 1
fi

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

get_current_dir
PYTHON3_VENV_NAME="python3-virtualenv"

# import bash functions
source ${DIR}/scripts/utils.sh

if [ -d "${DIR}/${PYTHON3_VENV_NAME}" ]; then
  info "loading virtual environment under [${DIR}/${PYTHON3_VENV_NAME}]"
  source ${DIR}/${PYTHON3_VENV_NAME}/bin/activate
else
  error "virtual environment is missing. call bootstrap.sh first"
  exit 1
fi