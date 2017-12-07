#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"
#include "merge.h"
#include "Queue.h"
#include "heap.h"


void fromQueueToHeap(queue* qu,int queueNo,minHeap mHeap){
	if(!isEmpty(qu[queueNo])){
		return;
	}
	int no = peek(qu[queueNo]);
	pop(qu[queueNo]);
	insert(mHeap,no,queueNo);
}

int binarySearch(int arr[], int l, int r, int x)
{
  int m;
	while( r - l > 1 )
	{
		m = l + (r - l)/2;
		if( arr[m] <= x )
			l = m;
		else
			r = m;
	}
	return l;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main ( int argc , char **argv )
{
	struct timeval start,end;
	int i , ierr, rank , size , dest , source , from , to , count , tag ;
	int stat_count , stat_source , stat_tag ;
	MPI_Status status ;
	MPI_Init (&argc ,& argv ) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank ) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size ) ;
	dest=size-1;
	//int k=atoi(argv[1]); //Total Number of Elements to be sorted
	//int noPerProcessor = k/(size-1);
	long noPerProcessor = atoi(argv[1]);
	long k = noPerProcessor*(size);
	
	//removed if
	int *arr= malloc(sizeof(int)*noPerProcessor);
	srand(rank*7+10);
	for (i=0;i<noPerProcessor;i++){
		arr[i] = rand()%50000;
	}
	gettimeofday(&start,NULL);
	qsort(arr, noPerProcessor, sizeof(int), cmpfunc);
	//sort(arr,noPerProcessor);
	//printf("Sorted array is \n");
	//printArray(arr,noPerProcessor);
	// Send initial values
	int temp[size];
	int pivots[size-1];
	struct queue** q = createQueues(size);
	minHeap mHeap = createMinHeap(size);

	if(rank!=dest){
		for (i=0;i<size;i++){
			if(i==0)
				temp[i] = arr[i];
			else
				temp[i] = arr[((i)*(k/(size*size)))];
		}
		
		ierr = MPI_Send( temp , size ,MPI_REAL, dest , rank ,MPI_COMM_WORLD);
	}
	else{
		tag = MPI_ANY_TAG; /* wildcard */
		count = size;
		//int data[count] ;
		for(i=0;i<size-1;i++){
			from = i; /* another wildcard */
			ierr= MPI_Recv( temp , count ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
			ierr= MPI_Get_count (&status ,MPI_REAL,&stat_count ) ;
			stat_source = status.MPI_SOURCE;
			stat_tag = status.MPI_TAG;
			//printf( "Initial Status of receive : dest=%d , source=%d , tag=%d ,count=%d \n " , rank , stat_source , stat_tag , stat_count );
			int t;
			for (t=0;t<count;t++){
				push(q[i],temp[t]);
				//printf("%d \n",temp[t]);
			}
			fromQueueToHeap(q,i,mHeap);
			//insert(mHeap,peek(q[i]),i);
			//printf("head:%d\n",peek(q[i]));
			
		}
		for(i=0;i<size;i++){
			if (i==0)
				push(q[size-1],arr[0]);
			else
				push(q[size-1],arr[((i)*(k/(size*size)))]);
		}
		fromQueueToHeap(q,size-1,mHeap);
		int pivotval[size*size];
		int co=0;
		//printf("sorted pivot values\n");
		while(mHeap->size!=0){
			int *min = extractMin(mHeap);
			pivotval[co] = min[0];
			//printf("%d\t",pivotval[co]);
			fromQueueToHeap(q,min[1],mHeap);
			//printf("size:%d\n",mHeap->size);
			co++;
		}
		//printf("\n Pivots\n");
		for(i=0;i<size-1;i++){
			pivots[i] = pivotval[size*(i+1)];
			//printf("%d---\t",pivotval[size*(i+1)]);
		}
		//printf("\n");
		
	}
	ierr = MPI_Bcast(pivots,size-1,MPI_REAL,dest,MPI_COMM_WORLD);
	int ind = 0;
	int prevind = 0;
	int *temp2 = malloc(sizeof(int));
	int total = 0;
	for(i=0;i<size;i++){
		if(i!=size-1)
			ind = binarySearch(arr,prevind,noPerProcessor,pivots[i]);
		else
			ind = noPerProcessor-1;
		//printf("%d:%d:%d--%d\n",prevind,ind,i,rank);
		if(rank==i){
			int t;
			for(t=prevind;t<=ind;t++){
				push(q[i],arr[t]);
			}
			total+= ind-prevind+1;
			fromQueueToHeap(q,i,mHeap);
			int k;
			for(k=0;k<size-1;k++){
				from = MPI_ANY_SOURCE; /* another wildcard */
				tag = MPI_ANY_TAG; /* wildcard */
				ierr= MPI_Recv( temp2 , 1 ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
				ierr= MPI_Get_count (&status ,MPI_REAL,&stat_count ) ;
				stat_source = status.MPI_SOURCE;
				stat_tag = status.MPI_TAG;
				//printf( "Initial Status of receive : dest=%d , source=%d , tag=%d ,count=%d \n " , rank , stat_source , stat_tag , stat_count );
				int *temp1 = malloc(sizeof(int)*(*temp2));
				from = stat_source;
				tag = stat_tag;
				ierr= MPI_Recv( temp1 , *temp2 ,MPI_REAL, from , tag ,MPI_COMM_WORLD,&status ) ;
				ierr= MPI_Get_count (&status ,MPI_REAL,&stat_count ) ;
				stat_source = status.MPI_SOURCE;
				stat_tag = status.MPI_TAG;
				//printf( "Initial Status of receive : dest=%d , source=%d ,wanted=%d, tag=%d,wanted_tag=%d ,count=%d \n " , rank , stat_source,from , stat_tag ,tag, stat_count );
				for (t=0;t<*temp2;t++){
					push(q[from],temp1[t]);
					//printf("%d \n",temp1[t]);
				}
				total+= *temp2;
				fromQueueToHeap(q,from,mHeap);
				free(temp1);				
			}
		}
		else{
			*temp2= ind-prevind+1;
			//printf("%d:%d:%d\n",ind,prevind,rank);
			ierr = MPI_Send( temp2,1 ,MPI_REAL, i , rank ,MPI_COMM_WORLD);
			ierr = MPI_Send( &arr[prevind] , ind-prevind+1 ,MPI_REAL, i , rank ,MPI_COMM_WORLD);

		}
		prevind=ind+1;
		
	}
	arr = realloc(arr,sizeof(int)*total);
	int co=0;
	while(mHeap->size!=0){
		int *min = extractMin(mHeap);
		arr[co] = min[0];
		//printf("%d\t",arr[co]);
		fromQueueToHeap(q,min[1],mHeap);
		//printf("size:%d\n",mHeap->size);
		co++;
	}
	//printf("\n");
	gettimeofday(&end,NULL);
	printf("\n Time taken for execution on processor = %d is : %ld microseconds\n",rank,
				((end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec))); 
	for(i=0;i<total-1;i++){
		if(arr[i]>arr[i+1])
			printf("Not sorted at index:%d, proc:%d",i,rank);
	}
	//printf("Total no at proc-%d : %d\n",rank,total);
	/* if(rank==0){
		ierr = MPI_Send( &arr[total-1],1 ,MPI_REAL, rank+1 , rank ,MPI_COMM_WORLD);
	}
	else if(rank<size-1){
		ierr= MPI_Recv( temp2 , 1 ,MPI_REAL, rank-1 , rank-1 ,MPI_COMM_WORLD,&status );
		if(*temp2>arr[0]){
			printf("Not sorted between proc:%d, proc:%d",rank-1,rank);
		}
		ierr = MPI_Send( &arr[total-1],1 ,MPI_REAL, rank+1 , rank ,MPI_COMM_WORLD);
	}
	else{
	    ierr= MPI_Recv( temp2 , 1 ,MPI_REAL, rank-1 , rank-1 ,MPI_COMM_WORLD,&status );
		if(*temp2>arr[0]){
			printf("Not sorted between proc:%d, proc:%d",rank-1,rank);
		}
	} */
	printf("%d-: size = %d, start = %d, end = %d\n",rank,total,arr[0],arr[total-1]);
	ierr= MPI_Finalize();
	//printf("dine");

	return 0;
}
