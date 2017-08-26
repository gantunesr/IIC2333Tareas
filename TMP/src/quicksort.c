#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "structs.c"

void swap(Queue* queue, int index_process1, int index_process2) {
   Process* p_temp;
   p_temp = queue->array[index_process1];
   queue->array[index_process1] = queue->array[index_process2];
   queue->array[index_process2] = p_temp;
   return;
}

int partition(Queue* queue, int left, int right, Process* pivot) {

   int leftPointer = left;
   int rightPointer = right - 1;

   while(true) {

      while(queue->array[leftPointer]->priority > pivot->priority) {
         leftPointer++;
      }

      while(rightPointer > 0 && queue->array[rightPointer]->priority <= pivot->priority) {
         rightPointer--;
      }

      if(leftPointer >= rightPointer) {
         printf("PIVOT %d\n", pivot->priority);
         printf("REVISAR item_count: %d\n", queue->item_count);
         for (int i = 0; i < queue->item_count; i++){
           printf("Prioridad: %d\n", queue->array[i]->priority);
         }
         break;
      } else {
         swap(queue, leftPointer, rightPointer);
      }
   }
   swap(queue, leftPointer, right);
   return leftPointer;
}

void quickSort(Queue* queue, int left, int right) {
   if(right-left < 1) {
      return;
   } else {
      Process* pivot = queue->array[right];
      int partitionPoint = partition(queue, left, right, pivot);
      quickSort(queue, left, partitionPoint-1);
      quickSort(queue, partitionPoint+1,right);
   }
}
