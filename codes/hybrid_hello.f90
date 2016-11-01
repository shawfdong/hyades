program main

  use mpi
  use omp_lib
  implicit none

  integer :: ierr, rank, size, tid, thread_level
  character(len=*), parameter :: FMT1 = "(2(A, I4))"

  call MPI_INIT_THREAD(MPI_THREAD_FUNNELED, thread_level, ierr)
  call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)
  call MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierr)

  write(*, FMT1) "Hello, world! I am the main thread of MPI rank ", &
                 rank, " of size ", size

  !$OMP PARALLEL PRIVATE(tid)
  tid = OMP_GET_THREAD_NUM()
  write(*, FMT1) "Hello, world! I am OpenMP thread ", &
                 tid, " of MPI rank ", rank
  !$OMP END PARALLEL

  call MPI_FINALIZE(ierr)

end program main
