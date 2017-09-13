#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include "structs.h"


void     INThandler(int);
Matrix* matrix;
Matrix* future_matrix;


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
	// printf("isdigit: %d", isdigit(*argv[1]));
	// argv[1] = iteraciones, argv[2] = filas, argv[3] = columnas
	// argv[1] = estado inicial, argv[2] = threads
	// NO SE SI ES NECESARIO HACER MAS VERIFICACION

  signal(SIGINT, INThandler);

  // INITIAL VARIABLES
  pid_t pid = getpid();
	int iterations, rows, cols, values, threads, cores, status;
	iterations = strtol(argv[1], NULL, 10);
	rows = strtol(argv[2], NULL, 10);
	cols = strtol(argv[3], NULL, 10);
	values = strtol(argv[4], NULL, 10);
	threads  = strtol(argv[5], NULL, 10);
	cores = sysconf(_SC_NPROCESSORS_ONLN);
  int array[2 * values];
  pid_t all_pid[cores];

  // INPUT ALIVE BACTERIA
  for(int i = 0; i < values; i++){
    int r, c;
    scanf("%d %d", &r, &c);
    array[2 * i] = r;
    array[2 * i + 1] = c;
  }

  // COPIED MEMORY
  matrix = init_matrix(rows, cols);
  future_matrix = init_matrix(matrix->rows, matrix->cols);
	poblate_matrix(matrix, array, values);
  int counter = -1;

  // SHARED MEMORY
  int *total_iter;
  Matrix** actual_state;
  total_iter = mmap(NULL, cores * sizeof(int), PROT_READ | PROT_WRITE,
                    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  actual_state = mmap(NULL, cores * sizeof(Matrix*), PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_SHARED, -1, 0);

  // CREATE WORKERS
  for(int workers = 0; workers < cores; workers++){
    if (pid != 0){
      counter++;  // Esto hara que cada worker se quede con un counter distinto
      pid = fork();
      if(pid){all_pid[workers] = pid;}
    }
  }

  if (pid == 0){  // Child
    while (iterations > 0){
      // FALTA AGREGAR THREADS
      lifegame(matrix, future_matrix);
      iterations--;

      // EDIT SHARED MEMORY
      total_iter[counter] = iterations;
      actual_state[counter] = matrix;
    }
    // REQUESTED OUTPUT
  	print_matrix(matrix);

  	// FREE MEMORY
  	destroy_matrix(matrix);
    destroy_matrix(future_matrix);
    exit(0);
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
  return 0;
}


void  INThandler(int sig){
    destroy_matrix(matrix);
    destroy_matrix(future_matrix);
    exit(0);
}
