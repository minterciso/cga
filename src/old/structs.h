#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <stdio.h>

#define LAT_SIZE 149
#define MAX_LATS 100
#define MAX_RULES 128
#define RADIUS 3
#define MODE 2

typedef struct Rule
{
  char cells[RADIUS*2+2];
  char next;
}Rule;

typedef struct Lattice
{
  unsigned int density;
  char cells[LAT_SIZE];
}Lattice;

typedef struct Individual
{
  Lattice lat[MAX_LATS];
  Rule rules[MAX_RULES];
  unsigned int id;
  unsigned int fitness;
}Individual;

#endif //__STRUCTS_H
