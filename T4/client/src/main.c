#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<pthread.h>
#include<stdbool.h>
#include"board.h"
#include"requests.h"


void *listener(void *);
pthread_mutex_t lock;
bool color, waiting;
Board* board;


int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    pthread_mutex_init(&lock, NULL);

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
        printf("Could not create socket");
    }

    board = init_board();

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

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
    bool waiting = 1;
    char opponent = 0;
    color = 0;

    //keep communicating with server
    while(1){
      pthread_mutex_lock(&lock);
      memset(message, 0, 1024 * sizeof(char));
      if(waiting){
        printf("Select option: \n");
        printf("\t1: Players List\n");
        printf("\t2: Match Request\n");
        printf("\t3: Disconnect\n");
        scanf("%c" , &message[0]);

        pthread_mutex_unlock(&lock);
        usleep(200);
        if(atoi(&message[0]) == 1){
          get_players(sock);
        }
        else if(atoi(&message[0]) == 2){
          printf("Match request X\n");
          match_request(sock, &opponent);
        }
        else if(atoi(&message[0]) == 3){
          destroy_board(board);
          // FALTA MATAR EL OTRO THREAD
          if( disconnect_server(sock)){
            printf("Hard disconnect\n");
            close((int)sock);
            return 1;
          }
          close(sock);
          return 0;
        }
        else{printf("Not valid\n");}
      }
    }

    close(sock);
    return 0;
}


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
      if(heartbeat(sock, &server_reply)){
        printf("Error al enviar el Heartbeat\n");
      }
    }
    else if(server_reply[0] == 3){
      printf("Player List\n");
      print_players(server_reply);
    }
    else if(server_reply[0] == 4){
      printf("Recieve answer for invitation %d\n", atoi(&server_reply[2]));
      if(atoi(&server_reply[2])){
        color = 0;
        waiting = 0;
        print_board(board);
      }
    }
    else if(server_reply[0] == 5){
      printf("Invitacion a jugar\n");
      pthread_mutex_lock(&lock);
      if(invite_to_play(sock, server_reply)){
        printf("Se envio un sí como respuesta\n");
        color = 1;
        waiting = 0;
        print_board(board);
      }
      pthread_mutex_unlock(&lock);
    }
  }
  return 0;
}
