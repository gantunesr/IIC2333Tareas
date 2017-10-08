#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct MMU MMU;
typedef struct page page;
typedef struct Address Address;


/* MMU */

/* MMU para manejar hits y page faults */
struct MMU {
  int tlb_hit;
  int tlb_miss;
  int page_fault;
  int page_total;
};

MMU* create_mmu() {
  MMU* mmu = malloc(sizeof(MMU));
  mmu->tlb_miss = 0;
  mmu->page_fault = 0;
  mmu->page_total = 0;
  return mmu;
}

void free_mmu(MMU* mmu) {
  free(mmu);
}

/* Memoria virtual */

struct page {
  int number;
  bool active;
  int framenumber;
  int order;
};

page* create_page() {
  page* page = malloc(sizeof(page));

  return page;
}

void free_page(page* page) {
  free(page);
}

/* */

struct Address {
  int number;
  int offset;
};

Address* create_address(int number, int offset) {
    Address* address = malloc(sizeof(struct Address));
    address->number = number;
    address->offset = offset;
    return address;
}

/* Funciones útiles */

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
    remain = decimal%2;
    decimal /= 2;
    bin += remain*i;
    i *= 10;
  }
  return create_address(bin_to_dec(bin%100000000), bin_to_dec(bin/100000000));
}

// Función para obtener la dirección en la memoria física
long long get_paddress(long long frame, long long offset) {
  long long last_bits = offset*100000000;
  return frame+last_bits;
}
