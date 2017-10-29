#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "requests.h"
#include "queue.h"


void *connection_handler(void *);


int main(int argc , char *argv[]){
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Could not create socket\n");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error\n");
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
            perror("could not create thread\n");
            free(new_sock);
            return 1;
        }
    }

    if (client_sock < 0){
        perror("accept failed\n");
        return 1;
    }

    return 0;
}

/* This will handle connection for each client */
void *connection_handler(void *socket_desc){
    //Get the socket descriptor
    uint16_t sock = *(int*)socket_desc;
    printf("SOCK: %d\n", sock);
    int read_size = 0;
    char client_message[1024] = {0};

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        if(client_message[0] == 1){printf("Heartbeat\n");} // Heartbeat
        else if(client_message[0] == 2){ // New user
          printf("New user\n");
        }
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
        printf("Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("recv failed\n");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
