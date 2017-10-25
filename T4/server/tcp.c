#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>


struct Message {
  int lenght;
  int id;
  int* info;
};typedef struct Message Message;

Message* receive_message (int socket){
  Message*  message = malloc(sizeof(Message));

  unsigned char lenght[4];
  unsigned char id[4];
  unsigned char receiver[4];

  if (recv(socket , lenght , 4 , 0) < 0){
    puts("Opponent disconnected, you won!");
    return 0;
  };
  message -> info = malloc((*lenght-2)*sizeof(int));
  if (recv(socket , id , 4 , 0) < 0){
    puts("Opponent disconnected, you won!");
    return 0;
  };
  message -> lenght = *lenght;
  message -> id = *id;

  for (int i = 0; i < *lenght-2; i++){
    if( recv(socket , receiver , 4 , 0) < 0) {
      puts("Opponent disconnected, you won!");
      return 0;
    }
    message -> info[i] = *receiver;
  }

  return message;
}

int connect_client(char* ip, int port){

  int sock;
  struct sockaddr_in server;


  //Create socket
  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1) {
    printf("Could not create socket");
  }

  server.sin_addr.s_addr = inet_addr(ip); //ip
  server.sin_family = AF_INET;
  server.sin_port = htons( port );

  //Connect to remote server
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("connect failed. Error");
    exit(1);
  }

  return sock;

}

int connect_server(char* ip, int port){
  int socket_desc , client_sock , c ;
  struct sockaddr_in server , client;

  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1)
  {
      printf("Could not create socket");
  }
  puts("Socket created");

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  if (strcmp(ip, "0.0.0.0") ==  0){
    server.sin_addr.s_addr = htonl(INADDR_ANY); //ip, aca se genera automatico
  }
  else {
    server.sin_addr.s_addr = inet_addr(ip);
    //"127.0.0.1"
  }
  server.sin_port = htons( port );

  //Bind
  if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
  {
      //print the error message
      perror("bind failed. Error");
      exit(1);
  }
  puts("bind done");

  //Listen
  listen(socket_desc , -1);

  //Accept and incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
  //accept connection from an incoming client

  //client_sock = connect(socket_desc, (struct sockaddr *)&client, (socklen_t)c);
  if (client_sock < 0)
  {
      perror("accept failed");
      exit(1);
  }
  puts("Connection accepted");
  return client_sock;
}

unsigned char* random_to_buffer (int random) {
  unsigned char *buffer = malloc(6*sizeof(char));
  buffer[0] = 6;
  buffer[1] = 11;
  buffer[2] = random;
  buffer[3] = (random/(256));
  buffer[4] = ((random)/(65536));
  buffer[5] = ((random)/(16777216));
  return buffer;
}


int receive_turn_message (int sock){
  unsigned char number_reply[4];
  unsigned char lenght[4];
  unsigned char id[4];
  if( recv(sock , lenght , 4 , 0) < 0) {
    puts("Server was used");
    exit(0);
  }
  if( recv(sock , id , 4 , 0) < 0) {
    puts("Server was used");
    exit(0);
  }
  int number = 0;
  for (double i = 0; i < *lenght-2; i++){
    if( recv(sock , number_reply , 4 , 0) < 0) {
      puts("Server was used");
      exit(0);
    }
    number += (*number_reply) * pow(2.0, i*8);
  }
  return number;
}

void new_move_message (int sock, int x, int y) {
  char* buffer = malloc(4*sizeof(char));
  buffer[0] = 4;
  buffer[1] = 12;
  buffer[2] = x;
  buffer[3] = y;
  for (int i = 0; i < 4; i++){
    if (send(sock , &buffer[i] , 4, 0)<0){
      puts("new move message failed");
    };
  }
}

void destroyed_message (int sock, int remaining) {
  char* buffer = malloc(4*sizeof(char));
  buffer[0] = 3;
  buffer[1] = 13;
  buffer[2] = remaining;
  for (int i = 0; i < 3; i++){
    if (send(sock , &buffer[i] , 4, 0)<0){
      puts("new move message failed");
    };
  }
}

int assign_turn(int socket, int server){
  int random = rand();
  unsigned char* buffer = random_to_buffer(random);

  if (server == 1) {
    for (int i = 0; i < 6; i++){
      if (send(socket , &buffer[i] , 4, 0)<0){
        puts("turn message failed");
      };
    }
  }
  else if (server == 0){
    for (int i = 0; i < 6; i++){
      if (write(socket , &buffer[i] , 4)<0){
        puts("turn message failed");
      };
    }
  }
  int number = receive_turn_message(socket);

  printf("My number was %d, my opponent's %d\n", random, number);

  int turn = 0;
  if (number <= random) {
    printf("My turn \n");
    turn = 2;
  }
  else {
    printf("Opponent's turn \n");
    turn = 1;
  }
  return turn;
}
