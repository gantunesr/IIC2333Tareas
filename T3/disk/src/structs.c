#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

// Queue Methods
Queue* init_queue(int max){
  Queue* queue = malloc(sizeof(Queue));
  queue->size = 0;
  queue->max = max;
  queue->array = malloc(sizeof(int) * max);
  return queue;
}

void insert_queue(int number, Queue* queue){
  if(queue->size == queue->max){
    queue->max *= 2;
    int* tmp = malloc(sizeof(int) * queue->max);
    for(int i = 0; i < queue->size; i++){tmp[i] = queue->array[i];}
    free(queue->array);
    queue->array = tmp;
  }
  queue->array[queue->size] = number;
  queue->size++;
}

int get_min_time(Queue* queue, int head){
  int min = abs(queue->array[0] - head);
  int index = 0;
  for(int i = 1; i < queue->size; i++){
    if(abs((queue->array[i] - head)) < min){
      min = queue->array[i] - head;
      index = i;
    }
  }
  min = queue->array[index];
  if(index != queue->size - 1){
    for(int i = index + 1; i < queue->size; i++){
      queue->array[i - 1] = queue->array[i];
    }
  }
  queue->size--;
  return min;
}

int get_index(Queue* queue, int head){
  for(int i = 0; i < queue->size; i++){
    if(queue->array[i] >= head){
      return i;
    }
  }
  return queue->size - 1;
}

int compare(const void *a, const void *b){
  int *x = (int *) a;
  int *y = (int *) b;
  return *x - *y;
}

void destroy_queue(Queue* queue){
  free(queue->array);
  free(queue);
}


// Disk Methods
Disk* init_disk(int head, int size_queue){
  Disk* disk = malloc(sizeof(Disk));
  disk->head = head;
  disk->read_array = malloc(size_queue * sizeof(int));
  disk->size = size_queue;
  disk->actual = 0;
  disk->seek_time = 0;
  disk->change_side = 0;
  return disk;
}

void destroy_disk(Disk* disk){
  free(disk->read_array);
  free(disk);
}


// Scheduler main methods
void fcfs(Disk* disk, Queue* queue){
  while(disk->size > disk->actual){
    compare_cilinders(disk, queue->array[disk->actual]);
  }
}

void sstf(Disk* disk, Queue* queue){
  while(queue->size){
      compare_cilinders(disk, get_min_time(queue, disk->head));
  }
}

void fun_scan(Disk* disk, Queue* queue){
  qsort(queue->array, disk->size, sizeof(int), compare);
  int index = get_index(queue, disk->head);
  if(!(disk->size - 1 - index)){index++;}
  for(int i = index; i < disk->size; i++){
    compare_cilinders(disk, queue->array[i]);
  }
  disk->seek_time += (255 - disk->head);
  disk->head = 255;
  for(int i = index - 1; i >= 0; i--){
    compare_cilinders(disk, queue->array[i]);
  }
}

void clook(Disk* disk, Queue* queue){
  qsort(queue->array, disk->size, sizeof(int), compare);
  int index = get_index(queue, disk->head);
  if(!(disk->size - 1 - index)){index++;}
  for(int i = index; i < disk->size; i++){
    compare_cilinders(disk, queue->array[i]);
  }
  for(int i = 0; i < index; i++){
    compare_cilinders(disk, queue->array[i]);
  }
}

void compare_cilinders(Disk* disk, int cilinder){
  if(cilinder >= disk->head){
    disk->seek_time += (cilinder - disk->head);
    if(disk->change_side % 2){disk->change_side++;}
  }
  else{
    disk->seek_time += (disk->head - cilinder);
    if(!(disk->change_side % 2)){disk->change_side++;}
  }
  disk->head = cilinder;
  disk->read_array[disk->actual] = cilinder;
  disk->actual++;
}
