#include "header.h"

// Manejador de señales para evitar el cierre del programa
void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("\n Error: no se puede terminar usando Ctrl+C\n Continuando operacion\n");
    fflush(stdout);
}

// Funcion para agregar un slash al final de un string
void addSlash(char *address) {
  char last = address[strlen(address)-1];
  if (strcmp(&last,"/") != 0) {
    strcat(address,"/");
  }
}

// Funcion para imprimir los permisos de un stat
void printPermissions(struct stat fileStat) {
  printf("File Permissions: \t");
      printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
      printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
      printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
      printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
      printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
      printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
      printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
      printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
      printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
      printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
      printf("\n\n");
}
