#!/bin/bash

RESET="\033[0m"
BOLD="\033[1m"
UNDERLINE="\033[4m"
RED="\033[31m"

if [ "$#" -ne 1 ]; then
    echo -e "${BOLD}Usage:${RESET} ./test.sh ${UNDERLINE}<input_string>${RESET}\n" >&2
    exit 1
fi

INPUT_STRING=$1
SRC="vuv.cpp"
EXECUTABLE="vuv.out"
FLAGS="-Wall"

mpic++ ${FLAGS} ${SRC} -o ${EXECUTABLE} 

EXIT_CODE=$?
if [ "$EXIT_CODE" -ne 0 ]; then
    echo -e "${BOLD}${RED}Compilation failed${RESET} with exit code ${EXIT_CODE}.\n" >&2
    exit 1
fi

NUM_OF_PROCESSES=$((2 * ${#INPUT_STRING} - 2))

mpirun --oversubscribe -np ${NUM_OF_PROCESSES} ./${EXECUTABLE} ${INPUT_STRING}

EXIT_CODE=$?

if [ "${EXIT_CODE}" -ne 0 ]; then
    echo -e "\n${BOLD}${RED}Run of ${EXECUTABLE} returned ${EXIT_CODE}.${RESET}\n" >&2
    exit 1
fi

rm -f ${EXECUTABLE}

exit 0