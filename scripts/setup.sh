#!/bin/bash

# will exit bash if a command doesnt succeed
set -e

. $(dirname "$0")/env_vars.sh

# create the dependency folder
echo "Creating folder in $ALIAS_DEP_ROOT"
mkdir -p $ALIAS_DEP_ROOT