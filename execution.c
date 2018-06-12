#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>  
#include <grp.h> 
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include "execution.h"

//FUNCION QUE IMPRIME LA INFORMACION DE UN ARCHIVO 
//ACCEDIENDO AL I-NODO DE UN ARCHIVO/DIRECTORIO
void ls(char*Nombre){
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id",getpid(),getppid());
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
	wait(&status); //EVITAR CONCURRENCIA 

}
/////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////
void cat(char*File){
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0){

		int TipoArchivo = FileType(File);
		//SI EL ARCHIVO ES UN DIRECTORIO
		if(TipoArchivo==1)
		{
			char Contenido[10024],c;
			FILE*Archivo;
			//ABRIMOS EL ARCHIVO 
			Archivo = fopen(File,"r");
			if(Archivo!=NULL){
				//LEEMOS EL CONTENIDO DEL ARCHIVO
				c = fgetc(Archivo); 
				while (c!=EOF){

					printf("%c",c);
					c = fgetc(Archivo);	
				}
				// CERRAMOS EL ARCHIVO 
				fclose(Archivo);
			}
			else{
				perror("ERROR AL ABRIR EL ARCHIVO");
				exit(1);
			}	
		}
		else{
			perror("NO SE PUDO ABRIR EL ARCHIVO/ EL ARCHIVO ES UN DIRECTORIO");
			exit(1);
		}
			
	}	
	wait(&status); //EVITAR CONCURRENCIA 		
}
/////////////////////////////////////////////////////////////////////////////////////
void conca(char*File,char*Directory,char*Res){
	int count=0;
	for (int k = 0; Res[k]!= '\0'; ++k)
	{
		Res[k]='\0';
	}
	//calculamos el largo del directorio y lo guardamos 
	for (int i = 0; Directory[i] != '\0'; ++i){
		Res[i]=Directory[i];
		count=count+1;
	}

	for (int j = 0; File[j] != '\0'; ++j,count++)
	{
			Res[count]=File[j];	
	}
	Res[count]='\0';	
	//return NOTHING
}
//////////////////////////////////////////////////////////////////////////
void cp(char*File1,char*File2){
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id",getpid(),getppid());
		
		if (File1!=NULL && File2!=NULL)
		{
			//VERIFICAMOS QUE EL ARCHIVO1 NO SEA UN DIRECTORIO
			int TipoArchivo;
			TipoArchivo = FileType(File1);

			if (TipoArchivo==0)
			{
				perror("EL PRIMER ARCHIVO NO PUEDE SER UN DIRECTORIO");
				exit(1);
			}
			////////////////////////////////////////////

			FILE *Archivo1,*Archivo2;
			char c;

			//ABRIMOS EL ARCHIVO1 PARA LEERLO
			Archivo1 = fopen(File1,"r");
			if (Archivo1 == NULL )
			{
				perror("No se puede abrir el primer archivo");
				exit(1);
			}
			else{
				//VERIFICAMOS SI EL ARCHIVO2 ES DE TIPO REGULAR O DIRECTORIO
				int TipoArchivo2 ;
				TipoArchivo2 = FileType(File2);
				//SI EL ARCHIVO2 ES UN DIRECTORIO CREAMOS UN ARCHIVO DENTRO DE ESE DIRECTORIO
				//CON EL MISMO NOMBRE QUE ARCHIVO1.
				if(TipoArchivo2 == 0){
					perror("ERROR ABRIR ARCHIVO");
					exit(1);
				}

				else{
					//ABRIMOS EL ARCHIVO2 PARA ESCRIBIR 
					Archivo2= fopen(File2,"w");
				}

				if(Archivo2==NULL){
					//creamos el archivo 2 en el directorio
					//char Path[10024];
					int filedescriptor1 = open(File2,O_CREAT);
					if (filedescriptor1 < 0)
					{
						perror("NO SE PUDO CREAR EL NUEVO ARCHIVO");
						exit(1);

					}
					Archivo2 = fopen(File2,"w");
				}


				
				// LEEMOS EL CONTENIDO DEL PRIMER ARCHIVO Y LO COPIAMOS EN EL SEGUNDO ARCHIVO //

				//LEEMOS CARACTER
				c = fgetc(Archivo1);
				while(c!=EOF){
					//COPIAMOS CARACTER LEIDO EN EL ARCHIVO 2
					fputc(c,Archivo2);
					c = fgetc(Archivo1);
				} 
				printf("El contenido del archivo %s se ha copiado al archivo %s",File1,File2 );
				//CERRAMOS LOS ARCHIVOS
				fclose(Archivo1);
				fclose(Archivo2);
				
			}
		}
		else{
			perror("ERROR: INTRODUZCA LOS ARCHIVOS");
			exit(1);
		}

	}
	wait(&status); //EVITAR CONCURRENCIA 

}
//////////////////////////////////////////////////////////////////////////////////////////
void cp2(char*File1,char*File2,char*parameter1){
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id",getpid(),getppid());
		
		if (File1!=NULL && File2!=NULL)
		{
			//VERIFICAMOS QUE EL ARCHIVO1 NO SEA UN DIRECTORIO
			int TipoArchivo;
			TipoArchivo = FileType(File1);

			if (TipoArchivo==0)
			{
				perror("EL PRIMER ARCHIVO NO PUEDE SER UN DIRECTORIO");
				exit(1);
			}
			////////////////////////////////////////////

			FILE *Archivo1,*Archivo2;
			char c;

			//ABRIMOS EL ARCHIVO1 PARA LEERLO
			Archivo1 = fopen(File1,"r");
			if (Archivo1 == NULL )
			{
				perror("No se puede abrir el primer archivo");
				exit(1);
			}
			else{

				char Path[10024];

				conca(parameter1,File2,Path);

				int filedescriptor2 = open(Path,O_RDWR | O_CREAT | O_TRUNC,0777);
				if (filedescriptor2 < 0)
				{
					perror("NO SE PUDO CREAR EL NUEVO ARCHIVO");
					exit(1);
				}
				Archivo2 = fopen(Path,"w");
				
				// LEEMOS EL CONTENIDO DEL PRIMER ARCHIVO Y LO COPIAMOS EN EL SEGUNDO ARCHIVO //

				//LEEMOS CARACTER
				c = fgetc(Archivo1);
				while(c!=EOF){
					//COPIAMOS CARACTER LEIDO EN EL ARCHIVO 2
					fputc(c,Archivo2);
					c = fgetc(Archivo1);
				} 
				printf("El contenido del archivo %s se ha copiado al archivo %s",File1,File2 );
				//CERRAMOS LOS ARCHIVOS
				fclose(Archivo1);
				fclose(Archivo2);
				
			}
		}
		else{
			perror("ERROR: INTRODUZCA LOS ARCHIVOS");
			exit(1);
		}

	}
	wait(&status); //EVITAR CONCURRENCIA 

}
//////////////////////////////////////////////////////////////////////////////////////////
void mv(char*File1,char*File2){
	//CREAMOS PROCESO HIJO CORRESPONDIENTE//
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id",getpid(),getppid());
		
		if (File1!=NULL && File2!=NULL)
		{
			//VERIFICAMOS QUE EL ARCHIVO1 NO SEA UN DIRECTORIO
			int TipoArchivo;
			TipoArchivo = FileType(File1);

			if (TipoArchivo==0)
			{
				perror("EL PRIMER ARCHIVO NO PUEDE SER UN DIRECTORIO");
				exit(1);
			}
			////////////////////////////////////////////

			//VERIFICAMOS SI EL ARCHIVO2 ES DE TIPO REGULAR O DIRECTORIO
			int TipoArchivo2 ;
			TipoArchivo2 = FileType(File2);
			//SI EL ARCHIVO2 ES UN DIRECTORIO MOVEMOS EL ARCHIVO1 A ESE DIRECTORIO
			if(TipoArchivo2 == 0){
				char Path[10024];
				sprintf(Path,File1,File2);
				int filedescriptor = open(Path,O_APPEND);
				if (filedescriptor < 0)
				{
					perror("NO SE PUDO MOVER EL ARCHIVO");
					exit(1);
				}
				int renombrar = rename(File1,Path);
				if (renombrar<0)
				{
					perror("NO SE PUDO MOVER EL ARCHIVO");
					exit(1);
				}
			}
			
			else{

				int renombrar2 = rename(File1,File2);
				if (renombrar2 < 0)
				{
					perror("NO SE PUDO MOVER EL ARCHIVO");
					exit(1);
				}
			}
				
			
		}
		else{
			perror("ERROR: INTRODUZCA LOS ARCHIVOS CORRECTAMENTE");
			exit(1);
		}

	}
	wait(&status); //EVITAR CONCURRENCIA 

}
///////////////////////////////////////////////////////////////////////////////////////////
void rm(char*File){

	//CREAMOS PROCESO
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id\n",getpid(),getppid());

		if (File!=NULL)
		{
			//VERIFICAMOS SI EL ARCHIVO ES REGULAR O DIRECTORIO
			int TipoArchivo = FileType(File);
			//SI EL ARCHIVO ES UN DIRECTORIO EL PROGRAMA DA ERROR
			if (TipoArchivo == 0)
			{
				perror("El Archivo no puede ser un Directorio");
				exit(1);
			}
			else
			{
				//SI EL ARCHIVO ES REGULAR REMOVEMOS EL ARCHIVO
				int eliminar = remove(File);
				if (eliminar < 0)
				{
					perror("ERROR AL INTENTAR ELIMINAR EL ARCHIVO");
					exit(1);
				}
				else{
					printf("EL archivo %s ha sido removido exitosamente del directorio\n",File);
				}
			}
		}
		else
		{
			perror("ERROR AL INTENTAR ABRIR EL ARCHIVO");
			exit(1);
		}		
			

	}

	wait(&status); //EVITAR CONCURRENCIA 
}
/////////////////////////////////////////////////////////////////////////
void mkDir(char*File){
	//CREAMOS PROCESO//
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id",getpid(),getppid());
 
		//VERIFICAMOS PRIMERO SI EL PATH PASADO COMO PARAMETRO ES UN DIRECTORIO EXISTENTE
		int r=chdir(File);
		//SI EL DIRECTORIO YA EXISTE, ENTONCES ES UN ERROR 
		if (r==0){
			perror("EL DIRECTORIO QUE INTENTA CREAR YA EXISTE");
		}
		//SI EL DIRECTORIO NO EXISTE SE CREA
		else if (errno==ENOENT){
				mkdir(File,S_IRWXU);
				printf("El directorio %s ha sido creado exitosamente \n",File );	
		}
			
	}

	wait(&status); //EVITAR CONCURRENCIA 
}
///////////////////////////////////////////////////////////////////////////////
void rmDir(char*File){
	//CREAMOS PROCESO HIJO CORRESPONDIENTE 
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{


		//VERIFICAMOS SI EL ARCHIVO INGRESADO ES DE TIPO REGULAR O DIRECTORIO
		int TipoArchivo = FileType(File);
		//SI EL ARCHIVO INGRESADO ES DE TIPO REGULAR ENTONCES DA ERROR//
		if (TipoArchivo != 0)
		{
			perror("EL ARCHIVO INGRESADO NO PUEDE SER REGULAR");
			exit(1);
		}

		DIR *directory;
		struct dirent *file;
		int count = 0;
		if ((directory = opendir(File))!=NULL)
		{
		 	//Verificamos que no exista ningun archivo en el directorio

		 	//if ((file = readdir(directory)) != NULL)
		 	while ((file = readdir(directory))!=NULL)
		 	{
		 		if (++count > 2)
		 		{
		 			break;
		 		}
		 	}
		 	//SI EXISTE AL MENOS UN ARCHIVO EN EL DIRECTORIO ENTONCES ES ERROR 
		 	if (count>2)
		 	{
		 		perror("EL DIRECTORIO QUE DESEA ELIMINAR POSEE AL MENOS UN ARCHIVO");
		 	 	exit(1);
		 	}

		 	//SI NO EXISTE ARCHIVO EN EL DIRECTORIO ENTONCES ES ELIMINADO //
		 	else
		 	{
		 		
		 		int eliminarDir = rmdir(File);
		 
		 		if (eliminarDir < 0)
		 		{
		 			perror("ERROR AL ELIMINAR DIRECTORIO");
		 			exit(1);
		 		}
		 		printf("El directorio %s ha sido eliminado exitosamente \n",File );
		 	}  
		} 
		else 
		{
			perror("ERROR OPENING DIRECTORY");
			exit(1);
		} 
			

	}

	wait(&status); //EVITAR CONCURRENCIA 
}
///////////////////////////////////////////////////////////////////////////////////
void find(char*Cadena,char*Path){
	int status;
	int childpid;
	childpid = fork();
	if (childpid <0)
	{
		perror("ERROR AL EJECUTAR FORK");
		exit(1);
	}
	else if (childpid == 0)
	{

		//printf("soy el hijo con pid %Id, y mi padre tiene pid %Id",getpid(),getppid());


		DIR *directory;
		struct dirent *actual;
		if ((directory=opendir(Path))!=NULL)
		{
			//IMPRIMIMOS TODOS LOS ARCHIVOS EN EL DIRECTORIO
			while((actual = readdir(directory))!=NULL){
				//if (strstr(actual->d_name,Cadena)!=NULL)
				//{
				//	printf("%s\n",actual->d_name);
				//}
				/*if ((strcmp(actual->d_name,".")) != 0 )
				{
					printf("SON DIFERENTES\n");
				}
				else{
					printf("SON IGUALES\n");
				}
				*/
				printf("%s\n",actual->d_name);
			}
		}
		else{

			perror("ERROR OPENING DIRECTORY");
			exit(1);

		}
			

	}

	wait(&status); //EVITAR CONCURRENCIA 
	

}


