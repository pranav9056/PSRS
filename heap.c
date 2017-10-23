#include <stdio.h>
#include<stdlib.h>
#include "heap.h"
minHeap createMinHeap(int size){
	minHeap temp = malloc(sizeof(struct minHeap));
	temp->heap = malloc(sizeof(struct minHeapNode)*(size+1)) ;
	temp->size = 0;
	return temp;
}
void insert(minHeap heap,int data,int queueNo){
	heap->heap[heap->size+1].data = data;
	heap->heap[heap->size+1].queueNo = queueNo;
	heap->size++;
	int temp = heap->size;
	while(heap->heap[temp].data < heap->heap[temp/2].data && temp/2>=1){
		int t1= heap->heap[temp].data;
		int t2= heap->heap[temp].queueNo;
		heap->heap[temp].data=heap->heap[temp/2].data;
		heap->heap[temp].queueNo=heap->heap[temp/2].queueNo;
		heap->heap[temp/2].data=t1;
		heap->heap[temp/2].queueNo=t2;
		temp = temp/2;
	}
}
int* extractMin(minHeap heap){
	int* min = malloc(sizeof(int)*2);
	min[0] = heap->heap[1].data;
	min[1] = heap->heap[1].queueNo;
	heap->heap[1].data = heap->heap[heap->size].data;
	heap->heap[1].queueNo = heap->heap[heap->size].queueNo;
	heap->size--;
	int temp = heap->size;
	int val=1;
	while(2*val <= 	temp){
		int j;
		if(2*val==temp || heap->heap[2*val].data <= heap->heap[2*val+1].data)
			j=2*val;
		else
			j=2*val+1;
		if(heap->heap[j].data < heap->heap[val].data){
			int t1= heap->heap[j].data;
			int t2= heap->heap[j].queueNo;
			heap->heap[j].data=heap->heap[val].data;
			heap->heap[j].queueNo=heap->heap[val].queueNo;
			heap->heap[val].data=t1;
			heap->heap[val].queueNo=t2;
			val=j;
		}
		else
			break;
			
	}
	return min;
	

}
/*
void main(){
	minHeap minh = createMinHeap(40);
	insert(minh,3,30);
	insert(minh,2,40);
	insert(minh,1,200);
	insert(minh,-10,200);
	insert(minh,134,200);
	insert(minh,15,200);
	insert(minh,112,200);
	insert(minh,13,200);

	printf("%d:%d:%d:%d:%d",minh->heap[1].data,minh->heap[2].data,minh->heap[1].queueNo,minh->heap[2].queueNo,minh->size);
	int *min = extractMin(minh);
	printf("\n%d",min[0]);
	printf("\n%d:%d:%d:%d:%d",minh->heap[1].data,minh->heap[2].data,minh->heap[1].queueNo,minh->heap[2].queueNo,minh->size);

}
*/