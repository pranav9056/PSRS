#include <stdio.h>
#include "mpi.h"
#include "merge.h"
int main ( int argc , char **argv )
{
	int i , ierr, rank , size , dest , source , from , to , count , tag ;
	int stat_count , stat_source , stat_tag ;
	int data[100] ;
	MPI_Status status ;
	MPI_Init (&argc ,& argv ) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank ) ;
	MPI_Comm_size (MPI_COMM_WORLD, &size ) ;
	printf ( "I am process %d of %d \n " , rank , size ) ;
	dest=size-1;
	source=0;
	int arr[] = {2, 8, 3, 5, 6, 7,1};
	int arr_size = sizeof(arr)/sizeof(arr[0]);
	sort(arr,arr_size);
	printf("Sorted array is \n");
	printArray(arr,arr_size);
	ierr= MPI_Finalize ( ) ;
	return 0;
}
