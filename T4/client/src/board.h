#include <wchar.h>


struct Board{
  wint_t cells[8][8];
};

typedef struct Board Board;

Board* init_board();

void init_pieces(Board* board);

void print_board(Board* board);

void destroy_board(Board* board);
