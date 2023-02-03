#ifndef LIBTRIXI_H_
#define LIBTRIXI_H_

#include <mpi.h> // required for MPI

void trixi_init(/* int argc, char *argv[] */);
void trixi_finalize();
int trixi_parallel_sum(int* data, int size, MPI_Fint* comm);

#endif // ifndef LIBTRIXI_H_
