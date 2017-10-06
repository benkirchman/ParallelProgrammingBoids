#! /bin/bash
srun -n 2 ./cilk 40
srun -n 2 ./serial 40
srun -n 2 ./openmp 40
