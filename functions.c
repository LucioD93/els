#include "functions.h"

// Indica la forma correcta de usar el programa
 void verificarArgumentos(int cantidad){
    if (cantidad != 2){
        printf("Violacion de parametros, pruebe: \n");
        printf("./els <outpufile> \n");
        exit(1);
    }
}

// Manejador de señales para evitar el cierre del programa
void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("\n Error: no se puede terminar usando Ctrl+C\n Continuando operacion\n");
    fflush(stdout);
}

// Funcion para agregar un slash al final de un string
void addSlash(char * address) {
  char last = address[strlen(address)-1];
  if (strcmp(&last,"/") != 0) {
    strcat(address,"/");
  }
}

// Funcion para imprimir los permisos de un stat
void printPermissions(struct stat fileStat, FILE * fp) {
    struct passwd *pwd;
    struct group *grp;

    fprintf(fp, (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    fprintf(fp, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    fprintf(fp, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    fprintf(fp, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    fprintf(fp, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    fprintf(fp, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    fprintf(fp, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    fprintf(fp, (fileStat.st_mode & S_IROTH) ? "r" : "-");
    fprintf(fp, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    fprintf(fp, (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    if ((pwd = getpwuid(fileStat.st_uid))) {
      fprintf(fp, " %-8.8s", pwd -> pw_name);
    } else {
      fprintf(fp," %-8d", fileStat.st_uid);
    }
    if ((grp = getgrgid(fileStat.st_uid))) {
      fprintf(fp, " %-8.8s", grp -> gr_name);
    } else {
      fprintf(fp," %-8d", fileStat.st_gid);
    }
    // fprintf(fp,"%s", ctime((const time_t *)&fileStat.st_mtim));
    // fprintf(fp,"%s", ctime((const time_t *)&fileStat.st_atim));
}

// Funcion para determinar si un path es un directorio,
// retorna 0 si lo es, 1 si no lo es
int isDirectory(char* path) {
  if (path[0] == '.') {
    return 0;
  }
  struct stat statbuffer;
  if (stat(path, &statbuffer) != 0) {
      return 0;
  }
  return S_ISDIR(statbuffer.st_mode);
}

// Funcion para contar los subdirectorios dentro de un directorio
int countDirectories(char* path) {
  int counter = 0;
  DIR *dir;
  struct dirent *ep;
  dir = opendir(path);
  if (dir != NULL) {
    while ((ep = readdir(dir))) {
      if (isDirectory(ep -> d_name)) {
        counter += 1;
      }
    }
    (void) closedir(dir);
  } else {
    printf("Error! No se ha podido abrir directorio\n");
    exit(1);
  }
  return counter;

}

// Funcion para procesar un directorio contando sus archivos y bytes
void processDirectory(char* path, FILE* fp, report * rep) {
  printf("Recibo |%s|\n", path);
  struct dirent *ep;
  DIR *dir;
  char* file;
  int byteCounter = 0;
  int fileCounter = 0;
  if (!(dir = opendir(path))) {
    printf("Error! no se pudo abrir directorio %s\n", path);
    return;
  }
  if (dir) {
    while ((ep = readdir(dir))) {
      // filtrar directorios ocultos
      if (ep->d_name[0] != '.') {
        fileCounter++;
        file = (char*) malloc(sizeof(char)*(int)strlen(path));
        if (!file) {
          printf("Error! No se pudo reservar memoria\n");
          exit(1);
        }
        strcpy(file,path);
        strcat(file, "/");
        strcat(file, ep->d_name);
        if (isDirectory(file) ){
          // Se encontro un directorio
          printf("dir: |%s|->|%s|\n", path, file);
          processDirectory(file, fp, rep);
        } else if (file[0] != '.') {
          // Se encontro un archivo
            printf("file: |%s|->|%s|\n", path, file);
            struct stat statBuffer;
            stat(file, &statBuffer);
            byteCounter += (int)statBuffer.st_size;
        }
      }
    }
    (void) closedir(dir);
  } else {
    printf("Error! No se ha podido abrir directorio\n");
    exit(1);
  }

  printf("permisos de: |%s|\n", path);
  printf("---->|%s| archivos:%d bytes %d\n", path, fileCounter, byteCounter);
  struct stat statB;
  stat(path, &statB);
  fprintf(fp, "%s ", path);
  printPermissions(statB, fp);
  fprintf(fp, " %d %d\n", fileCounter, byteCounter);
  rep -> fileCounter += fileCounter;
  rep -> byteCounter += byteCounter;
}

void strreverse(char* begin, char* end) {
    char aux;
    while(end>begin){
        aux=*end, *end--=*begin, *begin++=aux;
    }
}

char* itoa(int value, char* str) {
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr=str;
    int sign;
    div_t res;
    // Take care of sign

    if ((sign=value) < 0) {
      value = -value;
    }
    // Conversion. Number is reversed.

    do {
        res = div(value,10);
        *wstr++ = num[res.rem];
    }while((value=res.quot));

    if(sign<0) *wstr++='-';
    *wstr='\0';
    // Reverse string
    strreverse(str,wstr-1);
        return str;
}

char * repString(report * rep){
  char * fC = (char*) malloc(32);
  char * bC = (char*) malloc(32);
  fC = itoa(rep->fileCounter,fC);
  bC = itoa(rep->byteCounter,bC);
  strcat(fC," ");
  strcat(fC,bC);
  return fC;

}

report repValues(char * rep){
  report arr;
  int i = 0;
  char * token;
  const char space[2] = " ";

  token = strtok(rep,space);

  while((token != NULL) && (i<2)){
    if (i == 0) {
      arr.fileCounter = atoi(token);
    } else {
      arr.byteCounter = atoi(token);
    }
    token = strtok(NULL,space);
    i++;
  }

  return arr;
}

int isCore(const char * name){
  const char * core = "core";
  return strncmp(name, core, 4);
}
