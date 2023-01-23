# Embedding Julia in C examples

This repository contains examples for how to embed Julia code in C, i.e., how to
write a C program that passes data to Julia for processing and then continues
using the results.


## Installation
Prerequisites:
* Linux
* GNU make
* gcc
* Julia v1.8
* *optional:* MPI installation with `mpicc` and `mpiexec`

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
Pkg.jl has all information it needs in the
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

Contents after call to `double_me!`:
data:   0.840 0.394 0.783 0.798 0.912 0.198 0.335 0.768 0.278 0.554
result: 1.680 0.789 1.566 1.597 1.823 0.395 0.670 1.536 0.556 1.108
```


## Author
This repository was initiated by [Michael Schlottke-Lakemper](https://lakemper.eu).


## Acknowledgments
Some of the C code used in the demonstrators is inspired from the examples in the
[Julia manual](https://docs.julialang.org/en/v1/manual/embedding/).


## License
This repository is licensed under the MIT license (see [LICENSE.md](LICENSE.md)).
