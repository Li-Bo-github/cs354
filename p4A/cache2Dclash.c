////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dclash.c
// This File:        cache2Dclash.c
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

int arr2D[128][8];

int main(){
	for(int iteration = 0; iteration < 100; iteration++){
		for (int row = 0; row < 128; row += 64){
			for (int col = 0; col < 8; col++){
				arr2D[row][col] = iteration + row + col;
			}
		} 
	}
   return 0;
}