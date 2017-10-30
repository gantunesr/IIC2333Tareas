#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include"requests.h"


void *timer(void* is_running){
  bool finished = (bool*)is_running;
  sleep(10);
  finished = 1;
  return 0;}

void PacketSupport(int sock){
  unsigned char packets[10];
  packets[0] = (uint8_t)15;
  packets[1] = (uint8_t)8;
  packets[2] = (uint8_t)1;
  packets[3] = (uint8_t)2;
  packets[4] = (uint8_t)4;
  packets[5] = (uint8_t)6;
  packets[6] = (uint8_t)8;
  packets[7] = (uint8_t)9;
  packets[8] = (uint8_t)14;
  packets[9] = (uint8_t)15;
  write(socket , &packets, strlen(&packets));
}

void new_user(int sock, Queue* waiting, char* reply){
  insert_queue(waiting, sock);
  char user_id[3];
  user_id[0] = 2;
  user_id[1] = 2;
  user_id[2] = (uint16_t)sock;
}

void PlayersList(int sock, Queue* waiting){
  char players[1024];
  players[0] = (uint8_t)3;
  players[1] = (uint8_t)(waiting->size - 1);
  int counter = 0;
  for(int i = 0; i < waiting->size; i++){
    if(waiting->sockets[i] != sock){
      players[2 + counter] = (uint8_t)waiting->sockets[i];
      counter++;
    }
  }
  players[1] = (uint8_t)counter;
  write(sock , players , strlen(players));
}

void match_request(int sock, Queue* waiting, char* client_message){
  uint16_t opponent = atoi(&client_message[2]);
  printf("Opponent: %d\n", opponent);
  if(opponent != sock && inqueue(waiting, opponent)){
    printf("Encontró rival\n");
    return;
  }
  printf("No esta el rival\n");
}

void heartbeat(int socket, int time, char* message){
    unsigned char heartbeat_ans[3];
    heartbeat_ans[0] = 1;
    heartbeat_ans[1] = 1;
    heartbeat_ans[2] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random() % 26];
    write(socket , &heartbeat_ans , strlen(&heartbeat_ans));
    sleep(10);
}

void user_disconnect(int sock, Queue* waiting){
  remove_queue(waiting, sock);
  char answer[3];
  answer[0] = (uint8_t)9;
  answer[1] = (uint8_t)1;
  answer[2] = (uint8_t)0;
  write(sock , answer , strlen(answer));
}
