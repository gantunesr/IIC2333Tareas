#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"


void lru();
void fifo();


void lru() {}

void fifo() {}

int main(int argc, char const *argv[]) {

  if ( argc < 2) {
    printf("Modo de uso: %s <politica> < <input.txt>\nDonde\n", argv[0]);
    printf("\t<politica> politica de swapping (lru, fifo)\n");
    printf("\t<input.txt> (opcional) ruta al archivo inicial del programa\n");
    return 0;
  }
  if (strcmp(argv[1], "fifo") && strcmp(argv[1], "lru")){
    printf("Error\n\t Politicas validos son lru o fifo\n");
  }

  // INICIO: Creación de estructuras

  MMU* mmu = create_mmu();

  // FIN: Creación de estructuras

  unsigned int input;

  while (1) {
    scanf("%d", &input);
  }

  free(mmu);

  return 0;
}
