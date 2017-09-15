#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include "structs.h"
#include <pthread.h>


void INThandler(int);
void *lifegame_part1(void* tStruct);
void *lifegame_part2(void* tStruct);


Matrix *matrix, *future_matrix;
int *total_iter, counter, cores, threads;
Matrix** actual_state;
int** thread_rows;
int** thread_cols;


int main(int argc, char** argv){
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
	// argv[1] = iteraciones, argv[2] = filas, argv[3] = columnas
	// argv[4] = estado inicial, argv[5] = threads
	// NO SE SI ES NECESARIO HACER MAS VERIFICACION

  signal(SIGINT, INThandler);

  // INITIAL VARIABLES
  pid_t pid = getpid();
	int iterations, rows, cols, values, status;
	iterations = strtol(argv[1], NULL, 10);
	rows = strtol(argv[2], NULL, 10);
	cols = strtol(argv[3], NULL, 10);
	values = strtol(argv[4], NULL, 10);
	threads  = strtol(argv[5], NULL, 10);
	cores = sysconf(_SC_NPROCESSORS_ONLN);
  int array[2 * values];
  pid_t all_pid[cores];
	int matrix_size = rows * cols;
  // INPUT ALIVE BACTERIA
  for(int i = 0; i < values; i++){
    int r, c;
    scanf("%d %d", &r, &c);
    array[2 * i] = r;
    array[2 * i + 1] = c;
  }


  // COPIED MEMORY
  matrix = init_matrix(rows, cols);
  future_matrix = init_matrix(rows, cols);
	poblate_matrix(matrix, array, values);
  counter = -1;

	// SHARED MEMORY
  total_iter = mmap(NULL, cores * sizeof(int), PROT_READ | PROT_WRITE,
                    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  actual_state = mmap(NULL, cores * sizeof(Matrix*), PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	thread_cols = mmap(NULL, threads * sizeof(int*), PROT_READ | PROT_WRITE,
                    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	thread_rows = mmap(NULL, threads * sizeof(int*), PROT_READ | PROT_WRITE,
                    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	// THREAD DIVISIONS
	int entero = matrix_size / threads;
	int resto = matrix_size % threads;
	for(int t = 0; t < threads; t++){
		int size = entero;
		if(resto){size++; resto--;}
		int* array = malloc((size + 1) * sizeof(int));
		int* array2 = malloc((size + 1) * sizeof(int));
		array[0] = size + 1;
		array2[0] = size + 1;
		int index = 1;
		int count = 0;
		for(int r = 0; r < rows; r++){
			for(int c = 0; c < cols; c++){
				if((r * count + c) % threads == t){
					array[index] = r;
					array2[index] = c;
					index++;
				}
			}
			count++;
		}
		thread_rows[t] = array;
		thread_cols[t] = array2;
	}
  // CREATE WORKERS
  for(int workers = 0; workers < cores; workers++){
    if (pid != 0){
      counter++;  // Esto hara que cada worker se quede con un counter distinto
      pid = fork();
      if(pid){all_pid[workers] = pid;}
    }
  }
  if (pid == 0){  // Child
		int cicle = 0;
		while(cicle != iterations){
			pthread_t tid[threads];
			for(int t = 0; t < threads; t++){
					ThreadBase* threadb = init_thread(thread_rows[t], thread_cols[t], t);
					pthread_create(&tid[t], NULL, &lifegame_part1, (void*)threadb);
			}
			for(int t = 0; t < threads; t++){
					pthread_join(tid[t], NULL);
			}

			for(int t = 0; t < threads; t++){
					ThreadBase* threadb = init_thread(thread_rows[t], thread_cols[t], t);
					pthread_create(&tid[t], NULL, &lifegame_part2, (void*)threadb);
			}
			for(int t = 0; t < threads; t++){
					pthread_join(tid[t], NULL);
			}

			cicle++;
			total_iter[counter] = cicle;
			actual_state[counter] = matrix;
		}

    // REQUESTED OUTPUT
  	print_matrix(matrix);

  	// FREE MEMORY
		destroy_matrix(matrix);
    destroy_matrix(future_matrix);
    exit(counter);
  }
  else if(pid){  // Parent
    destroy_matrix(matrix);
    destroy_matrix(future_matrix);
    wait(&status);
  }

  // KILL CHILDS
  for(int workers = 0; workers < cores; workers++){
    kill(all_pid[workers], SIGTERM);
  }
	for(int t = 0; t < threads; t++){
		free(thread_rows[t]);
		free(thread_cols[t]);
	}
	munmap(&total_iter, cores * sizeof(int));
	munmap(&actual_state, cores * sizeof(Matrix*));
	munmap(&thread_cols, threads * sizeof(int*));
	munmap(&thread_rows, threads * sizeof(int*));
  return 0;
}


void  INThandler(int sig){
		if(total_iter){
			int total_iter = munmap(&total_iter, cores * sizeof(int));
		}
		if(actual_state){
			int actual_state = munmap(&actual_state, cores * sizeof(Matrix*));
		}
		if(thread_cols){
			int thread_cols = munmap(&thread_cols, threads * sizeof(int*));}
		if(thread_rows){
			int thread_rows = munmap(&thread_rows, threads * sizeof(int*));}
		if(matrix){destroy_matrix(matrix);}
    if(future_matrix){destroy_matrix(future_matrix);}
    exit(0);
}


void *lifegame_part1(void* tStruct){
	ThreadBase* threadb = (ThreadBase*)tStruct;
	int size = threadb->rows[0];
	for (int i = 1; i < size; i++){
			check_neighbours(threadb->rows[i], threadb->cols[i], matrix, future_matrix);
	}
	destroy_threadbase(threadb);
	return 0;
}

void *lifegame_part2(void* tStruct){
	ThreadBase* threadb = (ThreadBase*)tStruct;
	int size = threadb->rows[0];
	for (int i = 1; i < size; i	++){
			matrix->cells[threadb->rows[i]][threadb->cols[i]] = \
			future_matrix->cells[threadb->rows[i]][threadb->cols[i]];
	}
	destroy_threadbase(threadb);
	return 0;
}
