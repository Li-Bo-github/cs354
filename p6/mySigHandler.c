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
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

//print every 3 seconds
int global_time = 3;
//count of SIGUSR1
int count =0;

/*set an alarm clock for delivery of a signal
 */
void handler_ALARM (int sig){
	time_t currtime;
	if (time(&currtime) == -1) {
		perror("Error time");
		exit(0);
	}
	printf("PID: %d", getpid());
	printf(" CURRENT TIME: %s", ctime(&currtime));
	if (alarm(global_time) != 0) {
		printf("Alarm error.\n");
		exit(0);
	}
}

/*count SIGUSR1
 */
void handler_SIGUSR (int sig) {
	printf("SIGUSR1 handled and counted!\n");
	count++;
}

/*ctrl c specified
 */
void  handler_SIGINT (int sig) {
	printf("\nSIGINT handled.\n");
	printf("SIGUSR1 was handled %i times. Exiting now.\n", count);
	exit(0);
}

/*The main function
 */
int main(int argc, char *argv[]) {
	printf("PID and time print every 3 seconds.\n");
	printf("Type Ctrl-C to end the program.\n");

	struct sigaction sa;
	memset (&sa, 0, sizeof(sa));
	sa.sa_handler = handler_ALARM;
	if (sigaction(SIGALRM, &sa, NULL) < 0){
		printf("Error binding SIGALRM handler\n");
		exit(0);
	}

	struct sigaction sa2;
	memset (&sa2, 0, sizeof(sa2));
	sa2.sa_handler = handler_SIGUSR;
	if (sigaction (SIGUSR1, &sa2, NULL) < 0) {
		perror ("Error binding SIGALRM handler\n");
		exit(0);
	}

	struct sigaction sa3;
	memset (&sa3, 0, sizeof(sa3));
	sa3.sa_handler = handler_SIGINT;
	if (sigaction (SIGINT, &sa3, NULL) < 0) {
		perror ("Error binding SIGALRM handler\n");
		exit(0);
	}

	while(1) {
	}
	return (0);
}