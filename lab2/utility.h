/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef UTILITY_H_
#define UTILITY_H_

// Include your relevant functions declarations here they must start with the 
// extern keyword such as in the following example:
// extern void display_help(void);

// function checks if ouput redirection is entered
// returns 0 if output redirection is entered
// returns 1 if output redirection is not entered
extern int output_redirection(FILE **file, char *token);

#endif /* UTILITY_H_ */