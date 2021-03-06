#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


// SEQUENCE QUEUE
struct SeqQueue {
  int MAX;
  int front;
  int rear;
  int item_count;
  int total_time;
  int* array;
};
typedef struct SeqQueue SeqQueue;

bool seqqueue_is_empty(SeqQueue* seqqueue) {
  return seqqueue->item_count == 0;
};

bool seqqueue_is_full(SeqQueue* seqqueue) {
   return seqqueue->item_count == seqqueue -> MAX;
};

int seqqueue_insert(SeqQueue* seqqueue, int data) {
  seqqueue->array[++(seqqueue->rear)] = data;
  seqqueue->item_count++;
  return 0;
};

SeqQueue* seqqueue_create(int sequence_length, int* sequence, int init_time) {
  sequence_length = sequence_length*2 - 1;
  SeqQueue* seqqueue = malloc(sizeof(SeqQueue));
  seqqueue->MAX = sequence_length + 3;
  seqqueue->total_time = 0;
  seqqueue->front = 0;
  seqqueue->rear = -1;
  seqqueue->item_count = 0;
  seqqueue->array = malloc((sequence_length+2) * sizeof(int));
  seqqueue_insert(seqqueue, init_time);
  for (int i = 0; i < sequence_length; i++) {
    seqqueue_insert(seqqueue, sequence[i]);
    if (!(i % 2)){seqqueue->total_time += sequence[i];}
  }
  seqqueue_insert(seqqueue, 0);
  // Hay que restar 1 porque al agregar el 0 al final se esta "agrandando" la queue
  seqqueue->item_count--;
  return seqqueue;
};

void seqqueue_decrease_first(SeqQueue* seqqueue){
  seqqueue->array[seqqueue->front]--;
  return;
}

int seqqueue_get_first(SeqQueue* seqqueue) {
  return seqqueue->array[seqqueue->front];
}

int seqqueue_pop_first(SeqQueue* seqqueue) {
  int front = seqqueue->array[(seqqueue->front)++];
  seqqueue->item_count--;
  return front;
}

void seqqueue_print(SeqQueue* seqqueue) {
  if (seqqueue) {
    printf("Sequence");
    for (int i = seqqueue->front; i < seqqueue->MAX - 1; i++) {
      printf(" => %d ", seqqueue->array[i]);
    };
  }
};


// PROCESS
struct Process {
  int PID;
  int priority;
  char *name;
  // 0 RUNNING ; 1 READY ; 2 WAITING ; 3 DEAD
  char state;
  // Parte de la modelacion de los tiempos de ready o waiting para procesos //
  SeqQueue *sequence;
  int quantum;
  int actual_q;
  int run_time;
  int init_time;
  bool in_queue;
  int CPU_selected_times;
  int CPU_blocked_times;
  int turnaround_time;
  bool CPU_processed;
  int response_time;
  int waiting_time;
};
typedef struct Process Process;

Process* process_create (int PID, int priority, char *name, char state, int *sequence,
          int init_time, int sequence_length) {
  Process* process = malloc(sizeof(Process));
  process->PID = PID;
  process->priority = priority;
  process->state = state;
  process->quantum = 0;
  process->actual_q = 0;
  process->run_time = 0;
  process->in_queue = 0;
  process->init_time = init_time;
  process->name = malloc(256 * sizeof(char));
  strncpy(process->name, name, 256);
  process->sequence = seqqueue_create(sequence_length, sequence, init_time);
  process->CPU_selected_times = 0;
  process->CPU_blocked_times = 0;
  process->turnaround_time = 0;
  process->CPU_processed = 0;
  process->response_time = 0;
  process->waiting_time = 0;
  return process;
};

void process_print (Process* process) {
  printf("-------\n");
  printf("Proceso: %s\n", process->name);
  if (process->state != -1) {
    printf(" PID: %d Priority: %d State: %d\n",
          process->PID, process->priority, process->state);
    seqqueue_print(process->sequence);
    printf("\n");
    printf("Total time: %d\n", process->sequence->total_time);
    printf("Run time: %d\n", process->run_time);
    if (process  -> state == 0) {
      printf("Le quedan %d intervalos\n", process->sequence->total_time - process->run_time);
    }
    printf("Numero de veces seleccionado por CPU: %d\n", process->CPU_selected_times);
    printf("Numero de veces bloqueado por CPU: %d\n", process->CPU_blocked_times);
    printf("Turnaround time: %d\n", process->turnaround_time);
    printf("Response time: %d\n", process->response_time);
    printf("Waiting time: %d\n", process->waiting_time);
    printf("-------\n");
    printf("\n");
  }
  else {
    printf("Numero de veces seleccionado por CPU: %d\n", process->CPU_selected_times);
    printf("Run time: %d\n", process->run_time);
    printf("-------\n");
    printf("\n");
  }
};

void running_process_print(Process* process){
  printf(" PID: %d \n", process->PID);
  printf("RUN_TIME: %d\n", process->run_time);
  printf("Intervalos faltantes: %d\n", process->sequence->total_time - process->run_time);
  }

void process_print_final_info (Process* process) {
  if (process -> state != -1) {
    printf("Proceso: %s\n", process -> name);
    printf("-------\n");
    printf("Numero de veces seleccionado por CPU: %d\n", process->CPU_selected_times);
    printf("Numero de veces bloqueado por CPU: %d\n", process->CPU_blocked_times);
    printf("Turnaround time: %d\n", process->turnaround_time);
    printf("Response time: %d\n", process->response_time);
    printf("Waiting time: %d\n", process->waiting_time);
    printf("-------\n");
    printf("\n");
  }
};

void interrupted_process(Process* process){
  printf("Proceso: %s\n", process -> name);
  printf("-------\n");
  printf("Run Time: %d\n", process->run_time);
  printf("Intervalos faltantes: %d\n", process->sequence->total_time - process->run_time);
  printf("Numero de veces seleccionado por CPU: %d\n", process->CPU_selected_times);
  printf("Numero de veces bloqueado por CPU: %d\n", process->CPU_blocked_times);
  printf("Turnaround time: %d\n", process->turnaround_time);
  printf("Response time: %d\n", process->response_time);
  printf("Waiting time: %d\n", process->waiting_time);
  printf("-------\n");
  printf("\n");
}

void process_destroy(Process* process) {
  free(process->name);
  free(process->sequence->array);
  free(process->sequence);
  free(process);
};

Process* process_idle() {
  Process* idle_process = malloc(sizeof(Process));
  idle_process->name = "IDLE";
  idle_process->PID = 0;
  idle_process->priority = -1;
  idle_process->quantum = 0;
  idle_process->sequence = NULL;
  idle_process->state = -1;
  idle_process->CPU_blocked_times = 0;
  idle_process->CPU_selected_times = 0;
  idle_process->turnaround_time = 0;
  idle_process->CPU_processed = 0;
  idle_process->response_time = 0;
  idle_process->waiting_time = 0;
  idle_process->run_time = 0;
  return idle_process;
}

// QUEUE
struct Queue {
  int MAX;
  int front;
  int rear;
  int item_count;
  Process** array;
};
typedef struct Queue Queue;

Queue* queue_create(int MAX) {
  Queue* queue = malloc(sizeof(Queue));
  queue->MAX = MAX;
  queue->front = 0;
  queue->rear = -1;
  queue->item_count = 0;
  queue->array = malloc(MAX * sizeof(Process*));
  return queue;
};

bool queue_is_empty(Queue* queue) {
   // return queue->front == queue->MAX;
  return queue->item_count == 0;
};

bool queue_is_full(Queue* queue) {
   return queue->rear == queue->MAX-1;
};

void queue_insert(Queue* queue, Process* process) {
  int MAX = queue->MAX;
  if (queue_is_full(queue)) {
    ///la idea es duplicar el tamaño y copiar todos sus elementos en ella
    Process** array_ = malloc(MAX * 2 * sizeof(Process*));
    for (int i = 0; i < queue->MAX; i++) {
      array_[i] =  queue->array[i];
    }
    queue->MAX = queue->MAX * 2;
    free(queue->array);
    queue->array = malloc(MAX * 2 * sizeof(Process*));
    for (int i = 0; i < queue->MAX; i++) {
      queue->array [i] = array_[i];
    }
    free(array_);
  }

  process->in_queue = 1;
  queue->array[++(queue->rear)] = process;
  queue->item_count++;
}

void remove_element(Queue* queue, int index, int array_length){
  for(int i = index; i < array_length - 1; i++) {
    queue->array[i] = queue->array[i + 1];
  }
  queue->rear--;
  queue->item_count--;
  return;
}

Process* get_element(Queue* queue, int index, int array_length){
  Process* process = queue->array[index];
  for(int i = index; i < array_length - 1; i++) {
    queue->array[i] = queue->array[i + 1];
  }
  queue->rear--;
  queue->item_count--;
  return process;
}

Process* queue_pop_front(Queue* queue) {
   Process* front_process = queue -> array[(queue -> front)++];
   queue -> item_count--;
   front_process -> in_queue = 0;
   return front_process;
}

Process* queue_get_front(Queue* queue) {
   Process* front_process = queue -> array[(queue -> front)];
   return front_process;
}

void queue_process_print (Queue* queue) {
  int index = 0;
  for (int i = queue -> front; i < queue -> rear+1; i++) {
    printf("EN POSICION %d:\n", index);
    process_print(queue -> array[i]);
    index++;
  }
  if (index == 0){printf("LA COLA ESTA VACIA\n\n" );}
}

void queue_destroy(Queue* queue) {
  free(queue -> array);
  free(queue);
};
