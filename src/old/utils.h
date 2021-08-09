#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>

#include "structs.h"

double uniformDeviate(int seed);
int timeSeed(void);
void dec2bin(long decimal, char *binary);

//Ugly ugly bubblesort
void sort(Individual *population, int popSize);

#endif //__UTILS_H
