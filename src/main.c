#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "ca.h"
#include "cga.h"
#include "consts.h"
#include "utils.h"
#include "structs.h"
#include "params.h"
#include "templates.h"

void *start_threads(void *individual)
{
  Individual *ind = (Individual*)individual;
  evolve(ind);
  pthread_exit(ind);
}

int main(int argc, char *argv[])
{
  //Individual population[POPULATION];
  ///Individual *population = NULL;
  Individual population[POPULATION];
  pthread_t threads[1];
  int i,j;

  switch(parseParams(argc,argv))
  {
    case 1:  return EXIT_SUCCESS;
    case -1: return EXIT_FAILURE;
  }
  printParams(stderr);

  srand(params.seed);
  /*
  if((population=(Individual*)malloc(sizeof(Individual)*POPULATION))==NULL)
  {
    perror("malloc");
    return EXIT_FAILURE;
  }
  */
  memset(population,'\0',sizeof(Individual)*POPULATION);
  for(i=0;i<POPULATION;i++)
  {
    createRandomLattices(&population[i]);
#ifdef USE_BEST
    memset(population[i].rule,'0',RULE_SIZE);
    hex2bin(BEST_CGA,population[i].rule,32,RULE_SIZE);
    population[i].rule[RULE_SIZE]='\0';
#endif
#ifndef USE_BEST
    if(params.representation==REP_BINARY)
      createRandomRules(&population[i]);
    else
    {
      createRandomTemplates(&population[i]);
      decodeTemplates(&population[i]);
    }
#endif
  }

#ifdef DEBUG
  char fname[FNAME_SIZE];
  memset(fname,'\0',FNAME_SIZE);
  for(i=0;i<POPULATION;i++)
  {
    snprintf(fname,FNAME_SIZE-1,"logs/individual%03d.log",i);
    FILE *fp = fopen(fname,"w+");
    fprintf(fp,"Individual %03d\n",i);
    for(j=0;j<MAX_LATS;j++)
      fprintf(fp,"Lat %03d(%3d):%s\n",j,population[i].lat[j].density,population[i].lat[j].cells);
    fprintf(fp,"Rule: %s\n",population[i].rule);
    fclose(fp);
  }
#endif

  //Now we run the threaded part
  for(i=0;i<1;i++)
  {
    population[i].id = i; //Set before the thread starts reading the population
    pthread_create(&threads[i],NULL,&start_threads,&population);
  }
  for(i=0;i<1;i++)
  {
    pthread_join(threads[i],NULL);
//    fprintf(fp,"%d,%s,%d",i,population[POPULATION-1].rule,population[POPULATION-1].fitness);
  }

  //The population is ranked at the last generation and the elite is never altered
  //afterwards, so the last individual is the best one found
  char rule_dec[RULE_DEC_SIZE];
  Individual *best = &population[POPULATION-1];
  ruleToDecimal(best->rule,rule_dec);
  fprintf(stderr,"Best rule: %s (fitness %u)\n",rule_dec,best->fitness);
  if(params.representation!=REP_BINARY)
  {
    fprintf(stderr,"Templates (orientation %c):",best->orientation);
    for(i=0;i<best->n_tpl;i++)
      fprintf(stderr," %.*s",NEIGH_SIZE,best->tpl[i].cells);
    fprintf(stderr,"\n");
  }
  /*
  //Call GA (and output to a DB)
  FILE *fp = fopen("logs/output_db.csv","w+");
  fprintf(fp,"Iteration,Rule,Fitness\n");
  //Start threads
  fprintf(stderr,"Starting threads\n");
  for(i=0;i<2;i++)
  {
    pthread_create(&t_id,NULL,&start_threads,&population[i]);
    population[i].t_id = t_id;
    population[i].id=i;
  }
  //Join threads
  for(i=0;i<2;i++)
  {
    pthread_join(population[i].t_id,(void**)&population[i]);
    fprintf(fp,"%d,%s,%d",i,population[POPULATION-1].rule,population[POPULATION-1].fitness);
  }
  /*
  for(i=0;i<1;i++)
  {
    evolve(population);
    fprintf(fp,"%d,%s,%d",i,population[POPULATION-1].rule,population[POPULATION-1].fitness);
  }
  
  fclose(fp);
  */
  //free(population);
  //population=NULL;

  return EXIT_SUCCESS;
}
