# Introduction

In here you'll find the actual source code that was used to examine and find the solutions for the DCT in a 2 dimensional CA via GA. The paper regarding this solution can be found [https://ieeexplore.ieee.org/document/5949850](here).

Since the code is pretty old (circa 2011), and wasn't really touched a lot since then, you may find some different results. This is expected and to be updated both here, and on my [https://github.com/minterciso/cuCGA](CUDA port) of the same GA.

Please understand as well that it's not really optimized to execute **fast**, this will also be addressed if I have some free time.

If you have any questions, bugs, etc, please feel free to contact me.

# Update (10-2026)

So I took some time to check on this code, and update some issues that were found:

1. The build now is running on Cmake instead of autotools...15 years latter to the jam... :P
2. Added some CLI parameters
3. Fixes some issues with the ICs that were sent to the GA
4. Improved the mutation and crossover to (i) reduce the noisiness during the GA run and (ii) better represent the MCH and other GA that were used to reach the DCT algorithm from the paper.

**Now, expect different results from the original paper**

# Building

Requires CMake >= 3.20 and a C compiler with pthreads.

```sh
cmake -S . -B build
cmake --build build
./build/cga
```

Mutation rate, crossover rate, rule representation and random seed are runtime options. The parameters and seed in use are printed at startup, so any run can be repeated; the best rule found is printed at the end as a decimal rule number (same numbering as the paper):

```sh
./build/cga --mutation-rate 0.016 --crossover-rate 1.0 --seed 42
./build/cga --representation single --t-max 9 --hash-prob 0.2857 --seed 42
./build/cga --help
```

`--representation` selects the plain 128-bit rule string (`binary`) or the ternary template representation from the paper, with `single` or `double` orientation. Templates are 7-cell strings over `{0,1,#}`; a neighborhood matched by any template maps to the individual's orientation bit, every other one to its complement. Crossover swaps one template between the two offspring and mutation moves a template cell to one of the other two symbols, so the number of templates of an individual never changes after creation.


The other compile-time switches (`DEBUG`, `VALIDATE`, `USE_BEST`, `F_OUTPUT`) live in `src/consts.h`. With `DEBUG` enabled the program writes into `logs/` relative to the working directory, so create it first.
