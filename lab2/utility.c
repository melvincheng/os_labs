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
#include "utility.h"

// Define your utility functions here, these will most likely be functions that you call
// in your myshell.c source file

int output_redirection(FILE **file, char *token)
{
	// variable declaration
	char file_name[256];

	// checks if the token is write or append
	if(strcmp(token, ">") == 0)
	{
		// if a file is not given, nothing happens
		if((token = strtok(NULL, " \t\n")))
		{
			strcpy(file_name, token);
			*file = fopen(file_name, "w");
			return 0;
		}
	}
	else if(strcmp(token, ">>") == 0)
	{
		// if a file is not given, nothing happens
		if((token = strtok(NULL, " \t\n")))
		{
			strcpy(file_name, token);
			*file = fopen(file_name, "a");
			return 0;
		}
	}
	return 1;
}