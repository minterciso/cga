#include "ga.h"

#include <stdlib.h>
#include <string.h>
#include "ca.h"

void evolve(Individual *population,int popSize)
{
  int i,j,k,t;
  int counter = 0;
  for(t=0;t<GA_RUNS;t++)
  {
    fprintf(stderr,"Run %d:",t);
    for(i = 0; i < popSize;i++)
    {
      fprintf(stderr,".");
      population[i].id=i;
      population[i].fitness=0;
      for(j = 0; j < MAX_LATS; j++)
      {
        counter = 0;
        /*
        printLattice(stderr,population[i].lat[j],0);
        fprintf(stderr,"\n");
        */
        for(k=0;k<MAX_RULES;k++) 
          executeRule(&population[i].lat[j], population[i].rules[k]);
        /*
        printLattice(stderr,population[i].lat[j],0);
        fprintf(stderr,"\n");
        */

        //Calculate fitness
        for(k=0;k<LAT_SIZE;k++)
          if(population[i].lat[j].cells[k]=='1') counter++;
        if(counter!=0)
          fprintf(stderr,"We have something...\n");

        if( (counter==0 && population[i].lat[j].density < LAT_SIZE/2) ||
            (counter==LAT_SIZE-1 && population[i].lat[j].density > LAT_SIZE/2))
          population[i].fitness++;
      }
    }
    //Now we sort
    sort(population,popSize);
    fprintf(stderr,"%d\n",population[popSize-1].fitness);
    if(t == GA_RUNS-1) return;
    //Crossing over and mutating
    crossOver(population,popSize);
    mutate(population,popSize);
    //Recreate lattices
    for(i=0;i<popSize;i++)
    {
      population[i].fitness = 0;
      createRandomLattices(&population[i]);
    }
  }
}

void crossOver(Individual *population,int popSize)
{
  int amount    = 10; //The CGA picks up the best 20 individuals to remake the population
  int crossProb = 1;  //Single-Point crossover with 1.0 probability (pick one point at random)
  int point = 0;
  int i,k=0;
  int fat1_idx,fat2_idx;
  Individual fat1,fat2;
  Individual son1,son2;
  memset(&son1,'\0',sizeof(Individual));
  memset(&son2,'\0',sizeof(Individual));
#ifdef DEBUG
  FILE *fp = fopen("debug/cross.log","w+");
  fprintf(fp,"===STARTING CROSSOVER===\n");
#endif
  while(k < popSize-amount)
  {
    //point = uniformDeviate(rand()) * (MAX_RULES-1); //single point crossover
    point = rand() % (MAX_RULES-1);
#ifdef DEBUG
    fprintf(fp,"\n***Selected point %d***\n",point);
#endif 

    //fat1_idx = amount + uniformDeviate(rand()) * (popSize - amount);
    //fat2_idx = amount + uniformDeviate(rand()) * (popSize - amount);
    fat1_idx = amount + rand() % (popSize - amount);
    fat2_idx = amount + rand() % (popSize - amount);

#ifdef DEBUG
    fprintf(fp,"Fat1_idx=%d\tFat2_idx=%d\n",fat1_idx,fat2_idx);
#endif

    memcpy(&fat1,&population[fat1_idx],sizeof(Individual));
    memcpy(&fat2,&population[fat2_idx],sizeof(Individual));

#ifdef DEBUG
    fprintf(fp,"\n===FATHER Rules===\n");
    for(i = 0; i < MAX_RULES;i++)
      fprintf(fp,"%c",fat1.rules[i].next);
    fprintf(fp,"\n");
    for(i = 0; i < MAX_RULES;i++)
      fprintf(fp,"%c",fat2.rules[i].next);
#endif

    memcpy(&son1.rules,         &fat1.rules,         sizeof(Rule)*point);
    memcpy(&son1.rules[point],&fat2.rules[point],sizeof(Rule)*(MAX_RULES-point));
    memcpy(&son2.rules,         &fat1.rules[point],sizeof(Rule)*(MAX_RULES-point));
    memcpy(&son2.rules[MAX_RULES-point],&fat2.rules,         sizeof(Rule)*point);

#ifdef DEBUG
    fprintf(fp,"\n===SON Rules===\n");
    for(i = 0; i < MAX_RULES;i++)
      fprintf(fp,"%c",son1.rules[i].next);
    fprintf(fp,"\n");
    for(i = 0; i < MAX_RULES;i++)
      fprintf(fp,"%c",son2.rules[i].next);
#endif

    memcpy(&population[k++],&son1,sizeof(Individual));
    memcpy(&population[k++],&son2,sizeof(Individual));

#ifdef DEBUG
    fprintf(fp,"\n===INDIVIDUAL %d Rules===\n",(k-2));
    for(i = 0; i < MAX_RULES;i++)
      fprintf(fp,"%c",population[(k-2)].rules[i].next);
    fprintf(fp,"\n===INDIVIDUAL %d Rules===\n",(k-1));
    for(i = 0; i < MAX_RULES;i++)
      fprintf(fp,"%c",population[(k-1)].rules[i].next);
#endif
  }
#ifdef DEBUG
  fclose(fp);
#endif
}

void mutate(Individual *population,int popSize)
{
  double rate = 0.015625;
  double rnd  = 0.0;
  int i,t;
#ifdef DEBUG
  FILE *fp = fopen("debug/mutation.log","w+");
  fprintf(fp,"===MUTATING===\n");
#endif
  for(i=0;i<popSize;i++)
  {
#ifdef DEBUG
    int k;
    fprintf(fp,"\nIndividual %d:",i);
    for(k=0;k<MAX_RULES;k++)
      fprintf(fp,"%c",population[i].rules[k].next);
    fprintf(fp,"\n");
#endif
    for(t=0;t<MAX_RULES;t++)
    {
      rnd = (double)rand()/(double)RAND_MAX;
      if(rnd <= rate && rnd > 0)
        population[i].rules[t].next = (population[i].rules[t].next=='0'?'1':'0');
    }
#ifdef DEBUG
    fprintf(fp,"\nIndividual %d:",i);
    for(k=0;k<MAX_RULES;k++)
      fprintf(fp,"%c",population[i].rules[k].next);
    fprintf(fp,"\n");
#endif
  }
#ifdef DEBUG
  fclose(fp);
#endif
}

