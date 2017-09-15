#include <stdio.h>
#include <stdlib.h>
#include "structs.h"


Matrix* init_matrix(int n_row, int n_col){
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->rows = n_row;
    matrix->cols = n_col;
    matrix->cells = malloc(sizeof(bool*) * n_row);
    for (int i = 0;i < n_row; i++){
        matrix->cells[i] = malloc(n_col * sizeof(bool));
        for (int j = 0;j < n_col; j++){
          matrix->cells[i][j] = 0;
        }
    }
    return matrix;
}

void* poblate_matrix(Matrix* matrix, int *starter, int size){
	for (int i = 0; i < size; i++){
		matrix->cells[starter[2 * i]][starter[2 * i + 1]] = 1;
		}
	return matrix;
}

void check_neighbours(int row, int col, Matrix* matrix, Matrix* future_m){
    int count = 0;
    int prev_r = row - 1;
    int next_r = row + 1;
    int prev_c = col - 1;
    int next_c = col + 1;
    if (prev_r < 0) {prev_r = matrix->rows - 1;}
    if (next_r == matrix->rows) {next_r = 0;}
    if (prev_c < 0) {prev_c = matrix->cols - 1;}
    if (next_c == matrix->cols) {next_c = 0;}
    // diag izq abajo
    if (matrix->cells[next_r][prev_c]){
        count++;}
    // izquierda
    if (matrix->cells[row][prev_c]){
        count++;}
    // diagonal izquierda arriba
    if (matrix->cells[prev_r][prev_c]){
        count++;}
    // arriba
    if (matrix->cells[prev_r][col]){
        count++;}
    // abajo
    if (matrix->cells[next_r][col]){
        count++;}
    // diagonal derecha arriba
    if (matrix->cells[prev_r][next_c]){
        count++;}
    // derecha
    if (matrix->cells[row][next_c]){
        count++;}
    // derecha abajo
    if (matrix->cells[next_r][next_c]){
        count++;}

    // Muere
    if (count < 2){future_m->cells[row][col] = 0;}
    // Nace
    else if (count == 3 && !matrix->cells[row][col]){
       future_m->cells[row][col] = 1;
    }
    // Sobrevive otra generacion
    else if ((count == 2 || count == 3) && (matrix->cells[row][col])){
        future_m->cells[row][col] = 1;
    }
    // Muere sobrepoblada
    else{future_m->cells[row][col] = 0;}

    return;
}

void print_matrix(Matrix* matrix){
  for (int i = 0; i < matrix->rows; i++){
		for (int j = 0; j < matrix->cols; j++){
      printf("%d ", matrix->cells[i][j]);
    }
    printf("\n");
  }
}

void destroy_matrix(Matrix* matrix){
  for (int i = 0; i < matrix->rows; i++){free(matrix->cells[i]);}
	free(matrix->cells);
	free(matrix);
}


// ThreadBase
ThreadBase* init_thread(int* rows, int* cols, int index){
  ThreadBase* threadb = malloc(sizeof(ThreadBase));
  threadb->rows = rows;
  threadb->cols = cols;
  threadb->index = index;
  return threadb;
}

void destroy_threadbase(ThreadBase* thread){
  free(thread);
}
