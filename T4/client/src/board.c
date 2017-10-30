#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include"board.h"


Board* init_board(){
   Board* board = malloc(sizeof(Board));
   init_pieces(board);
   return board;
 }

Piece* init_piece(int r, int c, int player, wint_t piece) {
   Piece* new = malloc(sizeof(Piece));
   new->c = c;
   new->r = r;
   new->player = player;
   new->piece = piece;
   new->alive = true;
   return new;
 }

 Piece* null_piece() {
   Piece* null = malloc(sizeof(Piece));
   null->c = 0;
   null->r = 0;
   null->player = 0;
   null->piece = 0;
   null->alive = false;
   return null;
 }

void init_pieces(Board* board){

  board->cells = malloc(8 * sizeof(Piece));
  for (int i = 0; i < 8; i++) {
    board->cells[i] = malloc(8 * sizeof(Piece));
  }

  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      board->cells[i][j] = null_piece();
    }
  }

   board->cells[0][4] = init_piece(0, 4, 2, 9812); // Rey Blanco
   board->cells[0][3] = init_piece(0, 3, 2, 9813); // Reina Blanca
   board->cells[0][0] = init_piece(0, 0, 2, 9814); // Torre Blanca
   board->cells[0][7] = init_piece(0, 7, 2, 9814); // Torre Blanca
   board->cells[0][2] = init_piece(0, 2, 2, 9815); // Alfil Blanco
   board->cells[0][5] = init_piece(0, 5, 2, 9815); // Alfil Blanco
   board->cells[0][1] = init_piece(0, 1, 2, 9816); // Caballo Blanco
   board->cells[0][6] = init_piece(0, 6, 2, 9816); // Caballo Blanco
   for(int i = 0; i < 8; i++){
     board->cells[1][i] = init_piece(1, i, 2, 9817); // Peon Blanco
   }

   board->cells[7][3] = init_piece(7, 3, 1, 9818); // Rey Negro
   board->cells[7][4] = init_piece(7, 4, 1, 9819); // Reina Negra
   board->cells[7][0] = init_piece(7, 0, 1, 9820); // Torre Negra
   board->cells[7][7] = init_piece(7, 7, 1, 9820); // Torre Negra
   board->cells[7][2] = init_piece(7, 2, 1, 9821); // Alfil Negro
   board->cells[7][5] = init_piece(7, 5, 1, 9821); // Alfil Negro
   board->cells[7][1] = init_piece(7, 1, 1, 9822); // Caballo Negro
   board->cells[7][6] = init_piece(7, 6, 1, 9822); // Caballo Negro
   for(int i = 0; i < 8; i++){
     board->cells[6][i] = init_piece(6, i, 1, 9823); // Peon Negro
   }
 }

void print_board(Board* board){
  setlocale(LC_ALL, "");  // Si esto no esta, no imprime
  printf("\n\n");
  printf("         a   b   c   d   e   f   g   h\n");
  printf("        --- --- --- --- --- --- --- ---\n");
  for(int i = 0; i < 8; i++){
    printf("     %d |", i + 1);
    for(int j = 0; j < 8; j++){
      if (board->cells[i][j]->alive) printf(" %lc |", board->cells[i][j]->piece);
      else printf("   |");
    }
    printf("\n        --- --- --- --- --- --- --- ---\n");
  }
  printf("\n\n\n");
}

void make_move(Board* board, char piece_c[0], char piece_r[0], char new_c[0], char new_r[0]) {
  int row = atoi(new_r) - 1;
  int p_row = atoi(piece_r) - 1;
  int column = get_column(new_c);
  int p_column = get_column(piece_c);
  Piece* piece = board->cells[p_row][p_column];
  if (valid_move(board, piece, column, row)) {
    board->cells[row][column] = piece;
    board->cells[p_row][p_column] = null_piece();
  }
  else printf("Error: Invalid move. try another\n");
}

int get_column(char c[0]) {
  if (strcmp(c, "a") == 0) return 0;
  else if (strcmp(c, "b") == 0) return 1;
  else if (strcmp(c, "c") == 0) return 2;
  else if (strcmp(c, "d") == 0) return 3;
  else if (strcmp(c, "e") == 0) return 4;
  else if (strcmp(c, "f") == 0) return 5;
  else if (strcmp(c, "g") == 0) return 6;
  else if (strcmp(c, "h") == 0) return 7;
  return -1;
}

bool valid_move(Board* board, Piece* piece, int c, int r) {
  // Si la pieza existe en el juego
  if (!piece->alive) return false;
  // Jugada que sale del tablero
  if (!(in_board(r, c))) return false;
  // Jugada que no mueve a la pieza
  if (piece->r == r && piece->c == c) return false;
  // indica si el espacio selecciona para moverse esta ocupado por otra pieza
  bool oc = 0;
  if (board->cells[r][c]) {
    // Si el jugador esta moviendo una pieza a un espacio ya ocupado por él y no el rival
    if (board->cells[r][c]->player == piece->player) return false;
    oc = 1;
  }
  // PEON
  if (piece->piece == 9817 || piece->piece == 9823) return valid_pawn_move(piece, c, r, oc);
  // TORRE
  else if (piece->piece == 9814 || piece->piece == 9820) return valid_tower_move(board, piece, c, r, oc);
  // CABALLO
  else if (piece->piece == 9816 || piece->piece == 9822) return valid_horse_move(piece, c, r, oc);
  // ALFIL
  else if (piece->piece == 9815 || piece->piece == 9821) return valid_bishop_move(board, piece, c, r, oc);
  // REINA
  else if (piece->piece == 9813 || piece->piece == 9819) return valid_queen_move(piece, c, r, oc);
  // REY
  else return valid_king_move(board, piece, c, r, oc);
  // Cualquier otro caso no considerado
  return false;
}

bool in_board(int r, int c) {
  if ((r >= 0 || r <= 7) && (c >= 0 || c <= 7)) return true;
  return false;
}

bool valid_pawn_move(Piece* piece, int c, int r, bool oc) {
  if (piece->player == 1) {
    if (oc) {

    }
    else {
      if (piece->r == 1) {if (r == 2 || r == 3) return true;}
      else {if (r - piece->r == 1) return true;}
    }
  }
  else {
    if (oc) {

    }
    else {
      if (piece->r == 6) {if (r == 5 || r == 4) return true;}
      else  {if (piece->r - r == 1) return true;}
    }
  }
  return false;
}

bool valid_tower_move(Board* board, Piece* piece, int c, int r, bool oc) {
  int new_r = abs(r - piece->r);
  int new_c = abs(c - piece->c);
  if (new_c == 0) {
    if (r > piece->r) {
      for (int i = piece->r; i < (r+1); i++) {
        if (board->cells[i][c]->alive) return false;}
    }
    else {
      for (int i = r; i < (piece->r+1); i++) {
        if (board->cells[i][c]->alive) return false;}
    }
  }
  if (new_r == 0) {
    if (c > piece->c) {
      for (int i = piece->c; i < (c+1); i++) {
        if (board->cells[r][i]->alive) return false;}
    }
    else {
      for (int i = c; i < (piece->c+1); i++) {
        if (board->cells[r][i]->alive) return false;}
    }
  }
  if (new_r == 0 || new_c == 0) return true;
  return false;
}

bool valid_horse_move(Piece* piece, int c, int r, bool oc) {
  int new_r = abs(r - piece->r);
  int new_c = abs(c - piece->c);
  if (new_r + new_c == 3) return true;
  return false;
}

bool valid_bishop_move(Board* board, Piece* piece, int c, int r, bool oc) {
  int new_r = abs(r - piece->r);
  int new_c = abs(c - piece->c);
  if (r > piece->r && c > piece->c) {
    for (int i = 1; piece->c + i < 8 && piece->r + i < 8; i++) {if (board->cells[piece->r + i][piece->c + i]->alive) return false;}
  }
  else if (r > piece->r && c < piece->c) {
    for (int i = 1; piece->c - i < 0 && piece->r + i < 8; i++) {if (board->cells[piece->r + i][piece->c - i]->alive) return false;}
  }
  else if (r < piece->r && c > piece->c) {
    for (int i = 1; piece->c + i < 8 && piece->r - i < 8; i++) {if (board->cells[piece->r - i][piece->c + i]->alive) return false;}
  }
  else if (r < piece->r && c < piece->c) {
    for (int i = 1; piece->c - i < 8 && piece->r - i < 8; i++) {if (board->cells[piece->r - i][piece->c - i]->alive) return false;}
  }
  if (new_r == new_c) return true;
  return false;
}

bool valid_queen_move(Piece* piece, int c, int r, bool oc) {
  int new_r = abs(r - piece->r);
  int new_c = abs(c - piece->c);
  if (new_r < 2 && new_c < 2) return true;
  return false;
}

bool valid_king_move(Board* board, Piece* piece, int c, int r, bool oc) {
  if (valid_bishop_move(board, piece, c, r, oc)) return true;
  else if(valid_tower_move(board, piece, c, r, oc)) return true;
  return false;
}

void destroy_board(Board* board){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++) {
      free(board->cells[i][j]);
    }
  }
  for(int i = 0; i < 8; i++){
    free(board->cells[i]);
  }
  free(board->cells);
  free(board);
}


// Para probar el ajedrez
/*
int main() {

  Board* board = init_board();
  print_board(board);
  make_move(board, "b", "1", "c", "3");
  print_board(board);
  make_move(board, "g", "1", "f", "3");
  print_board(board);
  make_move(board, "h", "8", "h", "5");
  print_board(board);

  destroy_board(board);

  return 0;
}

*/
