#ifndef errmmry_h
#define errmmry_h

extern int varcount;
int *varadresses[20];
extern char *errorstring; 
int addfree(int* adr);
int freeall();
int writelog();
#endif
