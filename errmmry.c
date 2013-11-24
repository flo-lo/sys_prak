#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "errmmry.h"
#include <time.h>

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
return EXIT_SUCCESS;	
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
return EXIT_SUCCESS;
}
/**
 * Schreibt eine Fehlermeldung mit Timestamp in die Datei log.txt
 *
 * @return Gibt 0 zurueck.
 */
int writelog(){
FILE *logfile;
time_t result = time(NULL);
char *t = ctime(&result);
t[strlen(t)-1] = 0;
if ((logfile = fopen("log.txt","a"))==0){
  logfile = fopen("log.txt","w");
}
  char *errorstring=strdup(strerror(errno));
  fprintf(logfile,"%s --> ",t);
  fputs(errorstring,logfile);
  fprintf(logfile,"\n");
  close(logfile);
  free(errorstring);
  return EXIT_SUCCESS;
}
