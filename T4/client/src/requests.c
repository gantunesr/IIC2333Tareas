#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "requests.h"


int heartbeat(int sock, char* reply) {
  time_t now = time(0);
  time(&now);
  char buffer[7];
  buffer[0] = reply[0];
  buffer[1] = 5;
  buffer[2] = reply[2];
  buffer[3] = (int)now;
  if(send(sock , buffer , strlen(buffer) , 0) < 0){
    return 1;
  }
  return 0;
}

int get_players(int sock){
  printf("Get player list\n");
  char message[2];
  message[0] = (uint8_t)3;
  message[1] = (uint8_t)0;
  if(send(sock , message , strlen(message) , 0) < 0){
    return 1;
  }
  return 0;
}

int match_request(int sock, char* opponent){
  printf("Opponents ID's: \n");
  scanf("%s", opponent);
  char answer[3];
  answer[0] = 4;
  answer[1] = 2;
  answer[2] = *opponent;
  if(send(sock , answer , strlen(answer) , 0) < 0){
    return 1;
  }
  return 0;
}

int disconnect_server(int sock){
  char message[2];
  message[0] = (uint8_t)9;
  message[1] = (uint8_t)0;
  if(send(sock , message , strlen(message) , 0) < 0){
      return 1;
  }
  return 0;
}

// Listener methods
void print_players(char* players){
  for(int i = 0; i < players[1]; i++){
    printf("%d\n", players[2 + i]);
  }
}

int invite_to_play(int sock, char* message){
  printf("%d wants to play a match with you (0 no, other yes)\n", message[2]);
  char answer = 0;
  scanf("%s", &answer);
  char respond[3];
  respond[0] = 5;
  respond[1] = 1;
  respond[2] = (bool)answer;
  if(send(sock, respond, strlen(respond), 0) < 0 || !answer){return 0;}
  return 1;
}

void send_play(int sock, uint8_t oc, uint8_t or, uint8_t nc, uint8_t nr) {
  /*
  oc: original piece column in the board
  or: original piece row in the board
  nc: next piece column in the board
  nr: next piece row in the board
  */
  char* buffer = malloc(4*sizeof(char));
  buffer[0] = oc;
  buffer[1] = or;
  buffer[2] = nc;
  buffer[3] = nr;
  for (int i = 0; i < 4; i++){
    if (send(sock, &buffer[i], 4, 0) < 0) puts("Error: New move message failed");
  }
}
