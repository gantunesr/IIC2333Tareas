#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./structs.c"

void lru() {

}

void fifo() {

}

int main(int argc, char const *argv[]) {

  if ( argc < 2) {
    printf("ERROR: enter all parameters...\n");
    return 0;
  }

  // INICIO: Creación de estructuras

  MMU* mmu = create_mmu();

  // FIN: Creación de estructuras

  int input;

  // while (1) {
    if (strcmp(argv[1], "fifo") == 0) {
      scanf("%d", &input);
    }
    else if (strcmp(argv[1], "lru") == 0) {
      scanf("%d", &input);
    }
    else {
      printf("Invalid TLB politic\n");
      return 0;
    }
  // }

  free(mmu);

  return 0;
}
