#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int n = 3;
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
		row++;
		col++;
		
		
		if (*(*(ms + row) + col) != 0){
			//Wrap around
			//exist move left
			if (col == n && row == -1){
				col --;
			}
			if (col == n){ 
				col = 0;
			}else if (row == n){
				row = 0;
			}
		}else{
			//row+2,col+1
			row = row + 2;
			col++;
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", *(*(ms + i) + j));
        }
        printf("\n");
	}
	
	for (int r = 0; r < n; r++) {
		free(*(ms + r));
	}
	free(ms);
	
	return 0;
}
