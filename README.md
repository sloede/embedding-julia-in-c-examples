# Embedding Julia in C examples

This repository contains examples for how to embed Julia code in C, i.e., how to
write a C program that passes data to Julia for processing and then continues
using the results.

There are two demonstrators: The "simple demonstrator" just goes through the
motions and shows how to call a Julia function from C for some data processing.
The "MPI demonstrator" is still designed similarly, but in addition uses MPI
communication on the Julia side to communicate data originally created in C.


## Installation
Prerequisites:
* Linux
* GNU make
* gcc
* Julia v1.8
* *for `mpi-demo`:* MPI installation with `mpicc` and `mpiexec`
* *for `mpi-demo-fortran`:* MPI installation with `mpicc`, `mpif90`, and `mpiexec`

You need to make sure that your Julia executable is named `julia` and on the
`PATH` for the `Makefile`s to find it. Then, clone this repository and
enter the repository root directory:
```shell
git clone git@github.com:sloede/embedding-julia-in-c-examples.git
cd embedding-julia-in-c-examples
```

### Simple demonstrator
Go to the `simple-demo` folder and call `make`:
```shell
cd simple-demo
make
```
This should create an executable named `simple-demo`.


### MPI demonstrator
Go to the `mpi-demo` folder and call `make`:
```shell
cd mpi-demo
make
```
This should create an executable named `mpi-demo`.

Next, you need to install all relevant Julia packages. The Julia package manager
Pkg.jl gets all information it needs from the
[`Project.toml`](mpi-demo/Project.toml) and [`Manifest.toml`](mpi-demo/Manifest.toml) files.
To install these packages, execute Julia with the following command:
```shell
julia --project=. -e 'using Pkg; Pkg.instantiate()'
```
Finally, you need to tell Julia to use your system MPI library instead of the
the one that is shipped with Julia, such that both C and Julia use the same MPI
implementation. This can be achieved by executing
```shell
julia --project=. setup-mpi.jl
```
If successful, it will identify your local MPI implementation with an output
similar to this:
```
┌ Info: MPI implementation identified
│   libmpi = "libmpi"
│   version_string = "Open MPI v4.0.3, package: Debian OpenMPI, ident: 4.0.3, repo rev: v4.0.3, Mar 03, 2020\0"
│   impl = "OpenMPI"
│   version = v"4.0.3"
└   abi = "OpenMPI"
┌ Info: MPIPreferences changed
│   binary = "system"
│   libmpi = "libmpi"
│   abi = "OpenMPI"
└   mpiexec = "mpiexec"
```
It will also create a `LocalPreferences.toml` file that is subsequently used by MPI.jl.


### MPI demonstrator for Fortran
Go to the `mpi-demo-fortran` folder and call `make`:
```shell
cd mpi-demo-fortran
make
```
This should create an executable named `mpi-demo-fortran`.

Next, you need to install and configure all relevant Julia packages. For this,
please follow the instructions for the C-based MPI demonstrator
[above](#mpi-demonstrator-for-fortran).


## Usage

### Simple demonstrator
To test the example, just run the created executables:
```shell
./simple-demo
```
This should give you an output similar to this one:
```
Original contents:
data:   0.840 0.394 0.783 0.798 0.912 0.198 0.335 0.768 0.278 0.554
result: 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000

Contents after call to `double_me_julia_style`:
data:   0.840 0.394 0.783 0.798 0.912 0.198 0.335 0.768 0.278 0.554
result: 1.680 0.789 1.566 1.597 1.823 0.395 0.670 1.536 0.556 1.108

Contents after reset:
data:   0.840 0.394 0.783 0.798 0.912 0.198 0.335 0.768 0.278 0.554
result: 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000

Contents after call to `double_me_c_style`:
data:   0.840 0.394 0.783 0.798 0.912 0.198 0.335 0.768 0.278 0.554
result: 1.680 0.789 1.566 1.597 1.823 0.395 0.670 1.536 0.556 1.108
```

Here, first an array of 10 doubles is created in C and filled with random
values. This array and an equal-sized result array are passed to Julia, where
the values from the input array are doubled and written to the output array. The
result is then printed from C.

### MPI demonstrator
To test the example, start a parallel run by executing the generated file with
`mpiexec`. In addition, you need to pass the `--project=.` argument, which tells
Julia that it should use the packages described in the local `Project.toml` and
`Manifest.toml`:
```shell
mpiexec -n 3 ./mpi-demo --project=.
```
This will yield an output similar to following:
```
data on rank   0:     1    2    3    4    5    6    7    8    9   10
data on rank   1:    11   12   13   14   15   16   17   18   19   20
data on rank   2:    21   22   23   24   25   26   27   28   29   30
result from `parallel_sum` = 465 (expected: 465)
```
On each rank, an integer array with 10 entries is generated in the C part such that over all
ranks, the consecutive integers from 1 to $10 \times n_\textrm{ranks}$ are found. The
arrays are then passed to Julia, where they are summed up in parallel using
`MPI_Allreduce`, before the result is returned to C.

### MPI demonstrator for Fortran
To test the example, start a parallel run by executing the generated file with
`mpiexec`. In addition, you need to pass the `--project=.` argument, which tells
Julia that it should use the packages described in the local `Project.toml` and
`Manifest.toml`:
```shell
mpiexec -n 3 ./mpi-demo-fortran --project=.
```
This will yield an output similar to following:
```
data on rank   0:    1    2    3    4    5    6    7    8    9   10
data on rank   1:   11   12   13   14   15   16   17   18   19   20
data on rank   2:   21   22   23   24   25   26   27   28   29   30
result from `parallel_sum` = 465 (expected: 465)
```
On each rank, an integer array with 10 entries is generated in the Fortran part such that over all
ranks, the consecutive integers from 1 to $10 \times n_\textrm{ranks}$ are found. The
arrays are then passed to Julia via C, where they are summed up in parallel using
`MPI_Allreduce`, before the result is returned to Fortran.


## Author
This repository was initiated by [Michael Schlottke-Lakemper](https://lakemper.eu).


## Acknowledgments
Some of the C code used in the demonstrators is inspired from the examples in the
[Julia manual](https://docs.julialang.org/en/v1/manual/embedding/).


## License
This repository is licensed under the MIT license (see [LICENSE.md](LICENSE.md)).
