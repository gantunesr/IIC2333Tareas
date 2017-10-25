#include <stdbool.h>

typedef struct Address Address;
typedef struct MMU MMU;
typedef struct Page Page;
typedef struct TLBUnit TLBUnit;
typedef struct Frame Frame;

// structs

struct MMU {
  int tlb_hit;
  int tlb_miss;
  int page_fault;
  int page_total;
};

struct Page {
  int number;
  bool active;
  unsigned int framenumber;
  int access_order;
  int in_order;
};

struct Address {
  unsigned int number:8;
  unsigned int offset:8;
};

struct TLBUnit {
  int pagenumber;
  int framenumber;
  bool active;
  int access_order;
  int in_order;
};

struct Frame {
  int id;
  unsigned char* buffer[256];
};

/* MMU */

MMU* create_mmu();

void free_mmu(MMU* mmu);

/* Page */

Page* create_page();

Page** create_page_table();

bool not_page_fault(Page** page_table, int page_number);

void free_table(Page** table);


/* Address */

Address* create_address(int number, int offset);

void free_adress(Address* address);

Address* get_address(int decimal);

void print_address(Address* address);

long long get_paddress(long long frame, long long offset);

void update_access_order_table(Page** table);

/* TLB */

TLBUnit* create_tlbunit();

TLBUnit** create_TLB();

void free_TLB(TLBUnit** tlb);

int full_TLB (struct TLBUnit** TLB);

void update_access_order_tlb(TLBUnit** tlb);

bool tlb_hit(TLBUnit** tlb, int page_number);

/* Physical Memory */

Frame* create_frame(int id);

Frame** create_pm();

void free_pm(Frame** pm);

// Funciones utiles

int bin_to_dec(long long bin);

long long dec_to_bin(int decimal);

/* LRU & FIFO */

void lru_tlb(TLBUnit** tlb, int address, MMU* mmu, int frame);

void fifo_tlb(TLBUnit** tlb, int address, int frame, int in_order);

void lru_page_table(Page** page_table, int address, MMU* mmu, int frame);

void fifo_page_table(Page** page_table, int address, MMU* mmu, int frame, int in_order);

void print_statistics(MMU* mmu, TLBUnit** tlb, Page** table);
