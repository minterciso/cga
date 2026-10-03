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

Mutation rate, crossover rate, rule representation and random seed are runtime options. The parameters and seed in use are printed at startup, so any run can be repeated; the best rule found is printed at the end as a decimal rule number (same numbering as the paper):

```sh
./build/cga --mutation-rate 0.016 --crossover-rate 1.0 --seed 42
./build/cga --representation single --t-max 9 --hash-prob 0.2857 --seed 42
./build/cga --help
```

`--representation` selects the plain 128-bit rule string (`binary`) or the ternary template representation from the paper, with `single` or `double` orientation. Templates are 7-cell strings over `{0,1,#}`; a neighbourhood matched by any template maps to the individual's orientation bit, every other one to its complement. Crossover swaps one template between the two offspring and mutation moves a template cell to one of the other two symbols, so the number of templates of an individual never changes after creation.

Run the tests with `ctest --test-dir build`.

At the end of a run the best rule is evaluated on `--ics` (default 10⁴) unbiased ICs, each cell 1 with probability 0.5, as in the final evaluation of MCH/CMD. The last stdout line is

```
seed=42 train_best=62 rule=04de8db7fb77f5cbf66fcabfdfabd5ff nics=10000 perf=0.5048 perf_strict=0.5048
```

with the rule in hex, neighbourhood `0000000` first (MCH/CMD order). `perf` counts a uniform correct lattice after the last step; `perf_strict` also requires that state to be a fixed point of the rule. `./build/cga --validate <hex>` evaluates a given rule without running the GA. The per-generation trace goes to `logs/output.log` (create `logs/` first).

## CPU and GPU versions

[cuCGA](https://github.com/minterciso/cuCGA) is the CUDA version of this GA. Both share the same host code; only the CA backend differs (`src/backend_cpu.c` here, `src/kernel.cu` there, behind `src/backend.h`). For the same seed and options the two programs produce the same run, generation by generation.

The other compile-time switches (`DEBUG`, `VALIDATE`, `USE_BEST`, `F_OUTPUT`) live in `src/consts.h`. With `DEBUG` enabled the program writes into `logs/` relative to the working directory, so create it first.
