#include <stdlib.h>
#include "queue.h"


Queue* init_queue(int max){
  Queue* queue = malloc(sizeof(Queue));
  queue->size = 0;
  queue->max = max;
  queue->sockets = malloc(sizeof(int) * max);
  return queue;
}

void insert_queue(Queue* queue, int socket){
  if(queue->size == queue->max){
    queue->max *= 2;
    int* tmp = malloc(sizeof(int) * queue->max);
    for(int i = 0; i < queue->size; i++){tmp[i] = queue->sockets[i];}
    free(queue->sockets);
    queue->sockets = tmp;
  }
  queue->sockets[queue->size] = socket;
  queue->size++;
}

int pop_first(Queue* queue){
  if(queue->size){
    int socket = queue->sockets[0];
    for(int i = 1; i < queue->size; i++){
      queue->sockets[i - 1] = queue->sockets[i];
    }
    queue->size--;
    return socket;
  }
  return 0;
}

void destroy_queue(Queue* queue){
  free(queue->sockets);
  free(queue);
}
