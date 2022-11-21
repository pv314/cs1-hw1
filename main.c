      /* COP 3502C Assignment 1 
This program is written by: Heng Hong */ 

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

typedef struct monster {
  char *name;
  char *element;
  int population;
} monster;

typedef struct region {
  char *name;
  int nmonsters;
  int total_population;
  monster **monsters;
} region;

typedef struct itinerary {
  int nregions;
  region **regions;
  int captures;
} itinerary;

typedef struct trainer {
  char *name;
  itinerary *visits;
} trainer;

monster *makeMonster(char *name, char *element, int population) {

  //malloc all the member as needed
  monster *makeM = malloc(sizeof(monster));
  makeM->name = malloc((strlen(name) + 1) * sizeof(char));
  makeM->element = malloc((strlen(element) + 1) * sizeof(char));

  //sign the name,element and population to the DMA box
  strcpy(makeM->name, name);
  strcpy(makeM->element, element);
  makeM->population = population;

  return makeM;
}

monster **readMonsters(int *monsterCount) 
{
  char monsterTemp[51];
  char monsterWord[51];
  char elementTemp[51];
  int population;

  // read in monsterCount and word 
  scanf("%d", monsterCount); 
  scanf("%s", monsterWord); 
 
  //requesting the memory for monsterList
  monster **monsterList = malloc(*monsterCount * sizeof(monster *));

  for (int i = 0; i < *monsterCount; i++) 
  {
    //read in name element and population
    scanf("%s", monsterTemp);
    scanf("%s", elementTemp);
    scanf("%d", &population);

    //take user input and sign whole box into list 
    monsterList[i] = makeMonster(monsterTemp, elementTemp, population);
  }

  return monsterList;
}

region **readRegions(int *countRegions, monster **monsterList,int monsterCount) 
{ 
  char regionTemp[51];
  char regionNameTemp[51];
  char monsterTemp[51];
  char monsterName[51];
  int numMonster;
  int res;

  //read in #of regions and word
  scanf("%d %s", countRegions, regionTemp); 

  // allocate for #of regions from input
  region **regionList = malloc(*countRegions * sizeof(region *));
  
  // loop for each region
  for (int i = 0; i < *countRegions; i++) 
  {
    //locate the memory for each region and read in # region name, #of monster,and word
    regionList[i] = malloc(sizeof(region));   
    scanf("%s", regionNameTemp);              
    scanf("%d %s", &numMonster, monsterTemp); 

    //allacate and store monster name to MDA box
    regionList[i]->name = malloc((strlen(regionNameTemp) + 1) * sizeof(char));
    strcpy(regionList[i]->name, regionNameTemp);

    //read from input and set into DMA box
    regionList[i]->nmonsters = numMonster;

    //set pop to 0 and ready for further calaulate.
    int total_population = 0;

    // locate the memory for monster
    regionList[i]->monsters = malloc(numMonster * sizeof(monster *));
    
    // loop each  monster in each region
    for (int j = 0; j < numMonster; j++) 
    {
      //read the name prepare to match
      scanf("%s", monsterName);

      // ready to compare from mousterCount and monster in region
      for (int k = 0; k < monsterCount; k++) 
      {
        res = strcmp(monsterList[k]->name, monsterName);
        if (res == 0) 
        {
          // match, copy the individual monster frome monsterlist into regionlist
          regionList[i]->monsters[j] = monsterList[k];

          //add up population when each match
          total_population += regionList[i]->monsters[j]->population;         
        }                    
      }      
    } 
    //sign the static variable to DMA box
    regionList[i]->total_population = total_population;  
  }

  return regionList;
}

trainer *loadTrainers(int *trainerCount, region **regionList,int countRegions) 
{
  char regionTemp[51];
  char tranersName[51];
  char capturesTemp[51];
  char nregionsTemp[51];

  //read in #of traner and trainser word
  scanf("%d", trainerCount); 
  scanf("%s", regionTemp);   
  
  //allacation necessary memory for trainer
  trainer *trainersList = malloc(*trainerCount * sizeof(trainer));

  for (int i = 0; i < *trainerCount; i++) {

    //read in trainer name
    scanf("%s", tranersName);

    //mmalloc and getting the name set it to trainerslist
    trainersList[i].name = malloc((strlen(tranersName) + 1) * sizeof(char));
    strcpy(trainersList[i].name, tranersName);

    //malloc visits for itinerary
    itinerary *visits = malloc(sizeof(itinerary));

    //read in data for capture and #of regions.
    scanf("%d %s", &visits->captures, capturesTemp);
    scanf("%d %s", &visits->nregions, nregionsTemp);

    //malloc for each region visited
    region **tRegions = malloc(visits->nregions * sizeof(region *));

    //loop for # of region from readin
    for (int j = 0; j < visits->nregions; j++) 
    {
      //read in the region name prepare to match
      scanf("%s", regionTemp);

      //compare each region visited to whole regionslist
      for (int k = 0; k < countRegions; k++) 
      {
        int res = strcmp(regionList[k]->name, regionTemp);
        if (res == 0) 
        {
          //if match ,sign the whole box to temp regeions
          tRegions[j] = regionList[k];
          break;
        }
      }
    }
    //set the tmep regions final to trainerlist.
    visits->regions = tRegions;
    trainersList[i].visits = visits;
  }

  return trainersList;
}

void processInputs(monster **monsterList, int monsterCount, region **regionList,int regionCount, trainer *trainerList, int trainerCount) 
{
  // for each field trainer go to
  for (int i = 0; i < trainerCount; i++)
  {
    //output the trainer name
    printf("%s\n", trainerList[i].name);
   
    for (int k = 0; k < trainerList[i].visits->nregions; k++)//3
    {
      //show regions name
      printf("%s\n", trainerList[i].visits->regions[k]->name);

      //collect the total population and captures in particular region for trainer.
      int totalPop = trainerList[i].visits->regions[k]->total_population; 
      int captures = trainerList[i].visits->captures;

      //number of monster in particular region
      for(int m = 0; m<trainerList[i].visits->regions[k]->nmonsters;m++)
      {

        //collect the # of monster for trainer in certain region.
        int monsterPop =trainerList[i].visits->regions[k]->monsters[m]->population;

        //calculate the amount          
        double mcount = (double) (monsterPop*1.0 / totalPop) * captures;

        mcount = round(mcount);

        //casting to int
        int Nmcount = (int)mcount;
          if(Nmcount != 0)
          {
            //format final print
            printf("%d#%s\n",Nmcount,trainerList[i].visits->regions[k]->monsters[m]->name);
          }       
      }        
    }

      //separate blank line between each trainer except the last one
      if(i != trainerCount -1)
      {
      printf("\n");
      }   
  }
}
void releaseMemory(monster **monsterList, int monsterCount, region **regionList,int regionCount, trainer *trainerList, int trainerCount) {
  
  //free monsterCount malloc from inside to outside.
  for (int i = 0; i < monsterCount; i++) {
    free(monsterList[i]->name);
    free(monsterList[i]->element);
    free(monsterList[i]);
  }
  free(monsterList);

  //free regionCount malloc from inside to outside.
  for (int j = 0; j < regionCount; j++) {
    free(regionList[j]->monsters);
    free(regionList[j]->name);
    free(regionList[j]);
  }
  free(regionList);

  //free trainerCount malloc from inside to outside.
  for (int k = 0; k < trainerCount; k++) {
    free(trainerList[k].name);
    free(trainerList[k].visits->regions);
    free(trainerList[k].visits);
  }
  free(trainerList);
}

int main() {
  atexit(report_mem_leak);
  int monsterCount, regionCount, trainerCount;

  monster **monsterList = readMonsters(&monsterCount);
  region **regionList = readRegions(&regionCount, monsterList, monsterCount);
  trainer *trainerList = loadTrainers(&trainerCount, regionList, regionCount);

  processInputs(monsterList, monsterCount, regionList, regionCount, trainerList,trainerCount);
  releaseMemory(monsterList, monsterCount, regionList, regionCount,trainerList, trainerCount);

  return 0;
}