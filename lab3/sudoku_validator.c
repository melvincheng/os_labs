#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

void *horizontal_check(void *arg);
void *vertical_check(void *arg);
void *box_check(void *arg);
int find(int val, int *array);
int clear(int *array);

pthread_mutex_t valid_mutex;

// struct of the puzzle
// used to pass the puzzle to the threads
typedef struct{
	int sudoku[9][9];
} sudoku;

// variable indicates whether or not the puzzle is valid
// 1 is invalid
// 0 is invalid
int valid = 1;

int main(void){
	FILE *file;
	sudoku puzzle;
	pthread_t tid[NUM_THREADS];

	file = fopen("puzzle.txt", "r");

	// reads in the puzzle and puts in into the struct
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			fscanf(file, "%d", &puzzle.sudoku[i][j]);
		}
	}
	fclose(file);

	// creates threads to check the puzzle
	// one thread to check the rows,
	// one thread to check the columns,
	// one thread to check the squares/ subgrid
	// each thread checks has 9 different blocks
	// giving each thread equal load
	pthread_create(&tid[0], NULL, horizontal_check, (void*)&puzzle);
	pthread_create(&tid[1], NULL, vertical_check, (void*)&puzzle);
	pthread_create(&tid[2], NULL, box_check, (void*)&puzzle);

	// joins the threads
	// maaster thread waits for all the threads before continuing
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	// outputs the puzzle
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			printf("%d ", puzzle.sudoku[i][j]);
		}
		printf("\n");
	}
	
	// outputs whether the puzzle is valid or not
	if(valid == 0){
		printf("Invalid\n");
	}else{
		printf("Valid\n");
	}

	pthread_exit(NULL);
}

// checks the rows
// takes the puzzle as an argument
void *horizontal_check(void *arg){
	// temporary array use to store values that were found previous in the current row
	int temp[9] = { 0 };
	sudoku *puzzle = (sudoku*)arg;

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			// if the value is 0, the value is empty in the puzzle
			// therefore, skip the number
			if(puzzle->sudoku[i][j] != 0){
				// if the value currently being check is already
				// in the row, the puzzle is invalid and the thread exits
				// as more check is unnecessary
				if(find(puzzle->sudoku[i][j], temp) == 0){
					pthread_mutex_lock(&valid_mutex);
					valid = 0;
					pthread_mutex_unlock(&valid_mutex);
					pthread_exit(NULL);
				}
				// if the current value was not found previous in the current row,
				// add it to a temporary array
				temp[j] = puzzle->sudoku[i][j];
			}
		}
		// clears the temporary array
		clear(&temp);
	}
	pthread_exit(NULL);
}

// checks the columns
// takes the puzzle as an argument
void *vertical_check(void *arg){
	// temporary array use to store values that were found previous in the current column
	int temp[9] = { 0 };
	sudoku *puzzle = (sudoku*)arg;
	
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			// if the value is 0, the value is empty in the puzzle
			// therefore, skip the number
			if(puzzle->sudoku[j][i] != 0){
				// if the value currently being check is already
				// in the column, the puzzle is invalid and the thread exits
				// as more check is unnecessary
				if(find(puzzle->sudoku[j][i], temp) == 0){
					pthread_mutex_lock(&valid_mutex);
					valid = 0;
					pthread_mutex_unlock(&valid_mutex);
					pthread_exit(NULL);
				}
				// if the current value was not found previous in the current column,
				// add it to a temporary array
				temp[j] = puzzle->sudoku[j][i];
			}
		}
		// clears the temporary array
		clear(&temp);
	}
	pthread_exit(NULL);	
}

// checks the subgird
// takes the puzzle as an argument
void *box_check(void *arg){
	// temporary array use to store values that were found previous in the current subgrid
	int temp[9] = { 0 };
	// used to keep track of the temp array
	// this is exclusive to this function because
	// for loops do not go up to 9
	int x = 0;
	sudoku *puzzle = (sudoku*)arg;

	for(int l = 0; l < 9; l+=3){
		for(int k = 0; k < 9; k+=3){
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					// if the value is 0, the value is empty in the puzzle
					// therefore, skip the number
					if(puzzle->sudoku[i+k][j+l] != 0){
						// if the value currently being check is already
						// in the subgrid, the puzzle is invalid and the thread exits
						// as more check is unnecessary
						if(find(puzzle->sudoku[i+k][j+l], temp) == 0){
							pthread_mutex_lock(&valid_mutex);
							valid = 0;
							pthread_mutex_unlock(&valid_mutex);
							pthread_exit(NULL);
						}else{
							// if the current value was not found previous in the current subgrid,
							// add it to a temporary array
							temp[x++] = puzzle->sudoku[i+k][j+l];
						}
					}
				}
			}
			// clears the temporary array
			clear(&temp);
			x = 0;
		}
	}
	pthread_exit(NULL);
}

// checks if the current value was in the checking block
// returns 0 if found
// retunrs 1 if not found
int find(int val, int *array){
	for(int i = 0; i < 9; i++){
		if(val == array[i]){
			return 0;
		}
	}
	return 1;
}

// empties the temporary array that is used to store the values in the block
int clear(int *array){
	for(int i = 0; i < 9; i++){
		array[i] = 0;
	}
}