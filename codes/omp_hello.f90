program main

  use omp_lib
  implicit none

  integer :: nthreads, tid

  !$OMP PARALLEL PRIVATE(nthreads, tid)
  tid = OMP_GET_THREAD_NUM()
  print *, "Hello, world! I am thread ", tid

  !$OMP BARRIER

  if (tid .eq. 0) then
    nthreads = OMP_GET_NUM_THREADS()
    print *, "Number of threads = ", nthreads
  end if
  !$OMP END PARALLEL

end program main
