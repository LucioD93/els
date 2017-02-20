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
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>

void verificarArgumentos(int);
void sigintHandler(int);
char * currentDir(const char *);
void addSlash(char *);
void printPermissions(struct stat, FILE *);
int isDirectory(char*);
int countDirectories(char*);
void processDirectory(char*);

#endif
