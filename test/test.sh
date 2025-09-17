#!/bin/bash

# @ Author: Ondřej Koumar
# @ Email: xkouma02@stud.fit.vutbr.cz
# @ Create Time: 2024-03-30 18:15
# @ Modified time: 2024-03-30 19:57

red="\033[31m"
green="\033[32m"
reset="\033[0m"
bold="\033[1m"
yellow="\033[33m"
current_dir="$(dirname "$0")"
executable="../build/src/Parser"

counter_successful=0
counter_failed=0

for subdirectory in "$current_dir"/koubp_files/*/; do
    dir_base=$(basename "$subdirectory")
    echo -e "${bold}Processing files in directory: $dir_base${reset}"

    for input_file in "$subdirectory"*.koubp; do
        base_name=$(basename "$input_file" .koubp)
        echo -e "${yellow}Processing $dir_base/$base_name${reset}"
        rc_file="$subdirectory/$base_name.koubp.rc"

        if [ -f "$rc_file" ]; then
            expected_return_code=$(cat "$rc_file")

            "$current_dir"/$executable -f "$input_file" > /dev/null
            return_code=$?

            if [ "$return_code" -eq "$expected_return_code" ]; then
                ((counter_successful++))
                echo -e "${green}Test $base_name passed${reset}\n"
            else
                ((counter_failed++))
                echo -e "${red}Test $base_name failed${reset}\n"
            fi
        else
            echo -e "${red}Error: RC file not found for $base_name${reset}\n"
            ((counter_failed++))
        fi
    done
done

echo -e "${green}Tests passed: $counter_successful${reset}"
echo -e "${red}Tests failed: $counter_failed${reset}"