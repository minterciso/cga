#include "ca.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void createRandomLattices(Individual *ind)
{
  assert(ind!=NULL);
  int i,j;
  int count=0;
  int rnd=0;

  for(j=0;j<MAX_LATS;j++)
  {
    memset(ind->lat[j].cells,'0',LAT_SIZE);
    ind->lat[j].cells[LAT_SIZE]='\0';
#ifndef VALIDATE
    ind->lat[j].density = uniformDeviate(rand())*(LAT_SIZE-1); //Density is [0-LAT_SIZE)
#endif
#ifdef VALIDATE
    ind->lat[j].density = 90 + uniformDeviate(rand())*( 60 - 90); //Density is [0-LAT_SIZE)
#endif
    count=0;
    do
    {
      rnd = uniformDeviate(rand())*(LAT_SIZE-1); //Every cell has the same 1/(LAT_SIZE-1) prob of being setted
      if(ind->lat[j].cells[rnd]!='1')
      {
        ind->lat[j].cells[rnd]='1';
        count++;
      }
    }while(count <= ind->lat[j].density);      

  }
}

void createRandomRules(Individual *ind)
{
  int i,j;
  int rnd = 0;

  memset(ind->rule,'0',RULE_SIZE);
  ind->rule[RULE_SIZE-1]='\0';
  for(i=0;i<RULE_SIZE;i++)
  {
    rnd = uniformDeviate(rand())*MODE;
    switch(rnd)
    {
      case 0:ind->rule[i]='0';break;
      case 1:ind->rule[i]='1';break;
      case 2:ind->rule[i]='2';break;
    }
  }
}

void executeCA(Lattice *lat, char *rule, int ind_idx,int th_idx)
{
  int dif = 0;
  int pos = 0;
  int i,j,k;
  char res[LAT_SIZE];
  char bin[RADIUS*2+1+1];
  int idx=0;

  memset(bin,'0',RADIUS*2+1);

#ifdef DEBUG
  char fname[FNAME_SIZE];
  memset(fname,'\0',sizeof(FNAME));
  snprintf(fname,FNAME_SIZE-1,"logs/individual%03d-%03d.log",th_idx,ind_idx);
  FILE *fp = fopen(fname,"a+");
#endif
  for(i=0;i<CA_RUNS;i++)
  {
    memset(res,'0',LAT_SIZE);
#ifdef DEBUG
    fprintf(fp,"%3d:",i);
    printCA(fp,lat,1);
    fprintf(fp,"\n");
#endif
    for(j=0;j<LAT_SIZE;j++)
    {
      dif = j-RADIUS;
      if(dif < 0)
        pos = LAT_SIZE+dif;
      else
        pos = dif;
 
      for(k=0;k<RADIUS*2+1;k++)
      {
        //bin[k]= (lat->cells[pos]=='0'?'1':'0');
        bin[k]=lat->cells[pos];
        pos++;
        if(pos==LAT_SIZE)
          pos = 0;
      }
      bin[RADIUS*2+1]='\0';
      idx = bin2dec(bin,RADIUS*2+1);
      if(idx>=0 && idx<LAT_SIZE)
        res[j] = rule[idx];
      else
      {
        fprintf(stderr,"\nSOMETHING IS VERY WRONG!(%s)idx=%d th_idx=%d id_idx=%d\n",bin,idx,th_idx,ind_idx);
        abort();
      }
      memset(bin,'0',RADIUS*2+1);
    }
    if(memcmp(lat->cells,res,LAT_SIZE)==0) break; //If we hang...stop
    memcpy(lat->cells,res,LAT_SIZE);
  }
#ifdef DEBUG
  fclose(fp);
#endif
}

void printCA(FILE *stream, Lattice *lat, int mode)
{
  int i;
  for(i=0;i<LAT_SIZE;i++)
  {
    if(mode==0)
      fprintf(stream,"%c", (lat->cells[i]=='0'?'#':' ') );
    else if(mode==1)
      fprintf(stream,"%c", (lat->cells[i]=='0'?' ':'#') );
    else
      fprintf(stream,"%c",lat->cells[i]);
  }
}

