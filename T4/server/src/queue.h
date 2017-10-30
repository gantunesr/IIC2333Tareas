typedef struct Queue Queue;


struct Queue{
  int size;
  int max;
  int* sockets;
};

Queue* init_queue(int max);

void insert_queue(Queue* queue, int socket);

bool inqueue(Queue* queue, int socket);

void remove_queue(Queue* queue, int socket);

int pop_first(Queue* queue);

void destroy_queue(Queue* queue);
