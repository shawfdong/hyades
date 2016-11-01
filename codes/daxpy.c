#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 20480
 
int main(void) {
 
  double *x, *y;
  size_t size = N*sizeof(double);
 
  x = (double *)malloc(size);
  y = (double *)malloc(size);
 
  // initialize x and y
  srand(time(NULL));
  double a = (double)random() / RAND_MAX;
  int i;
  for (i=0; i<N; i++)
    x[i] = (double)random() / RAND_MAX;
  for (i=0; i<N; i++)
    y[i] = (double)random() / RAND_MAX;
 
  // compute axpy on the host CPU cores
  double *yomp;
  yomp = (double *)malloc(size);
    #pragma omp parallel for
  for (i=0; i<N; i++) {
    yomp[i] = a * x[i] + y[i];
  }
 
  // compute axpy on the accelerator
  double *yacc;
  yacc = (double *)malloc(size);
#pragma acc kernels copyin(x[0:N],y[0:N]), copyout(yacc[0:N])
  for (i=0; i<N; i++) {
    yacc[i] = a*x[i] + y[i];
  }
 
  // verify the results
  double m = -1.;
  double tmp;
#pragma omp parallel for private(tmp) reduction(max:m)
  for (i=0; i<N; i++) {
    tmp = fabs( (yacc[i]-yomp[i])/yomp[i] );
    if ( tmp > m ) m = tmp;
  }
 
  // release memory
  free(x);
  free(y);
  free(yomp);
  free(yacc);
 
  if ( m < 1E-12 ) {
    printf("Success!\n");
    return 0;
  }
  else {
    printf("Failure!\n");
    return 1;
  }
}
