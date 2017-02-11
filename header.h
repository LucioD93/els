#ifndef els
#define els
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void sigintHandler(int);
void addSlash(char*);
void printPermissions(struct stat);

#endif
