#include "cga.h"

#include <stdlib.h>
#include <string.h>
#include "consts.h"
#include "ca.h"
#include "utils.h"
#include "params.h"
#include "templates.h"

void evolve(Individual *pop)
{
  int r,i,j,k,count;
  double totFit = 0;
#ifdef F_OUTPUT
  FILE *fp = fopen(F_OUTPUT_FILE,"a+");
#endif
  for(r=0;r<GA_RUNS;r++)
  {
    totFit = 0.0;
#ifdef F_OUTPUT
    fprintf(fp,"Run %3d:",r);
#endif
    fprintf(stderr,"Run %3d:",r);
    for(i=0;i<POPULATION;i++)
    {
      if(params.representation!=REP_BINARY)
        decodeTemplates(&pop[i]);
      pop[i].fitness=0;
      for(j=0;j<MAX_LATS;j++)
      {
        count=0;
        executeCA(&pop[i].lat[j],pop[i].rule,i,pop[i].id);
        for(k=0;k<LAT_SIZE;k++)
          if(pop[i].lat[j].cells[k]=='1')
            count++;
        if( (pop[i].lat[j].density > LAT_SIZE/2 && count == LAT_SIZE) ||
            (pop[i].lat[j].density < LAT_SIZE/2 && count == 0) )
          pop[i].fitness++;
      }
#ifdef F_OUTPUT
      fprintf(fp," %3d ",pop[i].fitness);
#endif
      totFit+=(double)pop[i].fitness;
    }
    totFit/=(double)MAX_LATS;
    bubbleSort(pop);
#ifdef F_OUTPUT
    fprintf(fp,"[%3d](%.3f%%)\n",pop[POPULATION-1].fitness, totFit);
    fflush(fp);
#endif
    fprintf(stderr,"[%3d](%.3f%%)\n",pop[POPULATION-1].fitness, totFit);
    if(r==GA_RUNS) return;
    crossOver(pop);
    mutate(pop,POPULATION-CROSS_AMOUNT);
    for(i=0;i<POPULATION;i++)
      createRandomLattices(&pop[i]);
  }
#ifdef F_OUTPUT
  fclose(fp);
#endif
}

void crossOver(Individual *pop)
{
  Individual fat1,fat2,son1,son2;
  int f1_idx,f2_idx,s1_idx,s2_idx;
  int rest = (POPULATION-1)-CROSS_AMOUNT;
  int point = 0; //Crossover point
  int i,k;
  int with_tpl = 0; //Elite individuals that have at least one template
#ifdef DEBUG
  FILE *fp = fopen("logs/crossover.log","w+");
  fprintf(fp,"Crossing over...\n");
#endif
  for(i=rest;i<POPULATION;i++)
    if(pop[i].n_tpl>0)
      with_tpl++;
  k=0;
  for(i=0;i<rest;i+=2)
  {
    //Select 2 fathers from the 20 best individuals for crossing. With templates, both
    //need at least one template, otherwise another pair is drawn (if any pair can qualify)
    do
    {
      f1_idx = rest + uniformDeviate(rand()) * (POPULATION - rest);
      f2_idx = rest + uniformDeviate(rand()) * (POPULATION - rest);
    }while(params.representation!=REP_BINARY && with_tpl>0 && (pop[f1_idx].n_tpl==0 || pop[f2_idx].n_tpl==0));

    //Single point crossover with probability p_c, cut point uniform in [1,RULE_SIZE-1].
    //point=0 means no crossover: the sons are copies of the fathers.
    if(params.representation==REP_BINARY && uniformDeviate(rand()) < params.cross_rate)
      point = 1 + uniformDeviate(rand()) * (RULE_SIZE-1);
    else
      point = 0;

    //Copy fathers to a temp variable
    memcpy(&fat1,&pop[f1_idx],sizeof(Individual));
    memcpy(&fat2,&pop[f2_idx],sizeof(Individual));

    //Sons start as copies of the fathers, so no field is left uninitialized
    memcpy(&son1,&fat1,sizeof(Individual));
    memcpy(&son2,&fat2,sizeof(Individual));

    //Finnaly, cross the genomes
    memcpy(&son1.rule,       &fat2.rule,       point);
    memcpy(&son1.rule[point],&fat1.rule[point],RULE_SIZE-point);
    memcpy(&son2.rule,       &fat1.rule,       point);
    memcpy(&son2.rule[point],&fat2.rule[point],RULE_SIZE-point);

    //With templates, crossover (probability p_c) swaps one template between the sons
    if(params.representation!=REP_BINARY && son1.n_tpl>0 && son2.n_tpl>0 &&
       uniformDeviate(rand()) < params.cross_rate)
      swapTemplates(&son1,&son2);

    //Set the sons index
    s1_idx = k++;
    s2_idx = k++;
    //And put them on the population
    memcpy(&pop[s1_idx],&son1,sizeof(Individual));
    memcpy(&pop[s2_idx],&son2,sizeof(Individual));
#ifdef DEBUG
    fprintf(fp,"Selecting %d(%d) and %d(%d) as fathers.\n",f1_idx,fat1.fitness, f2_idx,fat2.fitness);
    fprintf(fp,"f1:%s\nf2:%s\n",fat1.rule,fat2.rule);
    fprintf(fp,"Point:%d\n",point);
    fprintf(fp,"s1:%s\ns2:%s\n",son1.rule,son2.rule);
#endif
  }
#ifdef DEBUG
  fclose(fp);
#endif
}

void mutate(Individual *pop, size_t amount)
{
  int i,j;
  double rnd=0.0;
  for(i=0;i<amount;i++)
  {
    if(params.representation!=REP_BINARY)
    {
      mutateTemplates(&pop[i]);
      continue;
    }
    for(j=0;j<RULE_SIZE;j++)
    {
      rnd = uniformDeviate(rand());
      if(rnd < params.mut_rate)
        pop[i].rule[j]=(pop[i].rule[j]=='0'?'1':'0');
    }
  }
}

