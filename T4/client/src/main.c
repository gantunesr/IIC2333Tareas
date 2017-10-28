#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<pthread.h>
#include"board.h"
#include"requests.h"


int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
        printf("Could not create socket");
    }

    Board* board = init_board();
    print_board(board);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }

    pthread_t sniffer_thread;
    int *new_sock = malloc(1);
    *new_sock = sock;

    if( pthread_create( &sniffer_thread , NULL ,  listener , (void*) new_sock) < 0){
        perror("could not create thread");
        free(new_sock);
        return 1;
    }

    char nickname[1024];
    char message[1024] = {0};
    printf("Choose your nickname: \n");
    scanf("%s", nickname);
    message[0] = 2;
    message[1] = strlen(nickname);
    for(int i = 0; i < strlen(nickname); i++){message[2 + i] = nickname[i];}
    if(send(sock , message , strlen(message) , 0) < 0){return 1;}

    //keep communicating with server
    while(1){

        printf("Select option: \n");
        scanf("%s" , message);

        //Send some data
        if(send(sock , message , strlen(message) , 0) < 0){
            return 1;
        }
    }

    close(sock);
    return 0;
}
