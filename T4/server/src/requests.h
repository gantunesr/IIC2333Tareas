#include "queue.h"


void *heartbeat(void* socket);

void *timer(void* is_running);

void new_user(int sock, Queue* waiting, char* reply);

void PlayersList(int sock, Queue* waiting);

int match_request(int sock, Queue* waiting, char* client_message);

void accept_match(int sock, char* client_message, int opponent);

void PacketSupport(int sock);

void user_disconnect(int sock, Queue* waiting);
