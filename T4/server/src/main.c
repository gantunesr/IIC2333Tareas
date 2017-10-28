#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>


void *connection_handler(void *);
void heartbeat(int socket, int time, char* message);
uint16_t ID;


int main(int argc , char *argv[]){
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    ID = 0;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0){
            perror("could not create thread");
            free(new_sock);
            return 1;
        }
    }

    if (client_sock < 0){
        perror("accept failed");
        return 1;
    }

    return 0;
}

/* This will handle connection for each client */
void *connection_handler(void *socket_desc){
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size = 0;
    char client_message[1024] = {0};
    int client_id;

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        if(client_message[0] == 1){printf("Heartbeat\n");} // Heartbeat
        else if(client_message[0] == 2){ // New user
          printf("New user\n");
          ID++;
          client_id = ID;}
        else if(client_message[0] == 4){ // Invitacion a jugar
          printf("Invitacion a jugar\n");
        }
        else if(client_message[0] == 6){ // Chat
          printf("chat\n");
        }
        else if(client_message[0] == 8){ // Move
          printf("Move\n");
        }
        else if(client_message[0] == 9){ // Disconnect
          printf("Disconnect\n");
        }
        else if(client_message[0] == 14){ // ServerInfo
          printf("ServerInfo\n");
        }
        else if(client_message[0] == 15){ // PacketSupport
          printf("PacketSupport\n");
        }
        else{printf("Invalid ID");}
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
    }

    if(read_size == 0){
        printf("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

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
    heartbeat_ans[2] = "a";
    write(socket , &heartbeat_ans , strlen(&heartbeat_ans));
    sleep(10);
}
