#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "quicksort.c"


#define RUNNING 0;
#define READY 1;
#define WAITING 2;
#define DEAD 3;

void INTHandler(int);
Queue* waiting_queue;
Queue* ready_queue;
Process** process_array;
int simulation_time, end_process;


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

  struct Process* process;
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

    process = process_create(pid, priority, process_name, 2,
                                            sequence, init_time, N);
    printf("==> P: %s | SL: %d\n", process_name, process_char_length);
    process_array[pid] = process;
    queue_insert(queue, process);
    pid++;
  };
  fclose(file);
  return 0;
};

// --------------------------  ROUNDROBIN  -----------------------------------

void roundrobin_quantum(struct Queue* queue, int quantum){
  struct Process* process;
  for (int i = 0; i < queue->item_count; i++){
    process = queue->array[i];
    int r = round((double)process->priority/quantum);
    process->priority = process->priority * quantum + pow(-1, r) * process->priority;
    process->quantum = process->priority / 64;
    if (process->quantum <= 0){process->quantum = 1;}
  }
  return;
}

// --------------------------  PRIORITY  -----------------------------------

// Función que obtener el indice del proceso de mayor prioridad
int highest_priority_process_index(struct Queue* queue) {
  Process* hprocess = queue->array[0];
  int index = 0;
  for (int i = 0; i < queue->item_count; i++) {
    if (queue->array[i]->priority > hprocess->priority) {
      hprocess = queue->array[i];
      index = i;
    }
  }
  return index;
}

// --------------------------  INTHANDLER ---------------------------------

void INTHandler(int sig){
    signal(sig, SIG_IGN);
    printf("INTERRUPTED\n"
           "SIMULATION TIME: %d\n"
           "FINISHED PROCESS: %d\n\n", simulation_time, end_process);
    printf(" ------ PROCESOS SIN TERMINAR --------\n");
    for (int i = 0; i < waiting_queue->item_count; i++){
      process_print_final_info(waiting_queue->array[i]);
      process_destroy(waiting_queue->array[i]);
    }
    for (int i = 0; i < ready_queue->item_count; i++){
      process_print_final_info(ready_queue->array[i]);
      process_destroy(ready_queue->array[i]);
    }
    queue_destroy(waiting_queue);
    queue_destroy(ready_queue);
    free(process_array[0]);  // no tiene seqqueue ni name
    free(process_array);
    exit(0);
}

// --------------------------  MAIN  -----------------------------------

int main(int argc, char *argv[]) {

  signal(SIGINT, INTHandler);
  // Variable quantum por defecto: 3
  int quantum = 3;
  if (argv[3] != NULL) {
    quantum = atoi(argv[3]);
  }
  printf("%d\n", quantum);

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
  simulation_time = 0;
  end_process = 0;
  // int index = 0; // priority

  //      CREACIÓN QUEUE Y ARRAY PROCESS

  // para ver cuantas lineas o procesos tiene el archivo
  FILE* file_ = fopen(argv[2], "r");
  while (fgets(process_char_counter, 500, file_)) {process_number++;}
  fclose(file_);

  // Creacion del arreglo de procesos
  process_number++; // por proceso idle
  Process** process_array = malloc(process_number * sizeof(Process*));

  // Creacion de queues
  waiting_queue = queue_create(process_number);
  ready_queue = queue_create(process_number);

  //creacion del proceso idle
  process_array[0] = process_idle();

  parse_file(file, process_array, waiting_queue);
  queue_process_print(waiting_queue);
  if (!strcmp(argv[1], "roundrobin")){
    roundrobin_quantum(waiting_queue, quantum);
  }

  printf("IC: %d\n", waiting_queue->item_count);


};
