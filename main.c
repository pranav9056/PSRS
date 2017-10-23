#include <stdio.h>
#include "mpi.h"
#include "merge.h"
#include "Queue.h"
#include "heap.h"

typedef struct node* node;
typedef struct queue* queue;

typedef struct minHeapNode* minHeapNode;
typedef struct minHeap* minHeap;


int main ( int argc , char **argv )
{
	int i , ierr, rank , size , dest , source , from , to , count , tag ;
	int stat_count , stat_source , stat_tag ;
	int data[100] ;
	MPI_Status status ;
	MPI_Init (&argc ,& argv ) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank ) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size ) ;
	dest=size-1;
	int arr[25];
	int k=100;
	if (rank!=dest){
		int i;
		srand(rank+2);
		for (i=0;i<25;i++){
			arr[i] = rand()%150;
		}
		int arr_size = sizeof(arr)/sizeof(arr[0]);
		sort(arr,arr_size);
		printf("Sorted array is \n");
		printArray(arr,arr_size);
		int valtosend = (k/(size-1))/size; 
		ierr = MPI_Send( arr , valtosend ,MPI_REAL, dest , rank ,MPI_COMM_WORLD) ;
	}
	else{
		tag = MPI_ANY_TAG; /* wildcard */
		count = (k/(size-1))/size;
		int i;
		struct queue** q = createQueues(size-1);
		for(i=0;i<size-1;i++){
			from = i; /* another wildcard */
			ierr= MPI_Recv( data , count ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
			ierr= MPI_Get_count (&status ,MPI_REAL,&stat_count ) ;
			stat_source = status.MPI_SOURCE;
			stat_tag = status.MPI_TAG;
			printf( "Status of receive : dest=%d , source=%d , tag=%d ,count=%d \n " , rank , stat_source , stat_tag , stat_count );
			int t;
			for (t=0;t<5;t++){
				push(q[i],data[t]);
				printf("%d \n",data[t]);
			}
			printf("head:%d\n",peek(q[i]));
			
		}


		
	}
	ierr= MPI_Finalize ( ) ;
	return 0;
}
