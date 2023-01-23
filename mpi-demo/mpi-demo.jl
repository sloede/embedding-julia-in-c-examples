using MPI

function parallel_sum(data::Ptr{Cint}, size::Cint, comm::MPI.MPI_Comm)
  data_obj = unsafe_wrap(Array, data, size)
  @show MPI.Comm_size(MPI.COMM_WORLD)
  @show MPI.Comm_size(MPI.Comm(comm))

  return Cint(sum(data_obj))
end

parallel_sum_cfunction() = @cfunction(parallel_sum, Cint, (Ptr{Cint}, Cint, MPI.MPI_Comm))
