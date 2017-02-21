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

  addSlash(cwd);

  int directorieCount = countDirectories(cwd);
  printf("Numero de directorios %d\n", directorieCount);
  char* directories[directorieCount];
  int counter = 0;
  DIR *dir;
  struct dirent *ep;
  dir = opendir(cwd);
  if (dir) {
    while ((ep = readdir(dir))) {
      if (isDirectory(ep -> d_name)) {
        if ((directories[counter] = (char*) malloc(sizeof(char)*strlen(ep->d_name))) == NULL){
          exit(1);
        }
        strcpy(directories[counter], ep -> d_name);
        counter ++;
      } else {
      }
    }
    (void) closedir(dir);
  } else {
    printf("Error! No se ha podido abrir directorio\n");
    exit(1);
  }

  name = (char*)argv[1];
  FILE * fp;
  if ((fp = fopen(name,"w"))== NULL){
    printf("Error! No se ha podido abrir archivo de salida\n");
    exit(1);
  }

  pid_t childsPids[directorieCount];
  int i;
  report * rep;
  for (i = 0; i < directorieCount; i++) {
    if ((childsPids[i] = fork()) == 0) {
      printf("Hola soy %d hijo de %d\n", getpid(), getppid());
      processDirectory(directories[i], name, rep);
      exit(0);
    } else if (childsPids[i] < 0) {
      printf("Error! No se pudo crear proceso\n");
      exit(1);
    }
  }

  for (i = 0; i < directorieCount; i++) {
    waitpid(childsPids[i], (int*)1, 0);
  }

  fprintf(fp,"CWD: %s\n", cwd);
  fprintf(fp,"Pid: %d\n", mainPid);
  struct stat cwdStat;

  if(stat(cwd, &cwdStat)) {
    fprintf(fp,"Error para abrir el directorio actual\n");
    exit(1);
  }
  printPermissions(cwdStat,fp);
  fclose(fp);
  for (i = 0; i < directorieCount; i++) {
    free(directories[i]);
  }
}
