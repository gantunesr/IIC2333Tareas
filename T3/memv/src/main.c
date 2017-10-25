#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "structs.h"

void INTHandler(int);
MMU* mmu;
TLBUnit** tlb;
Page** page_table;
Address* address;

/* INT handler */

void INTHandler(int sig){
    signal(sig, SIG_IGN);
    print_statistics(mmu, tlb, page_table);
    exit(0);
}

int main(int argc, char const *argv[]) {

  signal(SIGINT, INTHandler);

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

  mmu = create_mmu();
  tlb = create_TLB();
  page_table = create_page_table();

  // FIN: Creación de estructuras
  // INICIO: Declaración de variables

  //int frame = 0;
  int enter_tlb = 0;
  int enter_pt = 0;
  int current_frame = 0;

  // FIN: Declaración de variables

  unsigned int input;

  while (1) {
    printf(">> ");
    scanf("%d", &input);
    address = get_address(input);

    if (tlb_hit(tlb, address->number) == 1) {
      mmu->tlb_hit++;
    }

    else {
      mmu->tlb_miss++;
      mmu->page_total++;
      if (not_page_fault(page_table, address->number) == 1) {
        // FIFO
        if (strcmp(argv[1], "fifo") == 0) {
          fifo_tlb(tlb, address->number, current_frame, enter_tlb);
        }
        //LRU
        else {
          lru_tlb(tlb, address->number,  mmu, current_frame);
        }
      }
      else {
        mmu->page_fault++;
        if (strcmp(argv[1], "fifo") == 0) {
          fifo_tlb(tlb, address->number, current_frame, enter_tlb);
          fifo_page_table(page_table, address->number, mmu, current_frame, enter_pt);
          if (current_frame < 256) {
            current_frame++;
          }
        }
        //LRU
        else {
          lru_page_table(page_table, address->number, mmu, current_frame);
          lru_tlb(tlb, address->number,  mmu, current_frame);
          if (current_frame < 256) {
            current_frame++;
          }
        }
      }
    }

    update_access_order_tlb(tlb);
    update_access_order_table(page_table);

    enter_tlb++;
    enter_pt++;


  }

  print_statistics(mmu, tlb, page_table);

  free_mmu(mmu);
  free_TLB(tlb);
  free_table(page_table);
  free(address);

  return 0;
}
