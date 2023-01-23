# Embedding Julia in C examples

This repository contains examples for how to embed Julia code in C, i.e., how to
write a C program that passes data to Julia for processing and then continues
using the results.

## Installation & usage
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
This should create an executable named `simple-demo`. To test the
example, just run this executable:
```shell
./simple-demo
```

## Author
This repository was initiated by [Michael Schlottke-Lakemper](https://lakemper.eu).

## Acknowledgments
Some of the C code used in the demonstrators is inspired from the examples in the
[Julia manual](https://docs.julialang.org/en/v1/manual/embedding/).

## License
This repository is licensed under the MIT license (see [LICENSE.md](LICENSE.md)).
