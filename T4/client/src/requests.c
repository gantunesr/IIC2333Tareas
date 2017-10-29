#include"requests.h"
#include<time.h>
#include<stdio.h>
#include<sys/socket.h>


void *listener(void *socket){
  int sock = *(int*)socket;
  char server_reply[2000];

  //Receive a reply from the server
  while(1){
    if(recv(sock , server_reply , 1024 , 0) < 0){
        puts("recv failed\n");
        break;
    }
    if(server_reply[0] == 1){
      heartbeat(sock, &server_reply);
    }
  }
  return 0;
}


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
