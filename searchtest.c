#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include "multitest.h"

int verbose = 1;
//How many iterations to run for a test (how vigorously we test)
int VIGOR = 500;
int test(int size){
	int* array = (int*)malloc(size * sizeof(int)); //make array
	int i;
	for(i=0; i<size; i++){
		array[i] = i; //propagate values into array strting from 0
	}
	double times[VIGOR];
	double min = DBL_MAX;
	double max = DBL_MIN;
	double sum = 0;
	int numOfSwaps = ((int)size*0.75)+1; //swaps three-quarters of array size (just going to +1 to go a little overboard because of truncation
	int index1=0; //used for swapping, first index
	int index2=0; //used for swapping, second index
	int temp=0; //used to temporarily store value for swapping
	for(i=0; i < numOfSwaps; i++){
		index1 = rand() % size;
		index2 = rand() % size; //generates random numbers between 0-size and puts them in the variables
		temp = array[index1];
		array[index1] = array[index2];
	 	array[index2] = temp; //this should conclude our swapping of values
	}
	i=0;
	int target = 5; //placeholder target value, can change this later to be whatever
	while(i<VIGOR){//do VIGOR tests
		index1 = rand() % size;
		index2 = rand() % size; //to generate new array, assignment says we can just generate a pair of indices and swap
		
		while(index1 == index2){ //this just makes sure the pair of indices are not equal
 	 		index1 = rand() % size;
 	 		index2 = rand() % size; 
 		}
		temp = array[index1];
		array[index1] = array[index2];
 		array[index2] = temp;
	
		//Search for target, time how long it takes	
		struct timeval tv1, tv2;
		gettimeofday(&tv1, NULL);
		int index3 = multi(array,&size,&target);
		gettimeofday(&tv2, NULL);
		times[i] =	(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         			(double) (tv2.tv_sec - tv1.tv_sec);
		//Convert to microsec
		times[i] *= 1000000;

		//Optional argument to program can disable this printout
		if (verbose == 1){
			if (index3 != -1) printf("Iteration %d: Found target %d at index %d in %f microseconds.\n",i,target,index3,times[i]);
			else printf("Iteration %d: Failed to find target %d after %f microseconds.\n",i,target,times[i]);
		}
		//Update min/max/sum
		if (times[i] < min)
			min = times[i];
		if (times[i] > max)
			max = times[i];
		sum += times[i];

		//Move target to random position after being found
		if (index3 != -1){
			while (index3 == index1){
				index1 = rand() % size;
			}
		}
		temp = array[index1];
		array[index1] = array[index3];
		array[index3] = temp;
		//Loop increment
		i++;
	}
	double mean = sum / VIGOR;
	double sum2 = 0;
	i = 0;
	while (i < VIGOR){
		sum2 += pow(times[i]-mean,2);
		i++;
	}
	double stddev = sum2 / (VIGOR-1);
	stddev = sqrt(stddev);
	printf("Results for %d runs of search on size %d array (in microseconds):\n",VIGOR, size);
	printf("\tmin: %f\n\tmax: %f\n\taverage: %f\n\tstd-dev: %f\n",min,max,mean,stddev);
	return 0;
}
int main(int argc, char** argv){
	//Seed RNG
	srand(time(0));
	
	if (argc != 3){
		printf("Arguments required to run searchtest:\n\tArgument 1: Test Suite (0 = Increasing Array Size, 1 = Increasing Worker Count)\n\tArgument 2: Verbose mode (0 = off, 1 = on)\n\nExample of proper execution: ./searchtest 0 0\n");
		return -1;
	}
	
	
	if (atoi(argv[2]) == 0){
		printf("Disabling iteration printouts.\n");
		verbose = 0;
	}
	//Pull what kind of test this is.
	if (multi_mode != 0) printf("Executing searchtest in multi-process mode\n");
	else printf("Executing searchtest in multi-thread mode\n");
	
	//Set amount of iterations for each test
	VIGOR = 5000;
	if (atoi(argv[1]) == 0){
		printf("Running Increasing Array Size tests...\n");
		//To enter additional tests, enter line test(size of choice)
		printf("Test A: %d searches in size 500 array.\n",VIGOR);
		test(500);
		printf("Test B: %d searches in size 1000 array.\n",VIGOR);
		test(1000);
		printf("Test C: %d searches in size 1001 array.\n",VIGOR);
		test(1001);
		printf("Test D: %d searches in size 2000 array.\n",VIGOR);
		test(2000);
		printf("Test E: %d searches in size 4000 array.\n",VIGOR);
		test(4000);
		printf("Test F: %d searches in size 6000 array.\n",VIGOR);
		test(6000);
		printf("Test G: %d searches in size 8000 array.\n",VIGOR);
		test(8000);
		printf("Test H: %d searches in size 10000 array.\n",VIGOR);
		test(10000);
		printf("Test I: %d searches in size 12000 array.\n",VIGOR);
		test(12000);
		printf("Test J: %d searches in size 14000 array.\n",VIGOR);
		test(14000);
		printf("Test K: %d searches in size 16000 array.\n",VIGOR);
		test(16000);
		printf("Test L: %d searches in size 18000 array.\n",VIGOR);
		test(18000);
		printf("Test M: %d searches in size 20000 array.\n",VIGOR);
		test(20000);	
	} else{
		printf("Running Increasing Worker Count tests...\n");
		PARTITION = 250;
		printf("Test A: %d searches in size 250 array, splitting every %d units, generating 1 worker(s).\n",VIGOR,PARTITION);
		test(250);
		PARTITION = 125;
		printf("Test B: %d searches in size 250 array, splitting every %d units, generating 2 worker(s).\n",VIGOR,PARTITION);
		test(250);
		PARTITION = 84;
		printf("Test C: %d searches in size 250 array, splitting every %d units, generating 3 worker(s).\n",VIGOR,PARTITION);
		test(250);
		PARTITION = 63;
		printf("Test D: %d searches in size 250 array, splitting every %d units, generating 4 worker(s).\n",VIGOR,PARTITION);
		test(250);
		PARTITION = 50;
		printf("Test E: %d searches in size 250 array, splitting every %d units, generating 5 worker(s).\n",VIGOR,PARTITION);
		test(250);
		PARTITION = 42;
		printf("Test F: %d searches in size 250 array, splitting every %d units, generating 6 worker(s).\n",VIGOR,PARTITION);
		test(250);
		/*
  		PARTITION = 250;
		printf("Test A: %d searches in size 2500 array, splitting every %d units, generating 10 workers.\n",VIGOR,PARTITION);
		test(2500);
		PARTITION = 228;
		printf("Test B: %d searches in size 2500 array, splitting every %d units, generating 11 workers.\n",VIGOR,PARTITION);
		PARTITION = 209;
		test(2500);
		printf("Test C: %d searches in size 2500 array, splitting every %d units, generating 12 workers.\n",VIGOR,PARTITION);
		test(2500);
		PARTITION = 194;
		printf("Test D: %d searches in size 2500 array, splitting every %d units, generating 13 workers.\n",VIGOR,PARTITION);
		test(2500);
		PARTITION = 179;
		printf("Test E: %d searches in size 2500 array, splitting every %d units, generating 14 workers.\n",VIGOR,PARTITION);
		test(2500);
		PARTITION = 167;
		printf("Test F: %d searches in size 2500 array, splitting every %d units, generating 15 workers.\n",VIGOR,PARTITION);
		test(2500);
		PARTITION = 157;
		printf("Test G: %d searches in size 2500 array, splitting every %d units, generating 16 workers.\n",VIGOR,PARTITION);
		test(2500);
		PARTITION = 148;
		printf("Test H: %d searches in size 2500 array, splitting every %d units, generating 17 workers.\n",VIGOR,PARTITION);
		test(2500);
		*/
	}	
	return 0;
}
