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
#include <pwd.h>
#include <grp.h>

// Estructura a utilizar para reportar resultado a proceso padre
typedef struct {
  int fileCounter;
  int byteCounter;
} report;

void verificarArgumentos(int);
void sigintHandler(int);
void addSlash(char *);
void printPermissions(struct stat, FILE*);
int isDirectory(char*);
int countDirectories(char*);
void processDirectory(char*, FILE*, report*);
char * repString(report *);
report repValues(char *);
int isCore(const char *);

#endif
