///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2021, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
   
// add your own File Header information here
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      (name of all other files if any)
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
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
	printf("Enter magic square's size (odd integer >=3)\n");
	int size_get;
	scanf("%d", &size_get);
	
	//check
	if ((size_get % 2) == 0 ){
		printf("Magic square size must be odd.\n");
		exit(1);
	}
	
	if (size_get < 3 ){
		printf("Magic square size must be >= 3.\n");
		exit(1);
	}
	
    return size_get;   
} 
   
/* TODO:
 * Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
	int row = n/2;
    int	col = n-1;
	int nsqr = n * n;
	
	//create magic square
	int **ms;
	ms = malloc(sizeof(int *) * n);
	if (ms == NULL) {
        printf("Error while allocating memory on the heap.\n");
        exit(1);
    }
	for (int r = 0; r < n; r++) {
		*(ms + r) = malloc(sizeof(int)* n);
		if (*(ms + r) == NULL) {
			printf("Error while allocating memory on the heap.\n");
			exit(1);
		}
	}
	
	//next row+1,col+1
	for (int v = 1; v <= nsqr; ++v){		
		*(*(ms + row) + col) = v;
		
		int old_row = row;
		int old_col = col;
		
		row++;
		col++;
		
		//Wrap around
		if (col == n && row == n){
			col = 0;
			row = 0;
		}
		
		if (col == n){ 
			col = 0;
		}else if (row == n){
			row = 0;
		}
		
		if (*(*(ms + row) + col) != 0){
			//move left
			col = old_col - 1;
			row = old_row;
		}
	} 
	
	//create pointer to struct
	MagicSquare *ptr = malloc(sizeof(MagicSquare));
	ptr -> size = n;
	ptr -> magic_square = ms;
	
    return ptr;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	FILE *ofp = fopen(filename, "w");
	
	if (ofp == NULL) {
        printf("Can't open output file\n");
        exit(1);
    }
	
	//convert to string and store it
	int row_len = magic_square -> size;
	char buffer[10];
	
	sprintf(buffer, "%d", row_len);
	fputs(buffer,ofp);
	fputs("\n",ofp);
	
	for (int i = 0; i < row_len; i++) {
		for (int j = 0; j < row_len; j++) {
			int value = *(*(magic_square -> magic_square + i) + j);
			sprintf(buffer, "%d", value);
			fputs(buffer,ofp);
			if (j != row_len-1){
				fputs(",",ofp);
			}
		}
		fputs("\n",ofp);
	}
	
	//close
	if (fclose(ofp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
}

/* TODO:
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {
    // TODO: Check input arguments to get output filename
	if (2 != argc) {
		printf("Usage: ./myMagicSquare <output_filename>\n");
        exit(1);
	}
	
    // TODO: Get magin square's size from user
	int size = getSize();
    // TODO: Generate the magic square
	MagicSquare *p = generateMagicSquare(size);
    // TODO: Output the magic square
	fileOutputMagicSquare(p,*(argv + 1));
	//free ms in struct, ptr,p
	for (int r = 0; r < size; r++) {
		free(*(p -> magic_square + r));
	}
	free(p -> magic_square);
	
	free(p);
	
	
	
    return 0;
} 






                                                         
//		s21		myMagicSquare.c      

