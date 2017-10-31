#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "requests.h"
#include "board.h"


Queue* waiting;
int opponents[1024];


void *connection_handler(void *);


int main(int argc , char *argv[]){
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    waiting = init_queue(1);

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Could not create socket\n");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

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
        else{insert_queue(waiting, *new_sock);}
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
    insert_queue(waiting, sock);

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        if(client_message[0] == 1){printf("Heartbeat\n");} // Heartbeat
        else if(client_message[0] == 2){ // New user
          new_user(sock, waiting, client_message);
        }
        else if(client_message[0] == 3){ // Jugadores Disponibles
          printf("Players List\n");
          PlayersList(sock, waiting);
        }
        else if(client_message[0] == 4){ // Invitacion a jugar
          printf("Invitacion a jugar\n");
          if(match_request(sock, waiting, client_message)){
            printf("Saving opponent %d\n", atoi(&client_message[2]));
            opponents[sock] = atoi(&client_message[2]);
            opponents[atoi(&client_message[2])] = sock;
          }
        }
        else if(client_message[0] == 5){ // Confirmacion a jugar
          printf("Confirmacion para jugar\n");
          accept_match(sock, client_message, opponents[sock]);

        }
        else if(client_message[0] == 6){ // Chat
          printf("chat\n");
        }
        else if(client_message[0] == 8){ // Move
          printf("Move\n");
        }
        else if(client_message[0] == 9){ // Disconnect
          user_disconnect(sock, waiting);
          printf("Client disconnected correctly\n");
          free(socket_desc);
          close(socket_desc);
          return 0;
        }
        else if(client_message[0] == 14){ // ServerInfo
          printf("ServerInfo\n");
        }
        else if(client_message[0] == 15){ // PacketSupport
          printf("PacketSupport\n");
        }
        else{printf("Invalid ID\n");}

        client_message[0] = 0;
        //Send the message back to client
        // write(sock , client_message , strlen(client_message));
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
    remove_queue(waiting, sock);

    return 0;
}
