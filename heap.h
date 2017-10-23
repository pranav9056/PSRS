#ifndef HEADER_H_
#define HEADER_H_
  
typedef struct minHeapNode* minHeapNode;
typedef struct minHeap* minHeap;
struct minHeapNode{
	int data;
	int queueNo;
};
struct minHeap{
	int size;
	minHeapNode heap;
};
minHeap createMinHeap(int size);
void insert(minHeap heap,int data,int queueNo);
int* extractMin(minHeap heap);
#endif
