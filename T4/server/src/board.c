#include<stdlib.h>
#include<stdio.h>
#include <locale.h>
#include"board.h"


Board* init_board(){
   Board* board = malloc(sizeof(Board));
   init_pieces(board);
   return board;
 }

void init_pieces(Board* board){

   board->cells[0][4] = 9812; // Rey Blanco
   board->cells[0][3] = 9813; // Reina Blanca
   board->cells[0][0] = 9814; // Torre Blanca
   board->cells[0][7] = 9814; // Torre Blanca
   board->cells[0][2] = 9815; // Alfil Blanco
   board->cells[0][5] = 9815; // Alfil Blanco
   board->cells[0][1] = 9816; // Caballo Blanco
   board->cells[0][6] = 9816; // Caballo Blanco
   for(int i = 0; i < 8; i++){
     board->cells[1][i] = 9817; // Peon Blanco
   }

   board->cells[7][3] = 9818; // Rey Negro
   board->cells[7][4] = 9819; // Reina Negra
   board->cells[7][0] = 9820; // Torre Negra
   board->cells[7][7] = 9820; // Torre Negra
   board->cells[7][2] = 9821; // Alfil Negro
   board->cells[7][5] = 9821; // Alfil Negro
   board->cells[7][1] = 9822; // Caballo Negro
   board->cells[7][6] = 9822; // Caballo Negro
   for(int i = 0; i < 8; i++){
     board->cells[6][i] = 9823; // Peon Negro
   }
 }

void print_board(Board* board){
  setlocale(LC_ALL, "");  // Si esto no esta, no imprime
  printf(" a  b  c  d  e  f  g  h\n");
  for(int i = 0; i < 8; i++){
    printf("%d", i + 1);
    for(int j = 0; j < 8; j++){
      printf("%lc  ", board->cells[i][j]);
    }
    printf("\n");
  }
}

void free_board(Board* board){
  for(int i = 0; i < 8; i++){
    free(board->cells[i]);
  }
  free(board);
}
