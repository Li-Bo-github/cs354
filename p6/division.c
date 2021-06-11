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
// This File:        division.c
// Other Files:      mySigHandler.c, sendsig.c
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

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int count;

//handle with SIGFPE  for /0
void handler_SIGFPE(int sig) {
	printf("Error: a division by 0 operation was attempted.\n");
	printf("Total number of operations completed successfully: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

//teminate
void  handler_SIGINT (int sig) {
	printf("\nTotal number of operations completed successfully: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

int main() {
	count = 0;

	struct sigaction sa;
	memset (&sa, 0, sizeof(sa));
	sa.sa_handler = handler_SIGFPE;
	if (sigaction(SIGFPE, &sa, NULL) < 0) {
		perror("Error binding SIGALRM handler\n");
		exit(0);
	}

	struct sigaction sa2;
	memset (&sa2, 0, sizeof(sa2));
	sa2.sa_handler = handler_SIGINT;	
	if (sigaction (SIGINT, &sa2, NULL) < 0) {
		perror ("Error binding SIGALRM handler\n");
		exit(0);
	}

	while(1) {
		//buffer of 100 bytes
		char buffer [100];
		printf("Enter first integer: ");
		fgets(buffer, 100, stdin);
		int first_Num = atoi(buffer);
		
		printf("Enter second integer: ");
		fgets(buffer, 100, stdin);
		int second_Num = atoi(buffer);
		
		int result = first_Num / second_Num;
		int remainder = first_Num % second_Num;
		printf("%i / %i is %i with a remainder of %i\n", first_Num, second_Num, result, remainder);
		count++;
	}
	return (0);
}