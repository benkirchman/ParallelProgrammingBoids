#! /bin/bash

# This script compiles the traffic.c program
echo "now running traffic.py"
python3 traffic.py traffic.csv
echo "now running traffic.c"
gcc -o traffic traffic.c
./traffic traffic.csv
