using MPI

include("mpi-demo.jl")

function main()
  MPI.Init()
  rank = MPI.Comm_rank(MPI.COMM_WORLD)
  nranks = MPI.Comm_size(MPI.COMM_WORLD)

  data = collect(Int32, 1:10) .+ Int32(rank*10)
  println("data on rank $rank: ", join(data, ", "))

  result = parallel_sum(pointer(data), Int32(10), MPI.API.MPI_COMM_WORLD[])
  expected = div((nranks*10)*(nranks*10 + 1), 2);
  if rank == 0
    println("result from `parallel_sum` = $result (expected: $expected)")
  end

  MPI.Finalize()

  return nothing
end
main()
