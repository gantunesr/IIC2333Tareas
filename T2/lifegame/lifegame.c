#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>  // is_digit
#include "structs.h"


int main(int argc, char** argv)
{
	if(argc != 6){
		printf("Modo de uso: %s <iteraciones> <filas> <columnas> <estado inicial> <threads>\n", argv[0]);
    printf("\titeraciones: número de iteraciones totales que deberá dar el juego de la vida\n");
		printf("\tfilas: Filas de la comunidad bacteriana\n");
		printf("\tcolumnas: Columnas de la comunidad bacteriana\n");
		printf("\testado inicial: Numero de bacterias vivas al comienzo del juego de la vida\n");
		printf("\tthreads: Threads que utilizara cada worker\n");
		printf("\tTodos corresponed a numeros enteros positivos y el estado inicial debe ser menos o igual al producto de filas y columnas\n");
		return 1;
	}
	// printf("isdigit: %d", isdigit(*argv[1]));
	// argv[1] = iteraciones, argv[2] = filas, argv[3] = columnas
	// argv[1] = estado inicial, argv[2] = threads
	// NO SE SI ES NECESARIO HACER MAS VERIFICACION

	int iterations, rows, cols, values, threads, cores;
	iterations = strtol(argv[1], NULL, 10);
	rows = strtol(argv[2], NULL, 10);
	cols = strtol(argv[3], NULL, 10);
	values = strtol(argv[4], NULL, 10);
	threads  = strtol(argv[5], NULL, 10);
	cores = sysconf(_SC_NPROCESSORS_ONLN);
	printf("Cores: %d\n", cores);

	// Creo mi matriz
	Matrix* matrix = init_matrix(rows, cols);
	poblate_matrix(matrix, values);
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
