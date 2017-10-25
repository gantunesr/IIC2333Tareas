#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>


void answer_heartbeat(int sock, char message, time_t now);


int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    char server_reply[2000];
    //time_t now = time(0);
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }

    //keep communicating with server
    while(1){
        char message[1024] = {0};
        printf("Enter message: ");
        scanf("%s" , message);

        //Send some data
        if(send(sock , message , strlen(message) , 0) < 0){
            return 1;
        }

        //Receive a reply from the server
        if(recv(sock , server_reply , 1024 , 0) < 0){
            puts("recv failed");
            break;
        }

        printf("Server reply: %s\n", server_reply);
    }

    close(sock);
    return 0;
}

void answer_heartbeat(int sock, char message, time_t now){
  time(&now);
  // responder message + now
}
