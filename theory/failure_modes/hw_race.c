#include <omp.h>
#include <stdio.h>
#include <pthread.h>

int main(int argc, char** argv) {
    size_t T = 8;
    int t_hello[T];
    int i = 0; 
    for(i = 0; i < T; i++) {
        t_hello[i] = -1;
    }
    #pragma omp parallel num_threads(8)
    {
        t_hello[omp_get_thread_num()]=omp_get_thread_num();
    }
   
    for(i = 0; i < T; i++) {
        printf("Hello from %d\n",t_hello[i]);
    }
    return 0;
}
