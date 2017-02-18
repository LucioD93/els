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

// Asigna el nombre del archivo a crear
char * filename(const char * path, const char * name){
  char * rt;
  char * n;
  rt = currentDir(path);
  n = (char *)malloc( strlen(name)+strlen(rt)+1);
  strcpy(n,name);  
  strcat(n,"_");
  strcat(n,rt);
  return n;
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
      
      fprintf(fp,"\n\n");
}
