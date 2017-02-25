#include "functions.h"

int main(int argc, char const *argv[]) {
  // Se verifica que se dio el nombre del archivo destino
  verificarArgumentos(argc);
  // Se previene la interrupcion CTRL+C
  signal(SIGINT, sigintHandler);

  char * name = (char*) malloc(sizeof(char)*5);
  strcpy(name,"/tmp/");
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
        if ((directories[counter] = (char*) malloc(sizeof(char)*strlen(ep->d_name))) == NULL) {
          exit(1);
        }
        strcpy(directories[counter], ep -> d_name);
        counter ++;
      } else {
        if(isCore(ep -> d_name) == 0) {
          if (remove(ep -> d_name) == 0) {
            printf("Archivo core: %s eliminado.\n", ep->d_name);
          } else {
            printf("Imposible eliminar archivo %s\n", ep->d_name);
            perror("Error");
          }
        }
      }
    }
    (void) closedir(dir);
  } else {
    printf("Error! No se ha podido abrir directorio\n");
    exit(1);
  }

  //name = (char*)argv[1];
  strcat(name,argv[1]);
  FILE * fp;
  if ((fp = fopen(name,"w"))== NULL) {
    printf("Error! No se ha podido abrir archivo de salida\n");
    exit(1);
  }

  pid_t childsPids[directorieCount];
  int i;
  int fd_array[directorieCount][2];

  for (i = 0; i < directorieCount; i++) {
    if (pipe(fd_array[i]) == -1) {
      printf("Error! No se pudo crear pipe\n");
    }
  }
  char* outputfile;
  FILE* fpp;
  report rep;
  report rep_array[directorieCount];
  int nbytes;
  char readbuffer[80];
  for (i = 0; i < directorieCount; i++) {
    if ((childsPids[i] = fork()) == 0) {
      //Child process closes up input side of pipe
      close(fd_array[i][0]);
      if (!(outputfile = (char*) malloc(sizeof(char)*(strlen(name)+strlen(directories[i]+1))))) {
        printf("Error! no se ha podido reservar memoria\n");
        exit(1);
      }
      strcpy(outputfile, name);
      strcat(outputfile, "-");
      strcat(outputfile, directories[i]);
      //printf("Archivo salida |%d| |%s|\n", getpid(), outputfile);
      if ((fpp = fopen(outputfile,"w"))== NULL){
        printf("Error! No se ha podido abrir archivo de salida\n");
        exit(1);
      }
      processDirectory(directories[i], fpp, &rep);
      fclose(fpp);
//      printf("values: %s\n", repString(&rep));
      char* rtn = repString(&rep);
      printf("      %s->%s\n", outputfile, rtn);
      write(fd_array[i][1], rtn, (strlen(rtn)+1));
      exit(0);
    } else if (childsPids[i] < 0) {
      printf("Error! No se pudo crear proceso\n");
      exit(1);
    } else {
      // Parent process closes up output side of pipe
      close(fd_array[i][1]);

      nbytes = read(fd_array[i][0], readbuffer, sizeof(readbuffer));
      rep_array[i]= repValues(readbuffer);
    }
  }

  for (i = 0; i < directorieCount; i++) {
    waitpid(childsPids[i], (int*)1, 0);
  }

  report finalRep;
  finalRep.fileCounter = 0;
  finalRep.byteCounter = 0;
  for (i = 0; i < directorieCount; i++) {
    finalRep.fileCounter += rep_array[i].fileCounter;
    finalRep.byteCounter += rep_array[i].byteCounter;
  }

  // fprintf(fp,"CWD: %s\n", cwd);
  // fprintf(fp,"Pid: %d\n", mainPid);
  struct stat cwdStat;

  if(stat(cwd, &cwdStat)) {
    fprintf(fp,"Error para abrir el directorio actual\n");
    exit(1);
  }
  printPermissions(cwdStat,fp);
  fprintf(fp," %d %d\n", finalRep.fileCounter, finalRep.byteCounter);

  struct stat childStat;
  char * fullpathD[directorieCount];

  for (i = 0; i < directorieCount; i++) {
    fullpathD[i] = (char*) malloc(strlen(cwd));
    strcpy(fullpathD[i],cwd);
    strcat(fullpathD[i],directories[i]);
    if(stat(directories[i], &childStat)) {
      fprintf(fp,"Error para abrir el directorio actual\n");
      exit(1);
    }
    fprintf(fp,"%s ", fullpathD[i]);
    printPermissions(childStat,fp);
    fprintf(fp," %d %d\n", rep_array[i].fileCounter, rep_array[i].byteCounter);
  }

  fclose(fp);
  for (i = 0; i < directorieCount; i++) {
    free(directories[i]);
    free(fullpathD[i]);
  }
}
