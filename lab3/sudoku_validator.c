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

typedef struct{
	int sudoku[9][9];
} sudoku;

int valid = 1;

int main(void){
	FILE *file;
	sudoku puzzle;
	pthread_t tid[NUM_THREADS];

	file = fopen("puzzle.txt", "r");

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			fscanf(file, "%d", &puzzle.sudoku[i][j]);
		}
	}
	fclose(file);

	pthread_create(&tid[0], NULL, horizontal_check, (void*)&puzzle);
	pthread_create(&tid[1], NULL, vertical_check, (void*)&puzzle);
	pthread_create(&tid[2], NULL, box_check, (void*)&puzzle);

	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			printf("%d ", puzzle.sudoku[i][j]);
		}
		printf("\n");
	}
	
	if(valid == 0){
		printf("Invalid\n");
	}else{
		printf("Valid\n");
	}

}

void *horizontal_check(void *arg){
	int temp[9] = { 0 };
	sudoku *puzzle = (sudoku*)arg;

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(puzzle->sudoku[i][j] != 0){
				if(find(puzzle->sudoku[i][j], temp) == 0){
					pthread_mutex_lock(&valid_mutex);
					valid = 0;
					pthread_mutex_unlock(&valid_mutex);
					pthread_exit(NULL);
				}
				temp[j] = puzzle->sudoku[i][j];
			}
		}
		clear(&temp);
	}
	pthread_exit(NULL);
}

void *vertical_check(void *arg){
	int temp[9] = { 0 };
	sudoku *puzzle = (sudoku*)arg;
	
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(puzzle->sudoku[j][i] != 0){
				if(find(puzzle->sudoku[j][i], temp) == 0){
					pthread_mutex_lock(&valid_mutex);
					valid = 0;
					pthread_mutex_unlock(&valid_mutex);
					pthread_exit(NULL);
				}
				temp[j] = puzzle->sudoku[j][i];
			}
		}
		clear(&temp);
	}
	pthread_exit(NULL);	
}

void *box_check(void *arg){
	int temp[9] = { 0 };
	int x = 0;
	sudoku *puzzle = (sudoku*)arg;

	for(int l = 0; l < 9; l+=3){
		for(int k = 0; k < 9; k+=3){
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					if(puzzle->sudoku[i+k][j+l] != 0){
						if(find(puzzle->sudoku[i+k][j+l], temp) == 0){
							pthread_mutex_lock(&valid_mutex);
							valid = 0;
							pthread_mutex_unlock(&valid_mutex);
							pthread_exit(NULL);
						}else{
							temp[x++] = puzzle->sudoku[i+k][j+l];
						}
					}
				}
			}
			clear(&temp);
			x = 0;
		}
	}
	pthread_exit(NULL);
}

int find(int val, int *array){
	for(int i = 0; i < 9; i++){
		if(val == array[i]){
			return 0;
		}
	}
	return 1;
}

int clear(int *array){
	for(int i = 0; i < 9; i++){
		array[i] = 0;
	}
}