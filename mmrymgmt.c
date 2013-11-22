#include <stdio.h>
#include <stdlib.h>
#include "mmrymgmt.h"

int varcount=0; // Anzahl der zugewiesenen Speicherbereiche
/**
 * Fuegt den Pointer auf den spaeter zu befreienden Bereich in ein Array hinzu.
 *
 * @param  adr Der Pointer auf den Speicherbereich
 * @return Gibt 0 zurueck.
 */
int addfree(int* adr){
varadresses[varcount]=adr;
varcount++;
return 0;	
}
/**
 * Befreit alle in das Array bisher aufgenommenen Speicherbereiche.
 *
 * @return Gibt 0 zurueck.
 */
int freeall(){
int i;
for(i=0;i<=varcount;i++){
	free(varadresses[i]);
}
printf("Es wurden %d Variablen befreit \n",varcount);
return 0;
}
