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
  char * pFile = (char*) malloc(strlen(name)+32);
  strcpy(pFile,name);
  strcat(pFile,"-");
  /* AQUI DEBERIAS ASEGURARTE DE QUE SE PONGA EL DIRECTORIO ACTUAL AL NOMBRE DEL ARCHIVO PADRE*/
  strcat(pFile,currentDir(cwd));
  FILE * fp;
  if ((fp = fopen(pFile,"w"))== NULL){
    printf("Error! No se ha podido abrir archivo de salida\n");
    exit(1);
  }

  pid_t childsPids[directorieCount];
  int i;
  int fd_array[directorieCount][2];

  for (i = 0; i < directorieCount; i++) {
    if (pipe(fd_array[i]) == -1){
      printf("Error! No se pudo crear pipe\n");
    }
  }

  report rep;
  int nbytes;
  char readbuffer[80];
  for (i = 0; i < directorieCount; i++) {
    if ((childsPids[i] = fork()) == 0) {
      //Child process closes up input side of pipe
      close(fd_array[i][0]);

      processDirectory(directories[i], name, &rep);
//      printf("values: %s\n", repString(&rep));
      char* rtn = repString(&rep);
      write(fd_array[i][1], rtn, (strlen(rtn)+1));
      exit(0);
    } else if (childsPids[i] < 0) {
      printf("Error! No se pudo crear proceso\n");
      exit(1);
    } else {
      // Parent process closes up output side of pipe
      close(fd_array[i][1]);

      nbytes = read(fd_array[i][0], readbuffer, sizeof(readbuffer));
      // printf("Probando %s\n", readbuffer);
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

  struct stat childStat;
  char * fullpathD[directorieCount];

  for (i = 0; i < directorieCount; i++) {
    fullpathD[i] = (char*) malloc(strlen(cwd)+32);
    strcpy(fullpathD[i],cwd);
    strcat(fullpathD[i],directories[i]);
    if(stat(directories[i], &childStat)) {
      fprintf(fp,"Error para abrir el directorio actual\n");
      exit(1);
    }
    fprintf(fp,"%s ", fullpathD[i]);
    printPermissions(childStat,fp);
  }

  fclose(fp);
  for (i = 0; i < directorieCount; i++) {
    free(directories[i]);
    free(fullpathD[i]);
  }
}
