struct Queue{
  int size;
  int max;
  int* array;
};
typedef struct Queue Queue;

Queue* init_queue(int max);

void insert_queue(int number, Queue* queue);

int get_min_time(Queue* queue, int head);

int get_index(Queue* queue, int head);

int compare(const void *a, const void *b);

void destroy_queue(Queue* queue);


struct Disk{
  int head;
  int* read_array;
  int size;
  int actual;
  int seek_time;
  int change_side;
};
typedef struct Disk Disk;

Disk* init_disk(int head, int size_queue);

void destroy_disk(Disk* disk);


// Schedulers
void fcfs(Disk* disk, Queue* queue);

void sstf(Disk* disk, Queue* queue);

void fun_scan(Disk* disk, Queue* queue);

void clook(Disk* disk, Queue* queue);

void compare_cilinders(Disk* disk, int cilinder);
