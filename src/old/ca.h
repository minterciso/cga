#ifndef __CA_H
#define __CA_H

#include <stdio.h>

#include "structs.h"
#define CA_RUNS 100

void createRandomLattices(Individual *ind);
void createRandomRules(Individual *ind);
void executeRule(Lattice *lat, Rule rule);
void printLattice(FILE *stream, Lattice lat, int mode);

#endif //__CA_H
