#!/bin/bash

# Script safe relative path
SCRIPT=$(realpath $0)
SCRIPT_PATH=$(dirname ${SCRIPT})
cd ${SCRIPT_PATH}

# Start
source env/bin/activate