#include <stdio.h>

void merge(int arr[] , int l, int m ,int r){
	int n1 = m-l+1;
	int n2 = r-m; 
	int t1[n1];
	int t2[n2];
	int i,j,k;
	j=0;
	for (i=l;i<m+1;i++){
		t1[j]=arr[i];
		j++;
	}
	j=0;
	for (i=m+1;i<r+1;i++){
		t2[j]=arr[i];
		j++;
	}
	i=0;
	j=0;
	k=l;
	while(i<n1 && j<n2){
		if(t1[i]<=t2[j]){
			arr[k++]=t1[i++];
		}
		else{
			arr[k++]=t2[j++];
		}
	}
	while(i!=n1){
		arr[k++]=t1[i++];
	}
	while(j!=n2){
		arr[k++]=t2[j++];
	}
	
}

void mergeSort(int arr[],int l,int r){
	if(l<r){
		int m = (l+r)/2;
		mergeSort(arr,l,m);
		mergeSort(arr,m+1,r);
		merge(arr,l,m,r);
	}
}

void printArray(int A[],int arr_size)
{
    int i;
    for (i=0; i < arr_size; i++){
		printf("%d ", A[i]);
	}
    printf("\n");
}
void sort(int arr[],int arr_size){
    mergeSort(arr, 0, arr_size - 1);
}
