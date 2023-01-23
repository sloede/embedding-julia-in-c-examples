using MPI

function parallel_sum(data::Ptr{Cint}, size::Cint, comm::MPI.MPI_Comm)
  # Convert C types to Julia types
  data_obj = unsafe_wrap(Array, data, size)
  comm_obj = MPI.Comm(comm)

  # Compute local and global sum
  local_sum = Cint(sum(data_obj))
  global_sum = MPI.Allreduce(local_sum, +, comm_obj)

  return global_sum
end

parallel_sum_cfunction() = @cfunction(parallel_sum, Cint, (Ptr{Cint}, Cint, MPI.MPI_Comm))
