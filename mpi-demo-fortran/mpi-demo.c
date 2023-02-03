#include <julia.h>
JULIA_DEFINE_FAST_TLS // only define this once, in an executable (not in a shared library) if you want fast code.

#include <stdio.h> // required for `printf()`
#include <stdlib.h> // required for `rand()`

#include <mpi.h>

// Forward declaration
void print_array(int* data, int size, int rank);
int parallel_sum(int* data, MPI_Comm comm);

int main(int argc, char *argv[]) {
  // Parse options relevant for Julia
  jl_parse_opts(&argc, &argv);

  // Set up the Julia context
  jl_init();

  // Initialize MPI
  MPI_Init(&argc, &argv);

  // Get MPI information
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank, nranks;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &nranks);

  // Create an array in C and fill it with increasing numbers on each rank
  int* data = malloc(sizeof(int)*10);
  for (int i = 0; i < 10; i++) {
    data[i] = rank*10 + i + 1;
  }
  print_array(data, 10, rank);

  // Load Julia file with function definition
  jl_eval_string("include(\"mpi-demo.jl\")");

  // Get function pointer to the Julia function
  int (*parallel_sum)(int*, int, MPI_Comm) = jl_unbox_voidpointer(jl_eval_string("parallel_sum_cfunction()"));

  // Compute parallel sum and compare to analytical result
  int result = parallel_sum(data, 10, comm);
  int expected = (nranks*10)*(nranks*10 + 1)/2;
  if (rank == 0) {
    printf("result from `parallel_sum` = %d (expected: %d)\n", result, expected);
  }

  // perform clean-up tasks in Julia
  jl_atexit_hook(0);

  // Free memory
  free(data);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}

// Auxiliary function for printing the array contents
void print_array(int* data, int size, int rank) {
  printf("data on rank %3d: ", rank);
  for (int i = 0; i < size; i++) {
    printf(" %4d", data[i]);
  }
  printf("\n");
}
