#! /bin/bash
srun -n 2 ./recover_pins_serial ac5afbb539e900ad6724c5481bbb570f
srun -n 2 ./recover_pins_cilk ac5afbb539e900ad6724c5481bbb570f
srun -n 2 ./recover_pins_openmp ac5afbb539e900ad6724c5481bbb570f
srun -n 2 ./recover_pins_serial d98117def45cfa3830b2391f7906dac7
srun -n 2 ./recover_pins_cilk d98117def45cfa3830b2391f7906dac7
srun -n 1 ./recover_pins_cilk d98117def45cfa3830b2391f7906dac7
srun -n 2 ./recover_pins_openmp d98117def45cfa3830b2391f7906dac7
srun -n 1 ./recover_pins_openmp d98117def45cfa3830b2391f7906dac7

