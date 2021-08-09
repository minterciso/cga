#ifndef __GA_H
#define __GA_H

#include <stdio.h>
#include "structs.h"

#define GA_RUNS 100

void evolve(Individual *population,int popSize);
void crossOver(Individual *population,int popSize);
void mutate(Individual *population,int popSize);

#endif //__GA_H
