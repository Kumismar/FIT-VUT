#!/bin/bash

EXECUTABLE="./flp24-log"
TEST_DIR="tests"

COLOR_BLUE='\033[1;34m'
COLOR_GREEN='\033[1;32m'
COLOR_RED='\033[1;31m'
COLOR_NC='\033[0m'

if [ ! -f "$EXECUTABLE" ]; then
    make
fi

echo -e "\n${COLOR_BLUE}Running test for NTM simulator.${COLOR_NC}"
echo "==================================="

TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0

for test_file_in in "$TEST_DIR"/*.in; do
    [ -f "$test_file_in" ] || continue

    TEST_COUNT=$((TEST_COUNT + 1))
    test_name=$(basename "$test_file_in" .in)
    rc_file="$TEST_DIR/$test_name.rc"

    echo -e "${COLOR_BLUE}--- Running test: $test_name ---${COLOR_NC}"

    "$EXECUTABLE" < "$test_file_in"
    exit_code=$? 

    expected_rc=$(cat "$rc_file")
    expected_rc=$(echo "$expected_rc" | tr -d '[:space:]')

    echo "Expected return code: ${expected_rc}, actual: ${exit_code}"
    if [ "$exit_code" -eq "$expected_rc" ]; then
        echo -e "${COLOR_GREEN}PASS${COLOR_NC}\n"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${COLOR_RED}FAIL${COLOR_NC}\n"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
done

echo "==================================="
echo -e "Number of tests ran: $TEST_COUNT"
echo -e "${COLOR_GREEN}Passed tests: $PASS_COUNT${COLOR_NC}"
echo -e "${COLOR_RED}Failed tests: $FAIL_COUNT${COLOR_NC}"
echo "==================================="

make clean

exit 0