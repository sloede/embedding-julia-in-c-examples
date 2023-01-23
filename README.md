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

You need to make sure that your Julia executable is named `julia` and on the
`PATH`, for the [`Makefile`](Makefile) to find it. Then, clone this repository,
enter the repository root directory, and call `make`:
```shell
git clone git@github.com:sloede/embedding-julia-in-c-examples.git
cd embedding-julia-in-c-examples
make
```
This should create an executable named `simple-demo`.

## Usage
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
