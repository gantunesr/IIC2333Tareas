#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include"requests.h"


void *heartbeat(void* socket){
  uint16_t sock = *(int*)socket;
  while(1){
    char heartbeat_ans[3];
    heartbeat_ans[0] = 1;
    heartbeat_ans[1] = 1;
    heartbeat_ans[2] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random() % 26];
    if(write(sock , heartbeat_ans , strlen(heartbeat_ans)) < 0 ){
      return 0;
    }
    sleep(10);
  }
}

void PacketSupport(int sock){
  char packets[10];
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
  write(sock , packets, strlen(packets));
}

void new_user(int sock, Queue* waiting, char* reply){
  insert_queue(waiting, sock);
  char user_id[3];
  user_id[0] = 2;
  user_id[1] = 2;
  user_id[2] = (uint16_t)sock;
  write(sock, user_id, strlen(user_id));
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

int match_request(int sock, Queue* waiting, char* client_message){
  uint16_t opponent = atoi(&client_message[2]);
  if((opponent != sock) && inqueue(waiting, opponent)){
    char message[3];
    message[0] = 5;
    message[1] = 2;
    message[2] = sock;
    write(opponent, message, strlen(message));
    printf("Invitacion Enviada a %d\n", opponent);
    return 1;
  }
  return 0;
}

void accept_match(int sock, char* client_message, int opponent){
  client_message[0] = 4;
  write(opponent, client_message, strlen(client_message));
  printf("Message send to %d\n", opponent);
}

void user_disconnect(int sock, Queue* waiting){
  remove_queue(waiting, sock);
  char answer[3];
  answer[0] = (uint8_t)9;
  answer[1] = (uint8_t)1;
  answer[2] = (uint8_t)0;
  write(sock , answer , strlen(answer));
}
