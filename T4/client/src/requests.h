void heartbeat(int sock, char* reply);

int get_players(int sock);

int match_request(int sock, char* opponent);

int disconnect_server(int sock);


// Listener method
void print_players(char* players);

int invite_to_play(int sock, char* message);
