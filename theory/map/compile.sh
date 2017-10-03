#! /bin/bash
module load gcc-7.2.0
module load openssl-1.1.0
module load tbb-2018
module load slurm
module load openmpi-3.0.0

# Compile the serial version
gcc -o recover_pins_serial serial.c -lcrypto -lm

# Compile the OpenMP version
gcc -o recover_pins_openmp -fopenmp openmp.c -lcrypto

# Compile the cilkplus version
gcc -o recover_pins_cilk -fcilkplus cilk.c -lcrypto

# Compile the tbb version
g++ -o recover_pins_tbb tbb.c -ltbb

# Compile the MPI version
mpicc -o recover_pins_mpi mpi.c

# Run!
./run.sh
