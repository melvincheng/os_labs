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
	char *token;
	char SHELL[BUFFER_LEN];
	char cwd[BUFFER_LEN];
	char path[BUFFER_LEN];
	char exe_path[BUFFER_LEN];

	// the path where the shell was executed
	getcwd(exe_path, sizeof(exe_path));

	pid_t pid = getpid();
	sprintf(path, "/proc/%d/exe", pid);
	readlink(path, SHELL, BUFFER_LEN);
	setenv("SHELL", SHELL);


	// Parse the commands provided using argc and argv
	// if an additional argument is given, the puts all content into stdin
	if(argc > 1)
	{
		freopen(argv[1], "r", stdin);
	}
	// if no additional arguments are given, then print the cwd along with the prompt
	else
	{
		printf("%s ", getenv("PWD"));
	}
	// Perform an infinite loop getting command input from users
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
			// checks if the next token is NULL
			// if so, print the current working directory
			// if not, attempt to change the directory given
			// if moving to directory failed, an error message is displayed
			if((token = strtok(NULL, "\n")))
			{
				if(chdir(token) == 0)
				{
					setenv("PWD", getcwd(cwd, sizeof(cwd)));
				}
				else
				{
					fprintf(stderr, "Unable to move to directory entered\n");
				}	
			}
			else
			{
				printf("%s\n", getenv("PWD"));
			}
		}

		// clr command -- clears the terminal
		else if (strcmp(command, "clr") == 0){
			// clears the screen by putting multiple
			// this however, moves the cursor to the bottom left of the terminal
			printf("\033[2J");	
			// this then moves the cursor to the top left of the terminal
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
			help(exe_path);
		}
		// pause command -- pauses the shell until the "enter" key is pressed
		else if (strcmp(command, "pause") == 0)
		{
			printf("Press the [Enter] key to continue...");
			// a while loop until the enter key is entered
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
		// Clears command so empty input does not re run previous commands
		strcpy(command, "");

		// outputs current working directory
		if(argc == 1)
		{
			printf("%s ", getenv("PWD"));
		}
	}
	return EXIT_SUCCESS;
}

// dir command
void dir()
{
	// variable declartions
	// out is used to determine if output redirection is used where 1 is false
	FILE *file;
	DIR *dir;
	struct dirent *ent;
	char *token;
	int out = 1;

	// checks if the next token is NULL 
	// and whether or not opening the given directory is successful or not
	// NULL check is required as opendir cannot take NULL as an argument
	// this works because of short-circuit evaluations,
	// if the first statement is NULL, then the second statement is not checked
	if((token = strtok(NULL, " \n")) && (dir = opendir(token)))
	{
		// goes through the rest of the tokens to check if i/o redirection was entered
		while((token = strtok(NULL, " \t\n")))
		{
			if(strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
			{
				out = output_redirection(&file, token);
			}
		}
		// reads the entries of the directory
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
		// closes the file and directory
		if(out == 0)
		{
			fclose(file);
			closedir(dir);
			out = 1;
		}
	}
	// error message if directory is not given
	// or invalid directory is given
	else
	{
		fprintf(stderr, "Unable to display directory\n");
	}
}

void environ_command()
{
	// variable declaration
	// out is used to determine if output redirection is used where 1 is false
	FILE *file;
	int out = 1;
	char *token;

	// goes through the rest of the tokens to check if i/o redirection was entered
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
	// closes the file
	if(out == 0)
	{
		fclose(file);
	}
}

// echo command
void echo()
{
	// variable declaration
	// out is used to determine if output redirection is used where 1 is false
	FILE *file;
	char temp[BUFFER_LEN][BUFFER_LEN];
	int i = 0;
	int out = 1;
	char *token;

	// goes through the entire first argument
	while((token = strtok(NULL, " \t\n")))
	{
		// checks if i/o redirection is used
		// if not, copy the string into the string array
		if(strcmp(token, ">")  == 0 || strcmp(token, ">>") == 0)
		{
			out = output_redirection(&file, token);
		}
		else
		{
			strcpy(temp[i++], token);
		}
	}
	// outputs the message to the file or display
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
	// writes the newline and then closes the file
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

// help command - the argument is where the shell was executed
// this is necessary as if the user changes the directory
// it will search for the readme in the path where the shell was executed
void help(char *exe_path)
{
	// variable declaration
	char *token;
	char temp[BUFFER_LEN];

	char readme[BUFFER_LEN];

	sprintf(readme, "more '%s/readme'", exe_path);

	// goes through the rest of the tokens to check if i/o redirection was entered
	while((token = strtok(NULL, " \t\n")))
	{
		// if i/o redirection is found, concat the string with the i/o redirection
		if((strcmp(token, ">") == 0))
		{
			// if no file is given, concatenation does not occur
			if((token = strtok(NULL, "\n")))
			{
				sprintf(temp, " > %s", token);
				strcat(readme, temp);
			}
		}
		else if((strcmp(token, ">>") == 0))
		{
			// if no file is given, concatenation does not occur
			if((token = strtok(NULL, "\n")))
			{
				sprintf(temp, " >> %s", token);
				strcat(readme, temp);
			}
		}
	}
	// outputs the readme
	system(readme);
}