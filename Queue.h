#ifndef HEADER_H_
#define HEADER_H_  
typedef struct node* node;
typedef struct queue* queue;

struct node{
	int value;
	node next;
};

struct queue{
	node front,rear;
};

struct queue** createQueues(int size);
struct queue* createQueue();
struct node* createNode(int val);
int peek(queue q);
void push(queue q,int val);
void pop(queue q);
#endif

