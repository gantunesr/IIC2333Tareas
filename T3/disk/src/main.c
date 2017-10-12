#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"


int main(int argc, char const *argv[]) {

  // Verificacion del input
  if(argc != 3){
		printf("Modo de uso: %s <politica> <input.txt>\nDonde\n", argv[0]);
		printf("\t<politica> tipo de scheduler(fcfs, sstf, scan o c-look)\n");
    printf("\t<input.txt> ruta al archivo inicial del disco\n");
		return 1;
	}

  // Verify scheduler
  if(strcmp(argv[1], "fcfs") && strcmp(argv[1], "sstf")\
   && strcmp(argv[1], "scan") && strcmp(argv[1], "c-look")){
     printf("Error:\n\t<politica> tipo de scheduler(fcfs, sstf, scan o c-look)\n");
     return 2;
   }

  FILE* disk_file = fopen(argv[2], "r");
  // Si el archivo no existe
  if(!disk_file){
    perror(argv[1]);
    return 2;
  }

  // Load Queue
  int head;
  Queue* queue = init_queue(1);
  char* cilindro = malloc(sizeof(char) * 5);
  fgets(cilindro, 2048, disk_file);
  head = strtol(cilindro, NULL, 10);
  while(fgets(cilindro, 1024, disk_file)){
    insert_queue(strtol(cilindro, NULL, 10), queue);
  }
  free(cilindro);
  fclose(disk_file);
  Disk* disk = init_disk(head, queue->size);

  // Choose scheduler
  if(!strcmp(argv[1], "fcfs")){fcfs(disk, queue);}
  else if(!strcmp(argv[1], "sstf")){sstf(disk, queue);}
  else if(!strcmp(argv[1], "scan")){fun_scan(disk, queue);}
  else{clook(disk, queue);}

  // Final output
  printf("%d", disk->read_array[0]);
  for(int i = 1; i < disk->size; i++){printf(",%d", disk->read_array[i]);}
  printf("\n%d\n", disk->seek_time);
  printf("%dT + %dD\n", disk->seek_time, disk->change_side);
  destroy_disk(disk);
  destroy_queue(queue);

  return 0;
}
