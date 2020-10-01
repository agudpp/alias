#!/bin/bash

# will exit bash if a command doesnt succeed
set -e


# Root path of the repo obtained from this file
CURRENT_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_REPO_DIR=$(realpath "$CURRENT_SCRIPT_DIR/../")
#CONFIG_DIR=$(realpath "$ROOT_REPO_DIR/resources/config/")
#SCRIPTS_DIR=$(realpath "$ROOT_REPO_DIR/scripts/")


# Env variables required
if [[ -z "${ALIAS_ROOT}" ]]; then
  echo "ALIAS_ROOT environment variable must be set"
  exit 1
fi

# set real ones based on the user ones
export ALIAS_DEP_ROOT="${ALIAS_ROOT}/dependencies"
export ALIAS_REPO_ROOT="${ALIAS_ROOT}/alias"

# setup the LD_LIBRARY_PATH to the correct folder
export LD_LIBRARY_PATH="$ALIAS_DEP_ROOT/lib:$LD_LIBRARY_PATH"

