/**
 * Program to scale and project data into 2D and find a centroid
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

omp_lock_t lock;
/* struct to hold objects attributes */
struct phaseball {
    float x;
    float y;
    float z;
    float mass;
};

struct objectList {
    float** xs;
    float** ys;
    float** zs;
    float** masses;
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
        v->objects->xs = realloc(v->objects->xs, size);
        v->objects->ys = realloc(v->objects->ys, size);
        v->objects->zs = realloc(v->objects->zs, size);
        v->objects->masses = realloc(v->objects->masses, size);
    }
    struct objectList* objects = v->objects;
    float** xs = objects->xs;
    float* x = xs[(v->last)];
    float* new = &(o->x);
    x = new;
    (v->objects)->ys[(v->last)] = &(o->y);
    (v->objects)->zs[(v->last)] = &(o->z);
    (v->objects)->masses[(v->last)] = &(o->mass);
    (v->last) += 1;
    return;
}
        
// Place phaseballs uniformly in a box, with mass equal to the manhattan distance
void place_uniformly(int sx, int ex, int sy, int ey, int sz, int ez, struct volume* v) {
    int halfx = (ex+sx)/2;
    int halfy = (ey+sy)/2;
    int halfz = (ez+sz)/2;
    int psx[8] = {sx,halfx,sx,halfx,sx,halfx,sx,halfx};
    int psy[8] = {sy,sy,halfy,halfy,sy,sy,halfy,halfy};
    int psz[8] = {sz,sz,sz,sz,halfz,halfz,halfz,halfz};
    int pex[8] = {halfx,ex,halfx,ex,halfx,ex,halfx,ex};
    int pey[8] = {halfy,halfy,ey,ey,halfy,halfy,ey,ey};
    int pez[8] = {halfz,halfz,halfz,halfz,ez,ez,ez,ez};
    struct phaseball* n[8];
    #pragma omp parallel for
    for(int i = 0; i < 8; i++) {
        for(int x=psx[i]; x<=pex[i]; x++) {
	    for(int y=psy[i]; y<=pey[i]; y++) {
	        for(int z=psz[i]; z<=pez[i]; z++) {
                    n[i] = malloc(sizeof(struct phaseball));
	   	    n[i]->x = x;
		    n[i]->y = y;
		    n[i]->z = z;
		    n[i]->mass = 1;
		    n[i]->mass = fabs(n[i]->x)+fabs(n[i]->y)+fabs(n[i]->z);
		    omp_set_lock(&lock);    
                    volume_append(v,n[i]);
		    omp_unset_lock(&lock);
                }
            }
        }
    }
}

// Projects 3D volume to 11x11 2D map and report centroid
void post_process(struct volume* v, float* cx, float* cy) {
    double mass_sum=0.0;
    double wx=0.0;
    double wy=0.0;
    #pragma omp parallel for reduction(+:mass_sum) reduction(+:wx) reduction(+:wy)
    for(int i=0; i<v->last; i++) {
        struct objectList* o = v->objects;
        mass_sum += *(o->masses[i]);
        wx += *(o->xs[i]) * *(o->masses[i]);
        wy += *(o->ys[i]) * *(o->masses[i]);
     }
    *cx = wx/mass_sum;
    *cy = wy/mass_sum;
    
    return;
}

int main(int argc, char** argv) {
    // make a volume to store objects in
    struct volume v;
    v.size=100;
    v.last=0;
    v.objects = malloc(sizeof(struct objectList)); 
    int size = sizeof(float)*(v.size);
    v.objects->xs = malloc(size);
    v.objects->ys = malloc(size);
    v.objects->zs = malloc(size);
    v.objects->masses = malloc(size);
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
