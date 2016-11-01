#include <stdio.h>
#include <omp.h>

int main()
{
  int nthreads, tid;
  int ndevices;

  #pragma omp parallel private(nthreads, tid)
  {
    tid = omp_get_thread_num();
    printf("Hello, world! I am thread %d on host\n", tid);

    #pragma omp barrier

    if (tid == 0)
    {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d on host\n\n", nthreads);
    }
  }

  #pragma omp declare target
  {
    ndevices = omp_get_num_devices();
    printf("Number of devices = %d\n\n", ndevices);

    #pragma omp target device(1)
    {

      #pragma omp parallel private(nthreads, tid)
      {
        tid = omp_get_thread_num();
        printf("Hello, world! I am thread %d on mic0\n", tid);

        #pragma omp barrier

        if (tid == 0) 
        {
          nthreads = omp_get_num_threads();
          printf("Number of threads = %d on mic0\n\n", nthreads);
        }
      }
    }

    #pragma omp target device(2)
    {

      #pragma omp parallel private(nthreads, tid)
      {
        tid = omp_get_thread_num();
        printf("Hello, world! I am thread %d on mic1\n", tid);

        #pragma omp barrier

        if (tid == 0)
        {
          nthreads = omp_get_num_threads();
          printf("Number of threads = %d on mic1\n", nthreads);
        }
      }
    }

  }
}

