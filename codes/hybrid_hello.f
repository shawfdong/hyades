      program main
      implicit none
      include "mpif.h"
      include "omp_lib.h"
      integer ierr, rank, size, tid, thread_level

      call MPI_INIT_THREAD(MPI_THREAD_FUNNELED, thread_level, ierr)
      call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)
      call MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierr)

      write(*, 10) "Hello, world! I am the main thread of MPI rank ", 
     +         rank, " of size ", size

C$OMP PARALLEL PRIVATE(tid)
      tid = OMP_GET_THREAD_NUM()
      write(*,10) "Hello, world! I am OpenMP thread ", 
     +         tid, " of MPI rank ", rank
C$OMP END PARALLEL
      call MPI_FINALIZE(ierr)

10    format(2(A, I4))
      stop
      end
