# scab

Some ScaLAPACK Benchmarks. These are meant to be used with the [SLATE](http://icl.utk.edu/slate/) ScaLAPACK replacement.


## Setup

For the C programs, you will need to set `MPICC` appropriately in the Makefile and type `make`. You need ScaLAPACK, LAPACK, and the BLAS installed. You may need to set `LDFLAGS` in the Makefile and/or `LD_LIBRARY_PATH` for all the libraries.

For the R scripts, you need pbdDMAT and its dependencies installed. You can install it via `install.packages("pbdDMAT")`.

See the `scripts/` tree for example SLATE build scripts.
