program caf_pi
  implicit none

  integer          :: j
  integer*8        :: N_steps, i_step, hits
  integer          :: seed(2)
  double precision :: x, y
  double precision :: pi_sum, pi
  double precision :: pi_global[*]
  double precision, allocatable :: pi_local(:)

  seed(1) = 17*this_image()
  call random_seed(PUT=seed)

  hits = 0_8
  N_steps = 10000000_8
  do i_step=1,N_steps
    call random_number(x)
    call random_number(y)
    if ( (x*x + y*y) <= 1.d0) then
      hits = hits + 1_8
    endif
  enddo
  pi_global = 4.d0*dble(hits)/dble(N_steps)

  SYNC ALL

  if (this_image() == 1) then
    pi_sum = 0.d0
    do j=1,num_images()
      pi_sum = pi_sum + pi_global[j]
    enddo

    pi = pi_sum / num_images() 
    print *, 'pi = ', pi
 endif

end program caf_pi
