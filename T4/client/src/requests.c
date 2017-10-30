#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "requests.h"


void heartbeat(int sock, char* reply){
  time_t now = time(0);
  time(&now);
  printf("Llego un heartbeat\n");
  unsigned char buffer[4];
  buffer[0] = reply[0];
  buffer[1] = 2;
  buffer[2] = reply[1];
  buffer[3] = (int)now;
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
