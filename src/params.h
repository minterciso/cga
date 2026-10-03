#ifndef __PARAMS_H
#define __PARAMS_H

#include <stdio.h>

//Runtime parameters. Filled once by parseParams() before any thread starts,
//read-only afterwards.
typedef struct Params
{
  double mut_rate;   //Per-bit mutation probability [0,1]
  double cross_rate; //Single point crossover probability p_c [0,1]
  int mode;          //Number of rule symbols: 2 (binary) or 3 (ternary)
  unsigned int seed; //srand() seed; taken from the clock unless --seed is given
}Params;

extern Params params;

//Returns 0 to continue, 1 if the program should exit successfully (--help), -1 on error
int parseParams(int argc, char *argv[]);
void printParams(FILE *stream);

#endif //__PARAMS_H
