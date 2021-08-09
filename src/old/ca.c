#include "ca.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void createRandomLattices(Individual *ind)
{
  assert(ind != NULL);
  int qtdSet = 0;   //How many bits were setted
  int rnd = 0;      //Random number
  int t = 0;
  for(t = 0; t < MAX_LATS; t++)
  {
    qtdSet=0;
    ind->lat[t].density = uniformDeviate(rand()) * (LAT_SIZE-1);  //Random density
    memset(ind->lat[t].cells,'0',LAT_SIZE);
    do
    {
      rnd = uniformDeviate(rand())*(LAT_SIZE-1); //Random index to set
      if(ind->lat[t].cells[rnd]!='1')
      {
        ind->lat[t].cells[rnd]='1';
        qtdSet++;
      }
    }while(qtdSet <= ind->lat[t].density);
  }
}

void createRandomRules(Individual *ind)
{
  assert(ind != NULL);
  int i=0;
  int rnd = 0;

  for(i = 0; i < MAX_RULES; i++)
  {
    memset(ind->rules[i].cells,'0',(RADIUS*2)+1);
    rnd = uniformDeviate(rand()) * MODE;
    //rnd = rand()%MODE;
    switch(rnd)
    {
      case 0: ind->rules[i].next = '0';break;
      case 1: ind->rules[i].next = '1';break;
      case 2: ind->rules[i].next = '2';break;
    }
    dec2bin(i,ind->rules[i].cells);
    ind->rules[i].cells[RADIUS*2+1]='\0';
  }
}

void executeRule(Lattice *lat, Rule rule)
{
  char result[LAT_SIZE];  //We will store the results here
  int hits = 0;
  int pos  = 0;
  int dif  = 0;
  int i,j,k;

  for(k=0;k<CA_RUNS;k++)
  {
    memset(result,'0',LAT_SIZE);
    for(i=0;i<LAT_SIZE;i++)
    {
      //We need this because the Lattice is just like a ring
      dif = i - RADIUS;
      if(dif < 0)
        pos = (LAT_SIZE-1)+dif;
      else
        pos = dif;
      hits = 0;
      //Find the hits
      for(j=0;j<(RADIUS*2+1);j++)
      {
        if(lat->cells[i]==rule.cells[j] || lat->cells[i]=='2')
          hits++;
        pos++;
        if(pos == LAT_SIZE)
          pos = 0;
      }
      if(hits == (RADIUS*2+1))
        result[i]=(MODE==2?rule.next:'1');
    }
    //Copy back the result
    memcpy(lat->cells,result,LAT_SIZE);
  }
}

void printLattice(FILE *stream, Lattice lat, int mode)
{
  int i;
  for(i=0;i<LAT_SIZE;i++)
  {
    if(mode==0)
      fprintf(stream,"%c",(lat.cells[i]=='0'?' ':'#'));
    else
      fprintf(stream,"%c",lat.cells[i]);
  }
}

