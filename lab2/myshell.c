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
extern char **environ;

// Define functions declared in myshell.h here

void dir();
void environ_command();
void echo();
void help();

int main(int argc, char *argv[])
{
	// Input buffer and and commands
	char buffer[BUFFER_LEN] = { 0 };
	char command[BUFFER_LEN] = { 0 };
	char arg[BUFFER_LEN] = { 0 };
	char *token;
	char SHELL[BUFFER_LEN];
	char cwd[BUFFER_LEN];
	char path[BUFFER_LEN];

	pid_t pid = getpid();
	sprintf(path, "/proc/%d/exe", pid);
	readlink(path, SHELL, BUFFER_LEN);
	setenv("SHELL", SHELL);

	// Parse the commands provided using argc and argv
	if(argc > 0)
	{
		FILE *file;
		file = fopen(argv[0], "r");

	}
	// Perform an infinite loop getting command input from users
	printf("%s ", getenv("PWD"));
	while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
	{
		// Perform string tokenization to get the command and argument
		token = strtok(buffer, " \t\n");

		// Only attempts to copy if the token is not NULL
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
			setenv("PWD", getcwd(cwd, sizeof(cwd)));
		}

		// clr command -- clears the terminal
		else if (strcmp(command, "clr") == 0){
			printf("\033[2J");
			printf("\033[H");
		}

		// dir command -- displays the content of the current working directory
		else if (strcmp(command, "dir") == 0)
		{
			dir();
		}

		// environ command -- displays all the environment strings
		else if (strcmp(command, "environ") == 0)
		{
			environ_command();
		}

		// echo command -- displays everything after the echo command, tabs and multiple spaces are reduced to one space
		else if (strcmp(command, "echo") == 0)
		{
			echo();
		}

		// help command -- displays the user manual
		else if (strcmp(command, "help") == 0)
		{
			help();
		}
		// pause command -- pauses the shell until the "enter" key is pressed
		else if (strcmp(command, "pause") == 0)
		{
			printf("Press the [Enter] key to continue...");
			while(getchar() != "\n");
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
		// Clears command so empty input does not re run previous commands
		strcpy(command, "");

		// outputs current working directory
		printf("%s ", getenv("PWD"));
	}
	return EXIT_SUCCESS;
}

// dir command
void dir()
{
	FILE *file;
	DIR *dir;
	struct dirent *ent;
	char *token;
	int out = 1;

	if((token = strtok(NULL, " \n")) && (dir = opendir(token)))
	{
		while((token = strtok(NULL, " \t\n")))
		{
			if(strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
			{
				out = output_redirection(&file, token);
			}
		}
		while((ent = readdir(dir)))
		{
			if(out == 0)
			{
				fprintf(file, "%s\n", ent->d_name);
			}
			else
			{
				printf("%s\n", ent->d_name);
			}
		}
		if(out == 0)
		{
			fclose(file);
			close(dir);
			out = 1;
		}
	}
	else
	{
		printf("Unable to display directory\n");
	}
}

void environ_command()
{
	FILE *file;
	int out = 1;
	char *token;

	while((token = strtok(NULL, " \t\n")))
	{
		out = output_redirection(&file, token);
	}
	// copys the environ string array to curr
	// then increments the array until *curr is null
	for(char **curr = environ; *curr; curr++)
	{
		if(out == 0)
		{
			fprintf(file, "%s\n", *curr);
		}
		else
		{
			printf("%s\n", *curr);
		}
	}
	if(out == 0)
	{
		fclose(file);
	}
}

// echo command
void echo()
{
	FILE *file;
	char temp[BUFFER_LEN][BUFFER_LEN];
	int i = 0;
	int out = 1;
	char *token;

	while((token = strtok(NULL, " \t\n")))
	{
		if(strcmp(token, ">")  == 0 || strcmp(token, ">>") == 0)
		{
			out = output_redirection(&file, token);
		}
		else
		{
			strcpy(temp[i++], token);
		}
	}
	for(int j = 0; j < i; j++)
	{
		if(out == 0)
		{
			fprintf(file, "%s ", temp[j]);
		}
		else
		{
			printf("%s ", temp[j]);
		}
	}
	if(out == 0)
	{
		fprintf(file, "\n");	
		fclose(file);
		out = 1;
	}
	else
	{
		printf("\n");
	}
}

void help()
{
	FILE *file;
	int out = 1;
	char *token;

	while((token = strtok(NULL, " \t\n")))
	{
		out = output_redirection(&file, token);
	}
	if(out == 0)
	{
		fclose(file);
	}
}