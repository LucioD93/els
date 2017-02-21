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

// Devuelve el directorio actual
char * currentDir(const char * fullpath){
  char * token;
  char * dir;
  size_t len;

  token = strrchr(fullpath,'/');

  if (token == NULL){
     printf("Error\n");
     exit(1);
  }
  len = strlen(token);
  dir = (char *)malloc(len);
  memcpy(dir, token+1, len);

  return dir;
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
  //printf("File Permissions: \t");
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
      fprintf(fp," %-8d", fileStat.st_uid);
      fprintf(fp,"%-8d", fileStat.st_gid);
      fprintf(fp,"%s", ctime((const time_t *)&fileStat.st_mtim));
      fprintf(fp,"%s", ctime((const time_t *)&fileStat.st_atim));
      //printf("Number of links %ld", (long)fileStat.st_nlink);
}

// Funcion para determinar si un path es un directorio,
// retorna 0 si lo es, 1 si no lo es
int isDirectory(char* path) {
  if (path[0] == '.') {
    return 0;
  }
  struct stat statbuffer;
  if (stat(path, &statbuffer) != 0) {
      printf("Error! No se ha podido aplicar stat a %s\n", path);
      exit(1);
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
void processDirectory(char* path, char* outpufile, report * rep) {
  printf("Soy %d y me toca %s\n", getpid(), path);
  FILE *fp;
  strcat(outpufile, "-");
  strcat(outpufile, path);
  printf("Soy %d, archivo de salida: %s\n", getpid(), outpufile);

  struct dirent *ep;
  DIR *dir;

  dir = opendir(path);
  if (dir) {

    (void) closedir(dir);
  } else {
    printf("Error! No se ha podido abrir directorio\n");
    exit(1);
  }


  if ((fp = fopen(outpufile,"w"))== NULL){
    printf("Error! No se ha podido abrir archivo de salida\n");
    exit(1);
  }
  // Escribir en archivo de salida fp
  struct stat statBuffer;
  stat(path, &statBuffer);
  printPermissions(statBuffer, fp);
  fclose(fp);
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
   
    if ((sign=value) < 0){
      value = -value;
    } 
    // Conversion. Number is reversed.
   
    do {   
        res = div(value,10); 
        *wstr++ = num[res.rem];
    }while(value=res.quot);
   
    if(sign<0) *wstr++='-';
    *wstr='\0';
    // Reverse string  
    strreverse(str,wstr-1);
        return str;
}

char * repString(report * rep){
  char * fC;
  char * bC;
  fC = itoa(rep->fileCounter,fC);
  bC = itoa(rep->byteCounter,bC);
  strcat(fC," ");
  strcat(fC,bC);
  return fC;

}