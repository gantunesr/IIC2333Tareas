#include <wchar.h>
#include <stdint.h>

typedef struct Board Board;
typedef struct Piece Piece;

struct Board{
  Piece*** cells;
};

struct Piece{
  uint8_t r;
  uint8_t c;
  int player; // 1 -> black; 2 -> white
  wint_t piece;
  bool alive;
};

Board* init_board();

void init_pieces(Board* board);

void print_board(Board* board);

void destroy_board(Board* board);

bool valid_move(Board* board, Piece* piece, uint8_t c, uint8_t r);

int get_column(char c[0]);

bool in_board(uint8_t r, uint8_t c);

bool valid_pawn_move(Piece* piece, uint8_t c, uint8_t r, bool oc);

bool valid_tower_move(Board* board, Piece* piece, uint8_t c, uint8_t r, bool oc);

bool valid_horse_move(Piece* piece, uint8_t c, uint8_t r, bool oc);

bool valid_bishop_move(Board* board, Piece* piece, uint8_t c, uint8_t r, bool oc);

bool valid_queen_move(Piece* piece, uint8_t c, uint8_t r, bool oc);

bool valid_king_move(Board* board, Piece* piece, uint8_t c, uint8_t r, bool oc);
