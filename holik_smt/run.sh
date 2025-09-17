#!/bin/bash

for file in *; do 
    if [ "$file" != "$0"  ] && [ "$file" != "pani.txt" ]; then
        echo -e "\e[33mProcessing file: $file \e[0m"
        python3.12 $file
    fi
done