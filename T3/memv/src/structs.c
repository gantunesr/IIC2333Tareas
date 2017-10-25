#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "structs.h"


/* MMU */
MMU* create_mmu() {
  MMU* mmu = malloc(sizeof(MMU));
  mmu->tlb_hit = 0;
  mmu->tlb_miss = 0;
  mmu->page_fault = 0;
  mmu->page_total = 0;
  return mmu;
}

void free_mmu(MMU* mmu) {
  free(mmu);
}

/* Memoria virtual */

Page* create_page() {
  Page* page = malloc(sizeof(Page));
  page->access_order = 0;
  page->active = 0;
  page->framenumber = 0;
  page->in_order = 0;
  page->number = 0;
  return page;
}

Page** create_page_table() {
  Page** table = malloc(256 * sizeof(Page));
  for (int page = 0; page < 256; page++) {
    table[page] = create_page();
  }
  return table;
}

void free_table(Page** table) {
  for (int page = 0; page < 256; page++) {
    free(table[page]);
  }
  free(table);
}

void update_access_order_table(Page** table) {
  for (int i = 0; i < 256; i++) {
    if (table[i]->active) {
      table[i]->access_order++;
    }
  }
}

bool not_page_fault(Page** page_table, int page_number) {
  for (int i = 0; i < 256; i++) {
    if (page_table[i]->active && page_table[i]->number == page_number) {
      return 1;
    }
  }
  return 0;
}

/* Address*/
Address* create_address(int number, int offset) {
  Address* address = malloc(sizeof(Address));
  address->number = number;
  address->offset = offset;
  return address;
}

/* TLB */

TLBUnit* create_tlbunit() {
  TLBUnit* unit = malloc(sizeof(TLBUnit));
  unit->pagenumber = 0;
  unit->framenumber = 0;
  unit->active = 0;
  unit->access_order = 0;
  return unit;
}

TLBUnit** create_TLB() {
  TLBUnit** tlb = malloc(32 * sizeof(TLBUnit));
  for (int i = 0; i < 32; i++) {
    tlb[i] = create_tlbunit();
  }
  return tlb;
}

bool tlb_hit(TLBUnit** tlb, int page_number) {
  for (int i = 0; i < 32; i++) {
    if (tlb[i]->active && tlb[i]->pagenumber == page_number) {
      tlb[i] -> access_order = 0;
      return 1;
    }
  }
  return 0;
}

void free_TLB(TLBUnit** tlb) {
  for (int i = 0; i < 32; i++) {
    free(tlb[i]);
  }
  free(tlb);
}

// Función para verificar si la TLB esta llena
int full_TLB (struct TLBUnit** TLB) {
  for (int i = 0; i < 32; i++) {
    if (TLB[i]->active == 0) {
      return i;
    }
  }
  return -1;
}

void update_access_order_tlb(TLBUnit** tlb) {
  for (int i = 0; i < 32; i++) {
    if (tlb[i]->active) {
      tlb[i]->access_order++;
    }
  }
}

/* Physical Memory */

Frame* create_frame(int id) {
  Frame* frame = malloc(sizeof(Frame));
  frame->id = id;
  return frame;
}

Frame** create_pm() {
  Frame** pm = malloc(128*sizeof(Frame));
  for (int i = 0; i < 128; i++) {
    pm[i] = create_frame(i);
  }
  return pm;
}

void free_pm(Frame** pm) {
  for (int i = 0; i < 128; i++) {
    free(pm[i]);
  }
  free(pm);
}

/* Funciones útiles */

// Función para verificar si tabla de páginas esta llena
int full_table (struct Page** page_table) {
  for (int i = 0; i < 256; i++) {
    if (!page_table[i]->active) {
      return i;
    }
  }
  return -1;
}

// Función para convertir un binario a decimal
int bin_to_dec(long long bin) {
  int decimal = 0;
  int i = 0;
  int remain;

  while (bin != 0) {
    remain = bin%10;
    bin /= 10;
    decimal += remain*pow(2, i);
    i++;
  }

  return decimal;
}

// Función para convertir decimal a binario
long long dec_to_bin(int decimal) {
  long long bin = 0;
  double i = 1;
  int remain;

  while (decimal != 0) {
    remain = decimal%2;
    decimal /= 2;
    bin += remain*i;
    i *= 10;
  }
  return bin;
}

// Función para obtener el número de página y el offset
Address* get_address(int decimal) {
  long long bin = 0;
  double i = 1;
  int remain;

  while (decimal != 0) {
    remain = decimal % 2;
    decimal /= 2;
    bin += remain*i;
    i *= 10;
  }
  return create_address(bin_to_dec(bin%100000000), bin_to_dec(bin/100000000));
}

void print_address(Address* address) {
  printf("Page number: %d\n", address->number);
  printf("Offset: %d\n", address->offset);
}

// Función para obtener la dirección en la memoria física
long long get_paddress(long long frame, long long offset) {
  long long last_bits = offset*100000000;
  return frame+last_bits;
}

/* LRU & FIFO */

void lru_tlb(TLBUnit** tlb, int address, MMU* mmu, int frame) {
  int pos = full_TLB(tlb);
  if (pos == -1) {
    TLBUnit* unit = tlb[0];
    for (int i = 0; i < 32; i++) {
      if (tlb[i]->access_order > unit->access_order) {
        unit = tlb[i];
      }
      unit->access_order = 0;
      unit->pagenumber = address;
    }
  }
  else {
    tlb[pos]->access_order = 0;
    tlb[pos]->active = 1;
    tlb[pos]->framenumber = frame;
    tlb[pos]->pagenumber = address;
  }
}

void fifo_tlb(TLBUnit** tlb, int address, int frame, int in_order) {
  int pos = full_TLB(tlb);
  if (pos == -1) {
    TLBUnit* unit = tlb[0];
    for (int i = 0; i < 32; i++) {
      if (tlb[i]->in_order < unit->in_order) {
        unit = tlb[i];
      }
      unit->in_order = 0;
      unit->pagenumber = address;
      unit->in_order = in_order;
    }
  }
  else {
    tlb[pos]->access_order = 0;
    tlb[pos]->active = 1;
    tlb[pos]->in_order = in_order;
    tlb[pos]->framenumber = frame;
    tlb[pos]->pagenumber = address;
  }

}

void lru_page_table(Page** page_table, int address, MMU* mmu, int frame) {
  int pos = full_table(page_table);
  if (pos == -1) {
    Page* page = page_table[0];
    for (int i = 0; i < 256; i++) {
      if (page_table[i]->access_order > page->access_order) {
        page = page_table[i];
      }
      page->access_order = 0;
      page->number = address;
      page->framenumber = frame;
    }
  }
  else {
    page_table[pos]->access_order = 0;
    page_table[pos]->active = 1;
    page_table[pos]->framenumber = frame;
    page_table[pos]->number = address;
  }
}

void fifo_page_table(Page** page_table, int address, MMU* mmu, int frame, int in_order) {
  int pos = full_table(page_table);
  if (pos == -1) {
    Page* page = page_table[0];
    for (int i = 0; i < 256; i++) {
      if (page_table[i]->in_order > page->in_order) {
        page = page_table[i];
      }
      page->access_order = 0;
      page->number = address;
      page->framenumber = frame;
      page->in_order = in_order;
    }
  }
  else {
    page_table[pos]->access_order = 0;
    page_table[pos]->active = 1;
    page_table[pos]->framenumber = frame;
    page_table[pos]->number = address;
  }
}

void print_statistics(MMU* mmu, TLBUnit** tlb, Page** table) {
  printf("\n\nPORCENTAJE_PAGE_FAULTS = %d%%\n", 100*mmu->page_fault/mmu->page_total);
  printf("PORCENTAJE_TLB_HITS = %d%%\n", 100*mmu->tlb_hit/(mmu->tlb_hit+mmu->tlb_miss));
  printf("TABLA DE PÁGINAS\n");
  printf("page_number         frame_number\n");
  for (int i = 0; i < 256; i++) {
  printf(" %d                     %d\n", table[i]->number, table[i]->framenumber);
  }
  printf("TLB\n");
  printf("i           page_number         frame_number\n");
  for (int i = 0; i < 32; i++) {
    printf("%d            %d                  %d\n", i, tlb[i]->pagenumber, tlb[i]->framenumber);
  }
}
