#ifndef functions
#define functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void verificarArgumentos(int);
void sigintHandler(int);
char * currentDir(const char *);
char * filename(const char * , const char * );
void addSlash(char *);
void printPermissions(struct stat, FILE *);

#endif