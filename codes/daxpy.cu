#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define N 2048
 
// declare the kernel
__global__ void daxpy(double a, double *x, double *y) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        y[i] += a*x[i];
    }
}
 
int main(void) {
    double *x, *y, *d, a, *dx, *dy;
    double m = -1.;
    double tmp;
    int i;
    size_t size = N*sizeof(double);
 
    x = (double *)malloc(size);
    y = (double *)malloc(size);
 
    // initialize x and y
    srand(time(NULL));
    a = (double)random() / RAND_MAX;
    for (i=0; i<N; i++)
        x[i] = (double)random() / RAND_MAX;
    for (i=0; i<N; i++)
        y[i] = (double)random() / RAND_MAX;
 
    // allocate device memory for x and y
    cudaMalloc((void **) &dx, size);
    cudaMalloc((void **) &dy, size);
 
    // copy host memory to device memory
    cudaMemcpy(dx, x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dy, y, size, cudaMemcpyHostToDevice);
 
    // launch the kernel function
    daxpy<<<N/64,64>>>(a, dx, dy);
 
    // copy device memory to host memory
    d = (double *)malloc(size);
    cudaMemcpy(d, dy, size, cudaMemcpyDeviceToHost);
 
    // verify the results
    for (i=0; i<N; i++) {
        y[i] += a * x[i];
	tmp = fabs( (d[i]-y[i])/y[i] );
        if ( tmp > m ) m = tmp;
    }
 
    // deallocate device memory
    cudaFree(dx);
    cudaFree(dy);
    free(x);
    free(y);
    free(d);
 
    if ( m < 1E-12 ) {
        printf("Success!\n");
	return 0;
    }
    else {
        printf("Failure!\n");
	return 1;
    }
}
