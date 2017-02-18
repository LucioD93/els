#include "functions.h"

int main(int argc, char const *argv[]) {
  // Se verifica que se dio el nombre del archivo destino
  verificarArgumentos(argc);
  // Se previene la interrupcion CTRL+C
  signal(SIGINT, sigintHandler);

  char * name;
  int mainPid = getpid();

  char *cp;
  char cwd[BUFSIZ];

  if(!(cp = getcwd(cwd, BUFSIZ))) {
    printf("Error obteniendo directorio actual\n");
    exit(1);
  }

  name = filename(cwd, argv[1]); 
  FILE * fp;
  if ((fp = fopen(name,"w"))== NULL){
    printf("Error!\n");
    exit(1);
  }

  addSlash(cwd);
  fprintf(fp,"CWD: %s\n", cwd);
  fprintf(fp,"Pid: %d\n", mainPid);
  struct stat cwdStat;

  if(stat(cwd, &cwdStat)) {
    fprintf(fp,"Error para abrir el directorio actual\n");
    exit(1);
  }
  printPermissions(cwdStat,fp);
}
