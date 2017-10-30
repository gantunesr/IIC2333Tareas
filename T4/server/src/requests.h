#include "queue.h"


void heartbeat(int socket, int time, char* message);

void *timer(void* is_running);

void new_user(int sock, Queue* waiting, char* reply);

void PlayersList(int sock, Queue* waiting);

void match_request(int sock, Queue* waiting, char* client_message);

void PacketSupport(int sock);

void user_disconnect(int sock, Queue* waiting);
