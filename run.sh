#!/usr/bin/env bash

SCRIPT=`realpath $0`
SCRIPT_PATH=`dirname ${SCRIPT}`
DATE=$(date '+_%d%m_%H%M%S')
OUTPUT_PATH=${SCRIPT_PATH}"/out"
SRC_PATH=${SCRIPT_PATH}"/src"
MAKE="cd ${SRC_PATH}; make clean; make; make libmalloc.so; cd ${SCRIPT_PATH}"
TEST_LS=${SRC_PATH}"/make test_ls"
OUTPUT_FILE=${OUTPUT_PATH}"/test_ls"$DATE".csv"


function remake {
    cd ${SRC_PATH}; 
    make clean; 
    make; 
    make libmalloc.so; 
    cd ${SCRIPT_PATH}
}

function exec {
    cd ${SRC_PATH}; 
    mkdir -p ${OUTPUT_PATH}    
    make test_ls 2> $OUTPUT_FILE
    cd ${SCRIPT_PATH}
}

remake

start=`date +%s`
exec
end=`date +%s`


runtime="Runtime = "$((end-start))
echo ${runtime}