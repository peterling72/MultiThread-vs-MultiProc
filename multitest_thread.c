#include <stdio.h>
#include <pthread.h>
#include <math.h>

int multi_mode = 0;
int PARTITION = 250;
int indexFound = 0;
int indexNotF = -1;
int size = 0;
int target = 0;
int numOfThreads = 0;
int currThread = 0;
int index = -1;
pthread_mutex_t lock;

void* search(void * args){ // function where we look for the target in our subarray
	int * array = (int *)args;
	pthread_mutex_lock(&lock);
	int num = currThread++;
	pthread_mutex_unlock(&lock);
	//printf("num = %d\n",num);
	if (num == (numOfThreads-1) && size % PARTITION != 0){ // for the uneven array if needed
		for (int i = num * PARTITION; i < size; i++){
			if (array[i] == target){
				index = i;
				pthread_exit(&indexFound);
			}
		}
	}
	else for (int i = num * PARTITION; i < ((num + 1) * (PARTITION)); i++){ // for arrays of size PARTITION (250)
	//	printf("%d ",i);
		if (array[i] == target){
			index = i;
			pthread_exit(&indexFound);
		}
	}
	pthread_exit(&indexNotF);
}
int multi(int * array, int * size1, int * target1){
	size = *size1;
	target = *target1;
	numOfThreads = size/PARTITION;
	if (size % PARTITION != 0) numOfThreads++; // calculate number of threads here
	pthread_t thread[numOfThreads];
	pthread_mutex_init(&lock,NULL);
	for (int i = 0; i < numOfThreads; i++){ // create threads
		pthread_create(&thread[i],NULL,search, array);
	}
	for (int i = 0; i < numOfThreads; i++){
		pthread_join(thread[i],NULL); //Wait until all threads are completed
	}
	//printf("\n");
/*
	if (index != -1)
		printf("Found target %d at index %d.\n",target,index);
	else
		printf("Could not find target %d in array.\n",target);
*/
	pthread_mutex_destroy(&lock);
	currThread = 0;
	return index;
}

