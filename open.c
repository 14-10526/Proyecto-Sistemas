#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <signal.h>
#include "execution.h"
# define MAXCHAR  1025
main(){

	//////////////////////////////////////////////////////////////
	/////////////////////////////
	//Abrimos el Directorio Raiz
	char  Directory_PATH [10025];
	printf("c :\\>prompt$");
	scanf("%1024s",Directory_PATH );
	DIR *RootDirectory;
	RootDirectory = opendir(Directory_PATH);
	//////////////////////////////
	///// RUTINA MANEJADORA PARA IGNORAR CTRL+C
	void(*oldHandler)(); //guardamos la direccion del manejador
	oldHandler = signal(SIGINT,SIG_IGN); // se ignora ctrl+c

	//////////////////////////////////
	char Prompt_Command [1025];
	const char tab[2] = "-";
	const char sal[2] = "\n";
	//const char Whitespace[2] = "\t";
	char *Command;
	char *parameter1;
	char *parameter2;
	char p1[10025];
	char p2[10025];

	
	//FUNCION QUE RECIBE LOS COMANDOS DEL COMMAND PROMPT
	void command(char *Prompt_Command){
		printf("\nc :\\>prompt$");
		scanf("%1024s",Prompt_Command);
	}

	//FUNCION QUE LEE EL COMANDO Y LO EJECUTA 
	void Execute_Command(char *Prompt_Command,char*Directory_PATH){	
		if (Prompt_Command != NULL){


			Command = strtok(Prompt_Command,tab);

			
			if (strcmp(Command,"ls")==0){
				parameter1 = strtok(NULL,tab);
				if (parameter1 ==NULL)
				{
					ls(Directory_PATH);

				}
				else{
					conca(parameter1,Directory_PATH,p1);

					ls(p1);

				}	

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"cat")==0){
				parameter1 = strtok(NULL,tab);
				if (parameter1==NULL)
				{
					perror("DEBE INGRESAR UN ARCHIVO");
				}
				else{
					conca(parameter1,Directory_PATH,p1);

					cat(p1);
				}
				

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"cp")==0){
				parameter1 = strtok(NULL,tab);
				

				parameter2 = strtok(NULL,tab);
				
				
				conca(parameter1,Directory_PATH,p1);
				conca(parameter2,Directory_PATH,p2);
				if (parameter1 == NULL && parameter2 == NULL)
				{
					perror("INGRESE AL MENOS UN PARAMETRO");
					exit(1);
				}

				if (parameter1 == NULL)
				{
					perror("Introduzca el primer parametro");
					exit(1);
				}
				else if (parameter2==NULL)
				{
					cp2(p1,Directory_PATH,parameter1);
				}
				else{
					int i;
					int k;
					if ((i = FileType(p1))!=0 && (k =FileType(p2))==0)
					{
						printf("SE CUMPLE");
						cp2(p1,p2,parameter1);
					}
					else{
						cp(p1,p2);
					}	
				}	

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"mv")==0){
				parameter1 = strtok(NULL,tab);
				conca(parameter1,Directory_PATH,p1);

				parameter2 = strtok(NULL,tab);
				conca(parameter2,Directory_PATH,p2);

				if (parameter1 == NULL && parameter2 == NULL)
				{
					perror("INGRESE AL MENOS UN PARAMETRO");
					exit(1);
				}

				if (parameter1 == NULL)
				{
					mv(Directory_PATH,p2);
				}
				else if (parameter2==NULL)
				{
					mv(p1,Directory_PATH);
				}
				else{
					mv(p1,p2);
				}	


				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"find")==0){
				parameter1 = strtok(NULL,tab);
				if (parameter1 == NULL)
				{
					perror("INTRODUZCA EL PARAMETRO");
					exit(1);
				}

				find(parameter1,Directory_PATH);

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"rm")==0){
				parameter1 = strtok(NULL,tab);
				if (parameter1==NULL)
				{
					rm(Directory_PATH);
				}
				else {
					conca(parameter1,Directory_PATH,p1);
					rm(p1);
				}	

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"mkdir")==0){
				parameter1 = strtok(NULL,tab);
				if (parameter1==NULL)
				{
					perror("INGRESE EL DIRECTORIO A SER CREADO");
					exit(1);
				}
				conca(parameter1,Directory_PATH,p1);

				mkDir(p1);

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"rmdir")==0){
				parameter1 = strtok(NULL,tab);
				if (parameter1==NULL)
				{
					rmDir(Directory_PATH);
				}
				else{
					conca(parameter1,Directory_PATH,p1);

					rmDir(p1);
				}	

				command(Prompt_Command);
				Execute_Command(Prompt_Command,Directory_PATH);
			}
			else if (strcmp(Command,"quit")==0){	
	
				printf("HASTA PRONTO\n");
				exit(0);
				//Kill 0
				// AQUI DEBO MATAR TODOS LOS PROCESOS 
			}
			else{
				perror("INGRESE UN COMANDO VALIDO");
				exit(1);
			}	

			
		}
		else{
			perror("INGRESE UN COMANDO");
			exit(1);
		}

	}



	if (RootDirectory != NULL)
	{ 
		command(Prompt_Command);
		Execute_Command(Prompt_Command,Directory_PATH);
	}	

	else {
		perror("ERROR OPENING DIRECTORY");
		exit(1);
		}


}