program main
    implicit none
 
    ! Size of vectors
    integer,parameter :: n = 20480
 
    real(8) :: a, m, tmp
    integer :: i
 
    real(8),dimension(:),allocatable :: x
    real(8),dimension(:),allocatable :: y
    real(8),dimension(:),allocatable :: yomp
    real(8),dimension(:),allocatable :: yacc
 
    call random_number(a)
 
    ! Allocate memory for each vector on host
    allocate(x(n))
    allocate(y(n))
    allocate(yomp(n))
    allocate(yacc(n))
 
    ! Initialize x and y
    call random_number(x)
    call random_number(y)
 
    ! Compute axpy on the host CPU cores
    !$omp parallel do
    do i=1,n
        yomp(i) = a*x(i) + y(i)
    enddo
    !$omp end parallel do
 
    ! Compute axpy on the accelerator
    !$acc kernels copyin(x(1:n),y(1:n)), copyout(yacc(1:n))
    do i=1,n
        yacc(i) = a*x(i) + y(i)
    enddo
    !$acc end kernels
 
    ! Verify the results
    m = -1.
    !$omp parallel do private(tmp) reduction(max:m)
    do i=1,n
        tmp = abs( (yacc(i)-yomp(i))/yomp(i) )
        if ( tmp > m ) m = tmp
    enddo
 
    if ( m < 1E-12) then
        print *, "Success!"
    else
       print *, "Failure!"
    endif
 
    ! Release memory
    deallocate(x)
    deallocate(y)
    deallocate(yomp)
    deallocate(yacc)
 
end program main
