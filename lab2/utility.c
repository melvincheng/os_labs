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

extern void input_file()
{
	char *token;
	token = strtok(NULL, " \t\n");
	if(strcmp(token, "<") == 0)
	{
		return strtok(NULL, " \t\n");
	}
	else
	{
		return NULL;
	}
}

extern char *output_file(char *token)
{
	if(strcmp(token, ">") == 0)
	{
		return strtok(NULL, " \t\n");
	}
	else
	{
		return NULL;
	}
}