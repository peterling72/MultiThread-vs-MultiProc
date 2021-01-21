#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>

int multi_mode = 1;
int PARTITION = 250;
int search (int* array, int size, int target, int i){ // function where we look for the target in our subarray
  int startIndex = i*PARTITION;
  int x = 0;
  if((startIndex+PARTITION)>size){ //this is an uneven array, do something lil bit different
    for(x = startIndex; x < size; x++){
      if(array[x] == target){
	int temp = x - (i*PARTITION); //this will give a number between 0-PARTITION, which is smaller than 8 bits, we will reverse this later
	return temp;
      }
    }
  }
  else{ //for just a normal PARTITION size array
    for(x = startIndex; x < (startIndex+PARTITION); x++){
      if(array[x] == target){
	int temp = x - (i*PARTITION);
	return temp;
      }
    }
  }
  return 255; //element was not found
}



int multi(int* array, int* size1, int* target1){
  int size = *size1;
  int target = *target1;
  int numOfProcs = size/PARTITION; // calculate how many processes are going to be needed here
  if (size % PARTITION != 0) numOfProcs++;
  int i = 0;
  int pid[numOfProcs]; // create an array to hold the pids of the created child processes
  int p = 100;
  for(i = 0; i < numOfProcs; i++){
    p = fork();
    if(p  == -1){
      printf("unable to create process");
      i = i - 1; //loop will go around and try again
    }
    else if(p == 0){ //child process will take the array and do a search of part of it depending on i
      exit(search(array, size, target, i));
    }
    else{
      pid[i] = p; // fork returns child pid to the parent process and we store it into our array for later
    }
  }
  int status;
  int finalIndex = -1;
  for(i = 0; i < numOfProcs; i++){
    waitpid(pid[i], &status, 0); // wait for the child processes to finish in order
    if(WIFEXITED(status)){
	int exitStat = WEXITSTATUS(status);
	if(exitStat < PARTITION){ // if the child process returned a number that is less than PARTITION, it means target was found and we calculate the finalIndex
	  finalIndex = exitStat + (i*PARTITION);
	  //printf("Found target %d at index %d.\n", target, finalIndex);
	 // break;
	}
    }
  }
  return finalIndex;
}
