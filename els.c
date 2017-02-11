#include "header.h"

int main(int argc, char const *argv[]) {
  signal(SIGINT, sigintHandler);
  int mainPid = getpid();
  char *cp;
  char cwd[BUFSIZ];
  if(!(cp = getcwd(cwd, BUFSIZ))) {
    printf("Error obteniendo directorio actual\n");
    exit(1);
  }
  addSlash(cwd);
  printf("CWD: %s\n", cwd);
  printf("Pid: %d\n", mainPid);
  struct stat cwdStat;
  if(stat(cwd, &cwdStat)) {
    printf("Error para abrir el directorio actual\n");
    exit(1);
  }
  printPermissions(cwdStat);
}
