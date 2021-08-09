#include <stdio.h>
#include <stdlib.h>

#include <ca.h>
#include <structs.h>
#define POPULATION 100

int main(int argc, char *argv[])
{
  Individual population[POPULATION];
  int i=0,j=0,k=0;

  //Create start batch
  srand(timeSeed());
  for(i=0;i<POPULATION;i++)
  {
    createRandomLattices(&population[i]);
    createRandomRules(&population[i]);
  }

#ifdef DEBUG
  FILE *fp = fopen("debug/individuals.log","w+");
  for(i=0;i<POPULATION;i++)
  {
    fprintf(fp,"Individual %d:",i);
    for(j=0;j<MAX_RULES;j++)
      fprintf(fp,"%c",population[i].rules[j].next);
    fprintf(fp,"\n");
  }
  fclose(fp);
#endif

  //Evolve  
  evolve(&population,POPULATION);
  return EXIT_SUCCESS;
}
