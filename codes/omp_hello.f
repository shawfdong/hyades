      program main
      implicit none
      include "omp_lib.h"
      integer nthreads, tid

C$OMP PARALLEL PRIVATE(nthreads, tid)
      tid = OMP_GET_THREAD_NUM()
      print *, "Hello, world! I am thread ", tid
C$OMP BARRIER
      if (tid .eq. 0) then
        nthreads = OMP_GET_NUM_THREADS()
        print *, "Number of threads = ", nthreads
      end if
C$OMP END PARALLEL

      stop
      end
