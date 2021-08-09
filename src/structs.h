#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "consts.h"

typedef struct Lattice
{
  char cells[LAT_SIZE];
  unsigned int density;
}Lattice;

typedef struct Individual
{
  Lattice lat[MAX_LATS];
  char rule[RULE_SIZE];
  unsigned int fitness;
  pthread_t t_id;
  int id;
}Individual;

#endif //__STRUCTS_H
