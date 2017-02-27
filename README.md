# els
Proyecto 1 Sistemas de Operación ene-mar 2017

Sergio Medina - 09-11259
Lucio Mederos - 13-10856

Para recorrer los subdirectorios del directorio de trabajo se utiliza un proceso hijo por cada subdirectorio.
Dentro de cada proceso hijo se realizan las siguientes operaciones:
  - Se abre el subdirectorio con la llamada al sistema opendir()
  - Se cicla sobre los archivos abiertos por opendir y se prosigue segun su tipo
    - Si es un directorio se realiza una llamada recursiva para realizar el proceso con ese subdirectorio
    - Si es un archivo se toma su número de bytes en disco y se suma a un total
  - Al terminar el proceso recursivo se envían el total de bytes y de archivos (o directorios) encontrados al proceso padre haciendo uso de pipes
 Adicionalmente, cada proceso (padre e hijos) crean un archivo de salida en le directorio /tmp/ para mostrar los siguientes datos:
  - La ruta a cada subdirectorio
  - Los permisos correspondientes a cada subdirectorio
  - El nombre del usuario creador de cada subdirectorio
  - El nombre del grupo asignado a cada subdirectorio
  - Número total de archivos contenidos en cada subdirectorio
  - Número total de bytes ocupados por los archivos del subdirectorio (sin incluir subdirectorios)

  En caso de que se encuentre un archivo core, este es eliminado y se reporta su nombre.
