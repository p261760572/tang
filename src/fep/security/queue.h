typedef struct node {
  struct node *next;
  int flag;
  int sock_id;
  char ip[30];
  int port;
  int num;
} node;

typedef struct queue {
  node *head, *tail; 
  int num;
} queue;

void queue_init(queue *myroot);
void queue_put(queue *myroot, node *mynode);
node *queue_get(queue *myroot);

