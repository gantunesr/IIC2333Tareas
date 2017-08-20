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

  //CRIS EDIT --> Una waiting_queue y una ready_queue
  Queue* waiting_queue = queue_create(process_number);
  Queue* ready_queue = queue_create(0);
  //CRIS EDIT

  //creacion del proceso idle
  process_array[0] = process_idle();

  parse_file(file, process_array, waiting_queue);

  queue_process_print(waiting_queue);

  printf("\n\n ----- SIMULATION ----- \n\n");

  struct Process* process;
  struct Process* running_process;
  bool is_running = 0;

  while (!(queue_is_empty(waiting_queue)) || (!queue_is_empty(ready_queue))){
    // Primero deberiamos sumar 1 al current time de los que estan ready

    // 1. Revisar estado de procesos y cambiarlos
    for (int i = 0; i < waiting_queue->item_count; i++){
      process = waiting_queue->array[i];
      process->current_time++;
      process->waiting_time++;
      
      // Revisar los que entran por primera vez
      if (process->init_time == simulation_time){
        // Cambiar de waiting a ready
        //printf("En el tiempo %d\n", simulation_time);
        printf("El proceso %s paso de WAITING a READY\n", process->name);
        process->state = 1;
        // Removerlo de waiting_queue
        remove_element(waiting_queue, i, waiting_queue->item_count);
        // Añadirlo al ready_queue
        queue_insert(ready_queue, process);   
        seqqueue_pop_first(process->sequence);    
      }
      
      // Revisar los que ya pasaron su init_time
      else if (process->init_time <= simulation_time){
        seqqueue_decrease_first(process->sequence);
        if (seqqueue_get_first(process->sequence) == 0){
          // Cambiar de waiting a ready
          //printf("En el tiempo %d\n", simulation_time);
          printf("El proceso %s paso de WAITING a READY\n", process->name);
          process->state = 1;
          // Removerlo de waiting_queue
          remove_element(waiting_queue, i, waiting_queue->item_count);
          // Añadirlo al ready_queue
          queue_insert(ready_queue, process);
          // Sacamos el numero de la secuencia
          seqqueue_pop_first(process->sequence);
        }
      }
    }

    // Si hay alguno corriendo
    if (is_running){
      seqqueue_decrease_first(running_process->sequence);
      // Verificamos si termina su ciclo
      if (seqqueue_get_first(running_process->sequence) == 0){
        is_running = 0;
        // Sacamos el numero de la secuencia
        seqqueue_pop_first(running_process->sequence);
        // Verificamos si termino
        if (seqqueue_is_empty(running_process->sequence)){
          //printf("En el tiempo %d\n", simulation_time);
          printf("El proceso %s paso de RUNNING a DEAD\n", running_process->name);
          running_process->state = 3;
        }
        else {
          //printf("En el tiempo %d\n", simulation_time);
          printf("El proceso %s paso de RUNNING a WAITING\n", running_process->name); 
          running_process->state = 2;
          running_process->CPU_blocked_times++;  //No estoy seguro si es este
          // Lo agregamos a waiting_queue
          queue_insert(waiting_queue, running_process);
        }
        is_running = 0;
      }
    }

    // Aca entra el scheduler
    else {
      
      // Ver el siguiente en ser atendido
      if (!queue_is_empty(ready_queue)){
        if (!strcmp(argv[1], "fcfs")) {
            running_process = queue_pop_front(ready_queue);
            //printf("En el tiempo %d\n", simulation_time);
            printf("El proceso %s paso de READY a RUNNING\n", running_process->name);
            running_process->state = 0;
            running_process->CPU_selected_times++;
            is_running = 1;
          }
        }

        else if (!strcmp(argv[1], "roundrobin")) {
        }

        else {

        }
    }

    simulation_time++;
  }
  // Deberiamos llamar al idle, pero por ahora tengo que termine
  printf("Queue vacia\n");

  return 0;

};