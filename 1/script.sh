#!/bin/bash

# I am not an author of this script; the script was given to students of PRL so they can test 
# their projects easily. Because I used mpich instead of openmpi on my machine, this script is 
# a little different in mpicc and mpirun command-line parameters.

if [ $# -ne 1 ]; then
  echo "Chybi ti cislo ty vemeno"
  exit 1;
fi;

if [ $1 -eq 0 ]; then
  exit 2;
fi;
    
mpic++ -o oets oets.cpp -g

dd if=/dev/random bs=1 count=$1 of=numbers >/dev/null 2>&1

mpirun -np $1 ./oets

rm -f oets numbers