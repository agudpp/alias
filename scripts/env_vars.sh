#!/bin/bash

# will exit bash if a command doesnt succeed
set -e


# Root path of the repo obtained from this file
CURRENT_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_REPO_DIR=$(realpath "$CURRENT_SCRIPT_DIR/../")
#CONFIG_DIR=$(realpath "$ROOT_REPO_DIR/resources/config/")
SCRIPTS_DIR=$(realpath "$ROOT_REPO_DIR/scripts/")


# check the current user def for the env vars exists
USER_ENV_VARS_FILE="$CURRENT_SCRIPT_DIR/user_env_vars.sh"
if [ -f "$USER_ENV_VARS_FILE" ]; then
    echo "using $USER_ENV_VARS_FILE env vars"
else 
    echo "$USER_ENV_VARS_FILE does not exist and MUST be set"
    exit 1
fi

# include it
. $USER_ENV_VARS_FILE

# Env variables required
if [[ -z "${USER_ALIAS_DEP_ROOT}" ]]; then
  echo "USER_ALIAS_DEP_ROOT environment variable must be set"
  exit 1
fi
if [[ -z "${USER_ALIAS_REPO_ROOT}" ]]; then
  echo "USER_ALIAS_REPO_ROOT environment variable must be set"
  exit 1
fi

# set real ones based on the user ones
export ALIAS_DEP_ROOT="${USER_ALIAS_DEP_ROOT}"
export ALIAS_REPO_ROOT="${USER_ALIAS_REPO_ROOT}"

# setup the LD_LIBRARY_PATH to the correct folder
export LD_LIBRARY_PATH="$ALIAS_DEP_ROOT/lib:$LD_LIBRARY_PATH"

