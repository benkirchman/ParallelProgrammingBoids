#! /bin/bash

gcc  -g serial.c -lm -lGL -lglut -o serial
gcc -g -fopenmp openmp.c -lm -lGL -lglut -o openmp
gcc -g -fopenmp display.c -lm -lGL -lglut -o display
gcc -g -fcilkplus cilk.c -lm -lGL -lglut -o cilk
