#include "structs.h"


// Lifegame
int lifegame(int rows, int cols, int iterations, int* starter,int values){
	// Creo mi matriz
	Matrix* matrix = init_matrix(rows, cols);
	poblate_matrix(matrix, starter, values);
	Matrix* future_matrix = init_matrix(matrix->rows, matrix->cols);

	// inicia el ciclo
	while (iterations > 0){
		for (int i = 0; i < matrix->rows; i++){
			for (int j = 0; j < matrix->cols; j++){
				check_neighbours(i, j, matrix, future_matrix);
			}
		}
		// Tener la segunda matriz era para que no afecten los cambios
		for (int i = 0; i < matrix->rows; i++){
			for (int j = 0; j < matrix->cols; j++){
				matrix->cells[i][j] = future_matrix->cells[i][j];
			}
		}
		iterations--;
	}

	// Requested output
	print_matrix(matrix);

	// Free memory
	destroy_matrix(matrix);
	destroy_matrix(future_matrix);
	return 0;
}
