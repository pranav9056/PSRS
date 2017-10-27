#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"
#include "merge.h"
#include "Queue.h"
#include "heap.h"


void fromQueueToHeap(queue* qu,int queueNo,minHeap mHeap,int noData){
	if(!isEmpty(qu[queueNo])){
		int t =getData(queueNo,qu[queueNo],noData);
		if(t==0)
			return;
	}
	int no = peek(qu[queueNo]);
	pop(qu[queueNo]);
	insert(mHeap,no,queueNo);
}

int getData(int processorNo,queue tempQ,int noData){
	int i , ierr, rank , size , dest , source , from , to , count , tag ;
	int stat_count , stat_source , stat_tag ;
	int data[noData] ;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank );
	
	
	dest = processorNo;
	int *val = malloc(sizeof(int));
	*val = 1;
	ierr = MPI_Send( val , 1 ,MPI_REAL, dest , rank ,MPI_COMM_WORLD) ;
	count = noData;
	tag = MPI_ANY_TAG; /* wildcard */

	from = processorNo; /* another wildcard */
	//printf("asking for data at:%d \n",dest);
	int *check = malloc(sizeof(int));
	ierr= MPI_Recv( check , 1 ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status );
	if(*check==0){
		return 0;
	}
	ierr= MPI_Recv( data , *check ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
	//printf("got data from:%d\n",from);
	ierr= MPI_Get_count (&status ,MPI_REAL,&stat_count ) ;
	stat_source = status.MPI_SOURCE;
	stat_tag = status.MPI_TAG;
	//printf( "Status of receive : dest=%d , source=%d , tag=%d ,count=%d \n " , rank , stat_source , stat_tag , stat_count );

	/*	if(stat_count==1){
		//printf("aborting\n");
		return 0;
	}*/
	
	int t;
	for (t=0;t<*check;t++){
		push(tempQ,data[t]);
		//printf("%d \n",data[t]);
	}
	//fromQueueToHeap(q,i,mHeap);
	//insert(mHeap,peek(q[i]),i);
	//printf("head:%d\n",peek(tempQ));
	return 1;

	
}

int main ( int argc , char **argv )
{
	struct timeval start,end;
	gettimeofday(&start,NULL);
	int i , ierr, rank , size , dest , source , from , to , count , tag ;
	int stat_count , stat_source , stat_tag ;
	MPI_Status status ;
	MPI_Init (&argc ,& argv ) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank ) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size ) ;
	dest=size-1;
	//int k=atoi(argv[1]); //Total Number of Elements to be sorted
	//int noPerProcessor = k/(size-1);
	int noPerProcessor = atoi(argv[1]);
	int k = noPerProcessor*(size-1);
	
	if (rank!=dest){
		int i;
		int arr[noPerProcessor];
		srand(rank+2);
		for (i=0;i<noPerProcessor;i++){
			arr[i] = rand()%5000;
		}
		int arr_size = sizeof(arr)/sizeof(arr[0]);
		sort(arr,arr_size);
		//printf("Sorted array is \n");
		//printArray(arr,arr_size);
		int valtosend = (k/(size-1))/size;
		// Send initial values
		ierr = MPI_Send( arr , valtosend ,MPI_REAL, dest , rank ,MPI_COMM_WORLD) ;
		int add = valtosend;
		int a=0;
		//float last = ceil(noPerProcessor/(size+0.0))-valtosend; 
		int last = noPerProcessor-noPerProcessor%size;
		while(add<=noPerProcessor){
			from = dest; /* another wildcard */
			int *val = malloc(sizeof(int));
			tag = MPI_ANY_TAG; /* wildcard */
			ierr= MPI_Recv( val , 1 ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
			ierr= MPI_Get_count(&status ,MPI_REAL,&stat_count ) ;
			stat_source = status.MPI_SOURCE;
			stat_tag = status.MPI_TAG;
			//printf("here %d,%d,%d\n",a,rank,add);
			a++;
			//printf( "request Status of receive : dest=%d , source=%d , tag=%d ,count=%d ,value:%d\n " , rank , stat_source , stat_tag , stat_count,*val );
			if(*val==1 && add<noPerProcessor){
				if (add==last){
					valtosend = noPerProcessor-last;
					//printf("ggggggggggggg:%d\n",last);
				}
				//printf("sending data,%d\n",rank);
				*val = valtosend;
				ierr = MPI_Send( val , 1 ,MPI_REAL, dest , rank ,MPI_COMM_WORLD);
				ierr = MPI_Send( &arr[add] , valtosend ,MPI_REAL, dest , rank ,MPI_COMM_WORLD);
				add+=valtosend;
			}
			else{
				*val=0;
				//printf("end of data stream\n");
				ierr = MPI_Send( val , 1 ,MPI_REAL, dest , rank ,MPI_COMM_WORLD);
				add+=valtosend;
			}
		}
	}
	else{
		tag = MPI_ANY_TAG; /* wildcard */
		count = (k/(size-1))/size;
		int data[count] ;
		int i;
		struct queue** q = createQueues(size-1);
		minHeap mHeap = createMinHeap(size-1);
		for(i=0;i<size-1;i++){
			from = i; /* another wildcard */
			ierr= MPI_Recv( data , count ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
			ierr= MPI_Get_count (&status ,MPI_REAL,&stat_count ) ;
			stat_source = status.MPI_SOURCE;
			stat_tag = status.MPI_TAG;
			//printf( "Initial Status of receive : dest=%d , source=%d , tag=%d ,count=%d \n " , rank , stat_source , stat_tag , stat_count );
			int t;
			for (t=0;t<count;t++){
				push(q[i],data[t]);
				//printf("%d \n",data[t]);
			}
			fromQueueToHeap(q,i,mHeap,count);
			//insert(mHeap,peek(q[i]),i);
			//printf("head:%d\n",peek(q[i]));
			
		}
		//printf("answerrrrr:\n");
		FILE *f = fopen("file.txt", "w");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		int co = 0;
		while(mHeap->size!=0){
			int *min = extractMin(mHeap);
			fprintf(f,"%d\n",min[0]);
			//printf("\n%d:%d\n",min[0],min[1]);
			fromQueueToHeap(q,min[1],mHeap,count);
			//printf("size:%d\n",mHeap->size);
			co++;
		}
		fprintf(f,"%d\n",co);
		fclose(f);
		//printf("dine");



		
	}
		//printf("dine");
	gettimeofday(&end,NULL);
	printf("\n Time taken for execution on processor = %d is : %ld microseconds\n",rank,
				((end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec))); 
	ierr= MPI_Finalize();
	//printf("dine");

	return 0;
}
