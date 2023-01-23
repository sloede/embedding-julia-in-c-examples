using MPI

function parallel_sum(data::Ptr{Cint}, size::Cint, comm::MPI.MPI_Comm)
  data_obj = unsafe_wrap(Array, data, size)
  return sum(data_obj)
end
