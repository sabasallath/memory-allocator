#!/bin/bash

# Script safe relative path
SCRIPT=$(realpath $0)
SCRIPT_PATH=$(dirname ${SCRIPT})
cd ${SCRIPT_PATH}

# Start
virtualenv -p python3 env
source env/bin/activate
pip install -r requirements.txt