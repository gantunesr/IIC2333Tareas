#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include"requests.h"


void *timer(void* is_running){
  bool finished = (bool*)is_running;
  sleep(10);
  finished = 1;
  return 0;}

void PacketSupport(int socket){
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

void heartbeat(int socket, int time, char* message){
    unsigned char heartbeat_ans[3];
    heartbeat_ans[0] = 1;
    heartbeat_ans[1] = 1;
    heartbeat_ans[2] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random() % 26];
    write(socket , &heartbeat_ans , strlen(&heartbeat_ans));
    sleep(10);
}
