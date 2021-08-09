#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

double uniformDeviate(int seed)
{
  return seed * (1.0 / (RAND_MAX + 1.0));
}

int timeSeed(void)
{
  time_t now = time(NULL);
  unsigned char *p = (unsigned char *)&now;
  unsigned seed = 0;
  size_t i;
  for(i=0;i<sizeof(now);i++)
    seed = seed * (UCHAR_MAX + 2U) + p[i];
  return seed;
}

void dec2bin(long decimal, char *binary)
{
  int k = 6, n = 0;
  int remain;
   
  do
  {
    remain = decimal % 2;
    decimal = decimal / 2;
    binary[k--] = (remain==0?'0':'1');
  }while (decimal > 0);
}

void sort(Individual *population,int popSize)
{
  int swapped = 0;
  int i = 0;
  Individual tmp;
  do
  {
    swapped = 0;
    for(i=0;i<popSize-1;i++)
    {
      if(population[i].fitness > population[i+1].fitness)
      {
        memcpy(&tmp,&population[i],sizeof(Individual));
        memcpy(&population[i],&population[i+1],sizeof(Individual));
        memcpy(&population[i+1],&tmp,sizeof(Individual));
        swapped = 1;
      }
    }
  }while(swapped==1);
}

