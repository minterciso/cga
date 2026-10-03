# Introduction

In here you'll find the actuall source code that was used to examine and find the solutions for the DCT in a 2 dimenssional CA via GA. The paper regarding this solution can be found [https://ieeexplore.ieee.org/document/5949850](here).

Since the code is pretty old (circa 2011), and wasn't really touched a lot since then, you may find some different results. This is expected and to be updated both here, and on my [https://github.com/minterciso/cuCGA](CUDA port) of the same GA.

Please understand as well that it's not really optimized to execute **fast**, this will also be addressed if I have some free time.

If you have any questions, bugs, etc, please feel free to contact me.

# Building

Requires CMake >= 3.20 and a C compiler with pthreads.

```sh
cmake -S . -B build
cmake --build build
./build/cga
```

Mutation rate, crossover rate, number of rule symbols and random seed are runtime options. The seed in use is printed at startup, so any run can be repeated:

```sh
./build/cga --mutation-rate 0.016 --crossover-rate 1.0 --symbols 2 --seed 42
./build/cga --help
```

The other compile-time switches (`DEBUG`, `VALIDATE`, `USE_BEST`, `F_OUTPUT`) live in `src/consts.h`. With `DEBUG` enabled the program writes into `logs/` relative to the working directory, so create it first.
