#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/types.h>
#include <sys/wait.h>
// #include <ctype.h>  // is_digit


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
  pid_t pid = getpid();
	int iterations, rows, cols, values, threads, cores, status;
	iterations = strtol(argv[1], NULL, 10);
	rows = strtol(argv[2], NULL, 10);
	cols = strtol(argv[3], NULL, 10);
	values = strtol(argv[4], NULL, 10);
	threads  = strtol(argv[5], NULL, 10);
	cores = 2; //sysconf(_SC_NPROCESSORS_ONLN);
  status = 0;
  int array[2 * values];
  for(int i = 0; i < values; i++){
    int r, c;
    scanf("%d %d", &r, &c);
    array[2 * i] = r;
    array[2 * i + 1] = c;
  }
  for(int workers = 0; workers < cores; workers++){
    if (pid != 0){
      printf("Entra al if: %d\n", getpid());
      pid = fork();
    }
  }
  if (pid == 0){
    printf("Soy un hijo\n");
    status = 2;
  }
  else{
    while(status != 2 && pid != 0){
      wait(&status);
    }
  }
  printf("All childs should have finished");
  _exit(1);
  return 0;
}
