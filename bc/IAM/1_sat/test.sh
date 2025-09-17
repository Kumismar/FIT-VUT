#!/bin/bash

> test.out
for ((N=0; N<=20; N++)); do
    python3 queens.py $N queens.in
    minisat queens.in queens.out
    minisat_status=$?
    if [ $minisat_status -eq 10 ]; then
        echo "rozmer $N SAT" >> test.out
    elif [ $minisat_status -eq 20 ]; then
        echo "rozmer $N UNSAT" >> test.out
    else 
        echo "rozmer $N chyba" >> test.out
    fi
done
