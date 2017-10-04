
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

/* struct to hold objects attributes */
struct phaseball {
    float x;
    float y;
    float z;
    float mass;
};

struct objectList {
    float* xs;
    float* ys;
    float* zs;
    float* masses;
};

struct volume {
    size_t size;
    size_t last;
    struct objectList* objects;
};

// Add phaseball to a volume
void volume_append(struct volume* v, struct phaseball* o) {
 if( v->last == v->size ) {
        (v->size) += 100;
        int size = sizeof(float)*(v->size);
        v->objects->xs = (float*) realloc(v->objects->xs, size);
        v->objects->ys = (float*) realloc(v->objects->ys, size);
        v->objects->zs = (float*) realloc(v->objects->zs, size);
        v->objects->masses = (float*) realloc(v->objects->masses, size);
    }
    (v->objects)->xs[(v->last)] = (o->x);
    (v->objects)->ys[(v->last)] = (o->y);
    (v->objects)->zs[(v->last)] = (o->z);
    (v->objects)->masses[(v->last)] = (o->mass);
    (v->last) += 1;
return;
}
        
// Place phaseballs uniformly in a box, with mass equal to the manhattan distance
void place_uniformly(int sx, int ex, int sy, int ey, int sz, int ez, struct volume* v) {
    for(int i=sx; i<=ex; i++) {
        for(int j=sy; j<=ey; j++) {
            for(int k=sz; k<=ez; k++) {
                struct phaseball* n = (phaseball*) malloc(sizeof(struct phaseball));
                n->x = i;
                n->y = j;
                n->z = k;
                n->mass = 1;
                n->mass = fabs(n->x)+fabs(n->y)+fabs(n->z);
                volume_append(v,n);
            }
        }
    }
}

// Projects 3D volume to 11x11 2D map and report centroid
void post_process(struct volume* v, float* cx, float* cy) {
    cilk::reducer< cilk::op_add<double> > x_reducer (0.0);
    cilk::reducer< cilk::op_add<double> > y_reducer (0.0);
    cilk::reducer< cilk::op_add<double> > mass_reducer (0.0);
    cilk_for(int i=0; i<v->last; i++) {
        *mass_reducer += v->objects->masses[i];
        *x_reducer += v->objects->xs[i] * v->objects->masses[i];
        *y_reducer += v->objects->ys[i] * v->objects->masses[i];
    }
    
    *cx = x_reducer.get_value()/mass_reducer.get_value();
    *cy = y_reducer.get_value()/mass_reducer.get_value();
    
    return;
}

int main(int argc, char** argv) {
    // make a volume to store objects in
    struct volume v;
    v.size=100;
    v.last=0;
    v.objects = (objectList*) malloc(sizeof(struct objectList)); 
    int size = sizeof(float)*(v.size);
    v.objects->xs = (float*) malloc(size);
    v.objects->ys = (float*) malloc(size);
    v.objects->zs = (float*) malloc(size);
    v.objects->masses = (float*) malloc(size);
    // Set the initial configuration
    place_uniformly(-1000,1000,-100,100,-100,100,&v);

    // Generate output
    float cx,cy;
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    post_process(&v, &cx, &cy);
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
    
    // Print output
    printf("Centroid at: %f,%f\n",cx,cy);
    printf("found in %dms\n",msec);
    
    return EXIT_SUCCESS;
}

