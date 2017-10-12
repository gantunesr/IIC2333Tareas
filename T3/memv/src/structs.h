#include <stdbool.h>


struct MMU {
  int tlb_hit;
  int tlb_miss;
  int page_fault;
  int page_total;
};
typedef struct MMU MMU;

MMU* create_mmu();

void free_mmu(MMU* mmu);


struct page {
  int number;
  bool active;
  unsigned int framenumber;
  int order;
};
typedef struct page page;

page* create_page();

void free_page(page* page);

struct Address {
  unsigned int number:8;
  unsigned int offset:8;
};
typedef struct Address Address;

Address* create_address(int number, int offset);


// Funciones utiles
int bin_to_dec(long long bin);

long long dec_to_bin(int decimal);

Address* get_address(int decimal);

long long get_paddress(long long frame, long long offset);
