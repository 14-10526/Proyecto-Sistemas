#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>  
#include <grp.h> 
#include <time.h>
#include "execution.h"

//FUNCION QUE IMPRIME LA INFORMACION DE UN ARCHIVO 
//ACCEDIENDO AL I-NODO DE UN ARCHIVO/DIRECTORIO
void ls(char*Nombre){
	if (Nombre!=NULL){
		printf("uso: %s <nombre_archivo>\n",Nombre);
		//exit(0);
	}

	struct stat inodo;
	int x = stat(Nombre,&inodo);

	if (x!= 0 ){
		perror("NO SE PUDO OBTENER EL ARCHIVO");
		exit(1);
	}

	printf("Numero de dispositivos: %d\n",inodo.st_dev);
	printf("Numero de inodo: %d\n",inodo.st_ino );
	printf("Modo: 0x%X\n",inodo.st_nlink);
	printf("Pertenece a: %s\n", getpwuid(inodo.st_uid)->pw_name);
	printf("Del grupo: %s\n",getgrgid(inodo.st_gid)->gr_name );
	printf("EL archivo ocupa: %d bytes\n",inodo.st_size);
	printf("Accedido el: %s\n",asctime(localtime(&inodo.st_atime)) );

}
//FUNCION QUE VERIFICA SI EL ARCHIVO INGRESADO ES REGULAR O ES DIRECTORIO
// DEVUELVE 0 SI ES DIRECTORIO
// DEVUELVE 1 SI ES UN ARCHIVO REGULAR 
int FileType(char*File){
	struct stat path_stat;
	int x = stat(File,&path_stat);

	if (x!=0)
	{
		perror("NO SE PUDO OBTENER EL ARCHIVO");
		exit(1);
	}

	int TipoArchivo = S_ISREG(path_stat.st_mode);
	return TipoArchivo;
	//printf("NUMERO QUE DEVUELVE %d",tipoArchivo);	
}

void cat(char*File){
	int TipoArchivo = FileType(File);
	//SI EL ARCHIVO ES UN DIRECTORIO
	if(TipoArchivo==1)
	{
		char Contenido[10024];
		FILE*Archivo;
		Archivo = fopen(File,"r");
		if(Archivo!=NULL){
			while(fscanf(Archivo,"%s",Contenido)!=EOF){
				printf("%s",Contenido);	
			}
			fclose(Archivo);
	}
	else{
		perror("NO SE PUDO ABRIR EL ARCHIVO");
		exit(1);
	}
		}


/*	struct stat path_stat;
	int x = stat(File,&path_stat);

	if (x!=0)
	{
		perror("NO SE PUDO OBTENER EL ARCHIVO");
		exit(1);
	}

	int tipoArchivo = S_ISREG(path_stat.st_mode);
	printf("NUMERO QUE DEVUELVE %d",tipoArchivo);
*/
}