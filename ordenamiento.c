#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>


#define ELEMS 20000000 // Elementos a ordenar

int errors(int *numbers,int elems);
void initArr(int *numbers,int elems);
void SortArr(int *numbers);
void merge(int *numbers, int l, int m, int r);
void mergeSort(int *numbers, int l, int r);

int arr[ELEMS];

int main()
{
	int i;
	int n;

	long long start_ts;
	long long stop_ts;
	float elapsed_time;
	long lElapsedTime;
	struct timeval ts;
	
	initArr(arr,ELEMS);

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	#pragma omp parallel
	#pragma omp single
	SortArr(arr);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
	
	if((n=errors(arr,ELEMS)))
		printf("Se encontraron %d errores\n",n);
	else
		printf("%d elementos ordenados en %1.2f segundos\n",ELEMS,elapsed_time);
}

void initArr(int *numbers,int elems)
{
	int i;
	for(i=0;i<elems;i++)
		numbers[i]=rand()*rand()%ELEMS;

}

int errors(int *numbers,int elems)
{
	int i;
	int errs=0;
	for(i=0;i<elems-1;i++)
		if(numbers[i]>numbers[i+1])
			errs++;
	return(errs);
}

 
void SortArr(int *numbers)
{

	mergeSort(numbers, 0, ELEMS-1);
}

void merge(int *numbers, int l, int m, int r)
{
	int i, j, k, n, h, *aux;
	i = l;
	j = m+1;
	k = 0;
	n = (r-l)+1;
	aux = (int*)malloc(n*sizeof(int));

	while((i<=m) && (j<=r))
	{
		if(numbers[i] < numbers[j])
			aux[k++] = numbers[i++];
		else
			aux[k++] = numbers[j++];
	}

	while(i<=m)
		aux[k++] = numbers[i++];
	while(j<=r)
		aux[k++] = numbers[j++];

	for(h = 0; h < n; h++)
		numbers[h+l] = aux[h];

	free(aux);
}

void mergeSort(int *numbers, int l, int r)
{

	if(l < r)
	{		
		
	
		int m = l+(r-l)/2;

		#pragma omp task
		mergeSort(numbers, l, m);	
			
	
		mergeSort(numbers, m+1, r);	

		#pragma omp taskwait
		merge(numbers, l, m, r);
	

		
		
	
	}
}


