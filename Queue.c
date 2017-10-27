#include <stdio.h>
#include<stdlib.h>
#include "Queue.h"

queue createQueue(){
	queue temp = malloc(sizeof(struct queue));
	temp->front=NULL;
	temp->rear=NULL;
	return temp;
}

queue* createQueues(int size){
	queue* temp = malloc(sizeof(queue)*size);
	//queue temp[size];
	int i;
	for (i=0;i<size;i++){
		temp[i] = createQueue();
		//temp[i].front=NULL;
		//temp[i].rear=NULL;

	}
	return temp;
}

node createNode(int val){
	node temp = (node)malloc(sizeof(struct node));
	temp->value = val;
	temp->next = NULL;
	return temp;
}

int peek(queue q){
	return q->front->value;
}

void push(queue q,int val){
	node temp = createNode(val);
	if(q->front ==NULL && q->rear==NULL){
		q->front = temp;
		q->rear = temp;
	}
	else{
		q->rear->next = temp;
		q->rear = temp;
	}	
}

void pop(queue q){
	if(q->front == NULL && q->rear==NULL){
		printf("Queue empty\n");
	}
	else{
		node temp = q->front;
		q->front = q->front->next;
		if(q->front==NULL)
			q->rear=NULL;
		free(temp);
	}
}

int isEmpty(queue q){
	if(q->front == NULL && q->rear==NULL){
		return 0;
	}
	return 1;
}
/*
int main(){
	queue q = createQueue();
	push(q,2);
	push(q,3);
	push(q,4);
	push(q,5);
	printf("%d\n",peek(q));
	pop(q);
	pop(q);
	printf("%d\n",peek(q));
	pop(q);
	pop(q);
	pop(q);
	pop(q);
	pop(q);
	
}
*/