#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "structs.c"


// --------------------------  PARSER  -----------------------------------

int parse_file (FILE* file, struct Process** process_array, struct Queue* queue) {

  char process_char[2000];
  char process_name[256];
  int priority;
  int init_time;
  int pid = 1;
  int count;
  int last_space = 0;
  int process_char_length = 0 ;

  //iteramos sobre las lineas del archivo
  while (fgets(process_char, 500, file)) {
    last_space = 0;
    count = 0;
    int N;

    //Obtenemos linea por linea el archivo.txt y lo parseamos hasta el parametro N
    process_char_length = (int)strlen(process_char);
    sscanf(process_char,  "%s %d %d %d", process_name, &priority, &init_time, &N);

    //Obtenemos el ultimo espacio antes de la secuencia para parsearla
    for (int i = 0; i < process_char_length; i++) {
      if (process_char[i] == ' ') {
        count++;
      }
      if (count == 4) {
        last_space = i;
        break;
      }
    };

    //Definicion array sequence para la secuencia
    int sequence[1000] = {};
    int seq_index = 0;
    //Desde el espacio despues de N seleccionamos los tiempos de la secuencia
    for (int i = last_space + 1; i < process_char_length; i++) {
      if (process_char[i] != 10 &&  process_char[i] != 32) {
        sequence[seq_index] = atoi(&process_char[i]);
        seq_index++;
        //Para tomar los numeros de mas de un digito
        while (process_char[i+1] != 10 && process_char[i+1] != 32){
          i++;
        }
      }
    };

    struct Process* process = process_create(pid, priority, process_name, 2,
                                            sequence, init_time, N);
    process_array[pid] = process;
    queue_insert(queue, process);
    pid++;
  };
  fclose(file);
  return 0;
};

// --------------------------  FCFS  -----------------------------------

Process* fcfs(struct Queue* queue) {

  struct Process* process = queue_get_front(queue);

  return process;
}

// --------------------------  ROUNDROBIN  -----------------------------------


// --------------------------  PRIORITY  -----------------------------------



// --------------------------  MAIN  -----------------------------------

int main(int argc, char *argv[]) {

  // Variable quantum por defecto: 3
  int quantum = 3;
  if (argv[3] != NULL) {
    quantum = atoi(argv[3]);
  }

  // scheduler = argv[1]; file = argv[2]; quantum = argv[3];
  FILE* file = fopen(argv[2], "r");

  if (file == NULL) {
    printf("ERROR: No existe tal archivo\n");
    return 0;
  }
  if ( argc < 3) {
    printf("ERROR: Ingresa todos los parametros...\n");
    return 0;
  }
  // scheduler invalido //
  if (strcmp(argv[1], "fcfs") != 0 &&
      strcmp(argv[1], "roundrobin") != 0 &&
      strcmp(argv[1], "priority") != 0 ){
    printf("ERROR: Parametro scheduler inválido\n");
    return 0;
  }

  //      DEFINICION DE VARIABLES

  char process_char_counter[2000];
  int process_number = 0;
  int simulation_time = 0;


  //      CREACIÓN QUEUE Y ARRAYPROCESS

  // para ver cuantas lineas o procesos tiene el archivo
  FILE* file_ = fopen(argv[2], "r");
  while (fgets(process_char_counter, 500, file_)) {process_number++;}
  fclose(file_);

  // Creacion del arreglo de procesos
  process_number++; // por proceso idle
  struct  Process** process_array = malloc(process_number * sizeof(Process*));

  Queue* queue = queue_create(process_number);

  //creacion del proceso idle
  process_array[0] = process_idle();

  parse_file(file, process_array, queue);

  queue_process_print(queue);

  printf("\n\n ----- SIMULATION ----- \n\n");

  while (!queue_is_empty(queue)){

    // 1. Revisar estado de procesos y cambiarlos

    if (strcmp(argv[1], "fcfs")) {

      // 2. Revisar si el primero esta en READY
      // 2.a if TRUE
      //    se ejecuta
      // 2.b else
      //    pass

      

    }
    else if (strcmp(argv[1], "roundrobin")) {

    }
    else {

    }

    simulation_time++;

  }

  return 0;

};
