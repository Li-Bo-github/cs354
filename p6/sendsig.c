///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c, division.c
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Author:           Bo Li
// Email:            bli379@wisc.edu
// CS Login:         bol
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
	//check input arg
	if (argc != 3) {
		printf("Usage: <signal type> <pid>\n");
		exit(0);
	}
	
	pid_t pid = atoi(argv[2]);
	int result = strcmp(argv[1],"-i");
	
	if (result > 0) {
		//count SIGUSR1
		if (kill(pid, SIGUSR1) == -1) {
			perror("Error handlering");
			exit(0);
		}
	} else if (result == 0) { 
		//kill
		if (kill(pid, SIGINT) == -1) {
			perror("Error handlering");
			exit(0);
		}
	} else {
		perror("Invalid result");
		exit(0);
	}
	return 0;
}