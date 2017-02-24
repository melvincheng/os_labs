/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here

// Define functions declared in myshell.h here

int main(int argc, char *argv[])
{
	// Input buffer and and commands
	char buffer[BUFFER_LEN] = { 0 };
	char command[BUFFER_LEN] = { 0 };
	char arg[BUFFER_LEN] = { 0 };
	char *token;
	char PWD[BUFFER_LEN];
	char SHELL[BUFFER_LEN];
	char file_name[BUFFER_LEN];
	int out = 0;
	FILE *file;
	strcpy(SHELL, getcwd(PWD, sizeof(PWD)));
	// Parse the commands provided using argc and argv
	if(argc > 0)
	{
		FILE *file;
		file = fopen(argv[0], "r");

	}
	// Perform an infinite loop getting command input from users
	printf("%s ", getcwd(PWD, sizeof(PWD)));
	while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
	{
		// Perform string tokenization to get the command and argument
		token = strtok(buffer, " \t\n");
		if(token)
		{
			strcpy(command, token);
		}
		// Check the command and execute the operations for each command
		// cd command -- change the current directory
		if (strcmp(command, "cd") == 0)
		{
			token = strtok(NULL, "\n");
			chdir(token);
		}
		// clr command -- clears the terminal
		else if (strcmp(command, "clr") == 0){
			printf("\033[2J");
			printf("\033[H");
		}
		// dir command -- displays the content of the current working directory
		else if (strcmp(command, "dir") == 0)
		{
			DIR *dir;
			struct dirent *ent;
			
			while(token = strtok(NULL, " \t\n")){
				if(strcmp(token, ">") == 0)
				{
					strcpy(file_name, strtok(NULL, " \t\n"));
					printf("%s\n", file_name);
					file = fopen(file_name, "w");
					out = 1;
					break;
				}
				(strcmp(token, ">>") == 0)
				{
					strcpy(file_name, strtok(NULL, " \t\n"));
					printf("%s\n", file_name);
					file = fopen(file_name, "a");
					out = 1;
					break;
				}
			}
			if(dir = opendir("."))
			{
				while(ent = readdir(dir))
				{
					if(out)
					{
						fprintf(file, "%s\n", ent->d_name);
					}
					else
					{
						printf("%s\n", ent->d_name);
					}
				}
				fclose(file);
				if(out)
				{
					close(dir);
					out = 0;
				}
			}
		}
		// environ command
		else if (strcmp(command, "environ") == 0)
		{

		}
		// echo command -- displays everything after the echo command, tabs and multiple spaces are reduced to one space
		else if (strcmp(command, "echo") == 0)
		{
			while(token = strtok(NULL, " \t\n")){
				printf("%s ", token);
			}
			printf("\n");
		}
		// help command -- displays the user manual
		else if (strcmp(command, "help") == 0)
		{
			
		}
		// pause command -- pauses the shell until the "enter" key is pressed
		else if (strcmp(command, "pause") == 0)
		{
			printf("Press the [Enter] key to continue...");
			while(getchar() != '\n');
		}
		// quit command -- exit the shell
		else if (strcmp(command, "quit") == 0)
		{
			return EXIT_SUCCESS;
		}
		// Unsupported command
		else
		{
			fputs("Unsupported command, use help to display the manual\n", stderr);
		}
		strcpy(command, "");
		printf("%s ", getcwd(PWD, sizeof(PWD)));
	}
	return EXIT_SUCCESS;
}
