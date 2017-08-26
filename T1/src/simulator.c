#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "structs.c"


#define RUNNING 0;
#define READY 1;
#define WAITING 2;
#define DEAD 3;

void INTHandler(int);
Queue* waiting_queue;
Queue* ready_queue;
Queue* end_queue;
Process** process_array;
int simulation_time, end_process;


// --------------------------  PARSER  -----------------------------------

int parse_file (FILE* file, Process** process_array, Queue* queue) {

  char process_char[2000];
  char process_name[256];
  int priority;
  int init_time;
  int pid = 1;
  int count;
  int last_space = 0;
  int process_char_length = 0 ;

  Process* process;
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
    process_array[pid] = process;
    queue_insert(queue, process);
    pid++;
  };
  fclose(file);
  return 0;
};

// --------------------------  ROUNDROBIN  -----------------------------

void roundrobin_quantum(Queue* queue, int quantum){
  Process* process;
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

// --------------------------  INTHANDLER ------------------------------
void INTHandler(int sig){
    signal(sig, SIG_IGN);
    printf("INTERRUPTED\n"
           "SIMULATION TIME: %d\n"
           "FINISHED PROCESS: %d\n\n", simulation_time, end_process);
    printf(" ------ PROCESOS SIN TERMINAR --------\n");
    for (int i = 0; i < waiting_queue->item_count; i++){
      interrupted_process(waiting_queue->array[i]);
      process_destroy(waiting_queue->array[i]);
    }
    for (int i = 0; i < ready_queue->item_count; i++){
      interrupted_process(ready_queue->array[i]);
      process_destroy(ready_queue->array[i]);
    }
    printf(" ------ PROCESOS TERMINADOS --------\n");
    for (int i = 0; i < end_queue->item_count; i++){
      process_print_final_info(end_queue->array[i]);
      process_destroy(end_queue->array[i]);
    }
    queue_destroy(end_queue);
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

  // VERIFICACION DEL INPUT
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
  simulation_time = 0;
  end_process = 0;

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
  end_queue = queue_create(process_number);
  //creacion del proceso idle
  process_array[0] = process_idle();
  parse_file(file, process_array, waiting_queue);
  queue_process_print(waiting_queue);
  if (!strcmp(argv[1], "roundrobin")){
    roundrobin_quantum(waiting_queue, quantum);
  }


  printf("\n\n ----- SIMULATION ----- \n\n");
  Process* process;
  Process* running_process;
  bool is_running = 0;
  int index = 0;
  while (!queue_is_empty(waiting_queue) || \
  !queue_is_empty(ready_queue) || is_running){
    // Primero sumamos waiting time a los que estan ready
    for (int i = 0; i < ready_queue->item_count; i++){
      process = waiting_queue->array[i];
      process->waiting_time++;
    }
    // 1. Revisar estado de procesos y cambiarlos
    for (int i = 0; i < waiting_queue->item_count; i++){
      process = waiting_queue->array[i];
      process->waiting_time++;

      // Revisar los que entran por primera vez
      if (process->init_time == simulation_time){

        // Cambiar de waiting a ready
        printf("NOTIFICACIÓN: El proceso %s paso de WAITING a READY en la iteración %d\n",
                process->name, simulation_time);
        process->state = READY;
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
          // Cambiar de waiting a read
          printf("NOTIFICACIÓN: El proceso %s paso de WAITING a READY en la iteración %d\n",
                  process->name, simulation_time);
          process->state = READY;
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
      running_process->run_time++;
      running_process_print(running_process);
      // Verificamos si termina su ciclo
      if (seqqueue_get_first(running_process->sequence) == 0){
        is_running = 0;
        // Sacamos el numero de la secuencia
        seqqueue_pop_first(running_process->sequence);
        // Verificamos si termino
        if (seqqueue_is_empty(running_process->sequence)){
          printf("NOTIFICACIÓN: El proceso %s paso de RUNNING a DEAD en la iteración %d\n",
                  running_process->name, simulation_time);
          running_process->turnaround_time = simulation_time - process->init_time;
          running_process->state = DEAD;
          queue_insert(end_queue, running_process);
          end_process++;
        }
        else {
          printf("NOTIFICACIÓN: El proceso %s paso de RUNNING a WAITING en la iteración %d\n",
                  running_process->name, simulation_time);
          running_process->state = WAITING;
          running_process->CPU_blocked_times++;
          // Lo agregamos a waiting_queue
          queue_insert(waiting_queue, running_process);
        }
      }
      if (!strcmp(argv[1], "roundrobin") && is_running){
        running_process->actual_q--;
        if (!running_process->actual_q){
          printf("NOTIFICACIÓN: El proceso %s paso de RUNNING a READY en la iteración %d\n",
                running_process->name, simulation_time);
          running_process->state = READY;
          running_process->CPU_blocked_times++;
          // Lo agregamos a waiting_queue
          queue_insert(ready_queue, running_process);
          is_running = 0;
        }
      }
    }

    // Aca entra el scheduler
    else {
      // Ver el siguiente en ser atendido
      if (!queue_is_empty(ready_queue)){
        if (!strcmp(argv[1], "fcfs")) {
          running_process = queue_pop_front(ready_queue);
          //printf("En el tiempo %d\n", simulation_time);
          }
        // RR y priority entran al else
        else {
          index = highest_priority_process_index(ready_queue);
          running_process = get_element(ready_queue, index, ready_queue->item_count);
        }
        if (!strcmp(argv[1], "roundrobin")){
          running_process->actual_q = running_process->quantum;
        }
        if (process_array[0]->state == 0){
          process_array[0]->state = READY;
          process_array[0]->CPU_blocked_times++;
          printf("NOTIFICACIÓN: El proceso %s paso de RUNNING a READY en la iteración %d\n",
                      process_array[0]->name, simulation_time);
        }
        printf("NOTIFICACIÓN: El proceso %s paso de READY a RUNNING en la iteración %d\n",
                    running_process->name, simulation_time);
        running_process->state = RUNNING;
        running_process->CPU_selected_times++;
        is_running = 1;
        // Ver el response_time la primera vez
        if (!running_process->response_time){
          running_process->response_time = simulation_time - running_process->init_time;
        }
      }
      else {
        process_array[0]->CPU_selected_times++;
      }
    }

    if (!is_running && queue_is_empty(ready_queue)) {
      process_array[0]->run_time++;
      if (process_array[0]->state == 1){
        process_array[0]->CPU_selected_times++;
        printf("NOTIFICACIÓN: El proceso %s paso de READY a RUNNING en la iteración %d\n",
                    process_array[0]->name, simulation_time);
        process_array[0]->state = RUNNING;
      }
    } else {process_array[0]->waiting_time++;}

    simulation_time++;
  }
  printf("\n\n -----FIN SIMULATION ----- \n\n");
  printf("SIMULATION TIME: %d\n"
         "FINISHED PROCESS: %d\n\n", simulation_time, end_process);
  process_print(process_array[0]);
  for (int i = 0; i < end_queue->item_count; i++){
    process_print_final_info(end_queue->array[i]);
    process_destroy(end_queue->array[i]);
  }
  queue_destroy(end_queue);
  queue_destroy(waiting_queue);
  queue_destroy(ready_queue);
  free(process_array[0]);
  free(process_array);
  return 0;

};
