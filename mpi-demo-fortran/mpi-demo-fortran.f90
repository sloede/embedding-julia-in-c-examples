module LibTrixi
  implicit none

  interface
    subroutine trixi_init() bind(c)
    end subroutine

    subroutine trixi_finalize() bind(c)
    end subroutine

    function trixi_parallel_sum(data_, size_, comm) result(res) bind(c)
      use, intrinsic :: iso_c_binding, only: c_int
      integer(c_int), intent(in) :: data_(*)
      integer(c_int), value, intent(in) :: size_
      integer, intent(in) :: comm
      integer(c_int) :: res
    end function 
  end interface
end module


program mpi_demo_fortran
  use LibTrixi
  use iso_fortran_env, only: real64

  implicit none

  include 'mpif.h'

  integer :: input(10)
  integer :: res
  integer i, ierror, rank, nranks, expected

  ! Initialize MPI
  call MPI_Init(ierror)

  ! Initialize Julia and Trixi
  call trixi_init()

  ! Create an array in Fortran and fill it with increasing numbers on each rank
  call MPI_Comm_rank(MPI_COMM_WORLD, rank, ierror)
  do i = 1,size(input)
    input(i) = rank*10 + i
  end do
  print "('data on rank ', i3, ': ', *(i4,:,' '))", rank, input

  ! Compute parallel sum and compare to analytical result
  res = trixi_parallel_sum(input, size(input), MPI_COMM_WORLD)
  if (rank == 0) then
    call MPI_Comm_size(MPI_COMM_WORLD, nranks, ierror)
    expected = (nranks*10)*(nranks*10 + 1)/2
    print "('result from `parallel_sum` = ', g0, ' (expected: ', g0, ')')", res, expected
  end if

  ! Finalize Trixi and Julia
  call trixi_finalize()

  ! Finalize MPI
  call MPI_Finalize(ierror)
end program
