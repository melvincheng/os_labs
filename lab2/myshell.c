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
	// Parse the commands provided using argc and argv

	// Perform an infinite loop getting command input from users
	printf("%s ", getcwd(PWD, sizeof(PWD)));
	while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
	{
		// Perform string tokenization to get the command and argument

		token = strtok(buffer, " \n");
		strcpy(command, token);
		printf("Command Entered: %s\n", command);
		//printf("%lu\n", sizeof(command));
		//printf("%lu\n", sizeof(token));

		// Check the command and execute the operations for each command
		// cd command -- change the current directory
		if (strcmp(command, "cd") == 0)
		{
			token = strtok(NULL, "\n");
			chdir(token);
			// your code here
			printf("%s\n", "You typed cd");
		}
		else if (strcmp(command, "dir") == 0)
		{
			DIR *dir;
			struct dirent *ent;
			dir = opendir(".");
			if(dir)
			{
				while(dir = readdir(ent))
				{
					printf("%s\n", dir->d_name);
				}
				close(dir);
			}
		}

		// other commands here...

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
		printf("%s ", getcwd(PWD, sizeof(PWD)));
	}
	return EXIT_SUCCESS;
}
