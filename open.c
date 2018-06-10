#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include "execution.h"
//#define Directory_PATH "/home/maga"
//# define char Prompt_Command [1025]
# define MAXCHAR  1025
main(){

	//////////////////////////////////////////////////////////////
	/////////////////////////////
	//Abrimos el Directorio Raiz

	char  Directory_PATH [1025];
	printf("c :\\>prompt$");
	scanf("%1024s",Directory_PATH );
	DIR *RootDirectory;
	RootDirectory = opendir(Directory_PATH);
	//////////////////////////////
	char Prompt_Command [1025];
	char Whitespace[2] = "-";
	char *Command;
	char *parameter1;
	char *parameter2;

	
	//FUNCION QUE RECIBE LOS COMANDOS DEL COMMAND PROMPT
	void command(char *Prompt_Command){
		printf("\nc :\\>prompt$");
		scanf("%1024s",Prompt_Command);
	}

	//FUNCION QUE LEE EL COMANDO Y LO EJECUTA 
	void Execute_Command(char *Prompt_Command){	
		if (Prompt_Command != NULL){


			Command = strtok(Prompt_Command,Whitespace);
			printf("%s\n",Command);

			//parameter1 = strtok(NULL,Whitespace);
			//printf("%s\n",parameter1);

			//if(parameter1 != NULL){
			//	parameter2 = strtok(NULL,Whitespace);
			//	printf("%s\n",parameter2);
			//}
			
			if (strcmp(Command,"ls")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);
				ls(parameter1);

				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"cat")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);
				cat(parameter1);
				

				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"cp")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);
				parameter2 = strtok(NULL,Whitespace);
				printf("%s\n",parameter2);				

				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"mv")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);
				parameter2 = strtok(NULL,Whitespace);
				printf("%s\n",parameter2);


				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"find")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);


				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"rm")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);


				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"mkdir")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);


				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"rmdir")==0){
				parameter1 = strtok(NULL,Whitespace);
				printf("%s\n",parameter1);


				command(Prompt_Command);
				Execute_Command(Prompt_Command);
			}
			else if (strcmp(Command,"quit")==0){	
				printf("quit");	
				printf("HASTA PRONTO");
				exit(1);
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
		Execute_Command(Prompt_Command);
	}	

	else {
		perror("ERROR OPENING DIRECTORY");
		exit(1);
		}


}