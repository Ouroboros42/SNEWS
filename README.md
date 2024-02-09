# Time Difference Calculations for SNEWS

## Likelihood
Original python implementation is in [Farrukh's Fit.py](Extra%20Python%20Files/Farrukh's%20Fit.py).
This repo is for development of a C++ optimisation for <1hr calculations.

### Build system
Compilation is controlled by the [Makefile](likelihood/Makefile).
From the likelihood directory, run `make objects`, then `make tests` to run all unit tests. *Note!* the Makefile uses the `find` command, which is part of linux, but not windows. To run the make from windows, use the git-bash terminal, which has `find` built in.

To add code, just make sure all source files are `.cpp` and in the [src](likelihood/src/) directory. Source files which implement a main should go in [entrypoints](likelihood/src/entrypoints/). Only put in [tests](likelihood/src/entrypoints/tests) or [benchmarks](likelihood/src/entrypoints/tests) if approptiate.

The [src](likelihood/src/) directory is automatically added to the *GCC* include-path for including header files.

All compiled outputs go in [build](likelihood/build/).

### Make commands:
* `make wipe` - Empty [build](likelihood/build/)
* `make clean` - Remove all orphaned files from [build](likelihood/build/) (files whose source has been deleted)
* `make objects` - Compile all `.cpp` to `.o`
* `make tests` - Re-compile and run all files in [tests](likelihood/src/entrypoints/tests/)
* `make benches` - Re-compile and run all files in benchmarks [benchmarks](likelihood/src/entrypoints/benchmarks/)
* `make executables` - Re-compile all in [entrypoints](likelihood/src/entrypoints/) into an `.exe` in [build/run](likelihood/build/run/)
* `make tmods` - Re-compile and run all tests whose dependencies have been modified
* `make bmods` - Re-compile and run all benchmarks whose dependencies have been modified
* `make mods` - Re-compile and run all tests & benchmarks whose dependencies have been modified