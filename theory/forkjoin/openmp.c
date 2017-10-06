/**
 * Program to scale and project data into 2D and find a centroid
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

long q(long n) {
    if(n<3) {
        return 1;
    }
    double retVal1;
    double retVal2;
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        if(tid == 0) {
            retVal1 = q(n - q(n-1));
        } else {
        retVal2 = q(n-q(n-2));
        }
    }    
    return retVal1 + retVal2;
}
    
int main(int argc, char** argv) {
    long out;
    if(argc!=2) {
        printf("Usage: %s <integer>\nExiting...\n",argv[0]);
    }
    long n = strtol(argv[1],NULL,10);

    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    out = q(n);
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
    
    // Print output
    printf("q(%d) = %d\n",n,out);
    printf("openmp found in %dms\n",msec);
    
    return EXIT_SUCCESS;
}
