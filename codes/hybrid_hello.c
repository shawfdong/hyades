#include <stdio.h>
#include <omp.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
  int rank, size;
  int tid, thread_level;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &thread_level);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  printf("Hello, world! I am the main thread of MPI rank %d of size %d\n", rank, size);

  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    printf("Hello, world! I am OpenMP thread %d of MPI rank %d\n", tid, rank);
  }

  MPI_Finalize();

  return 0;
}
