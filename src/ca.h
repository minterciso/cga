#ifndef __CA_H
#define __CA_H

#include <stdio.h>
#include "structs.h"
#include "consts.h"

void createRandomLattices(Individual *ind);
void createRandomRules(Individual *ind);
void executeCA(Lattice *lat, char *rule,int ind_idx,int th_idx);
void printCA(FILE *stream, Lattice *lat, int mode);

#endif //__CA_H
