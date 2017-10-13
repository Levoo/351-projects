#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_SIZE 10

int unsortedArray[10] = {2, 3, 8, 9, 0, 4, 6, 5, 7, 1}; /* Global and shared between all threads
                          will populate it with random numbers to prove that
                          the theads are actually sorting it.
                      */
int sortedArray[10];
void displayArray(int args[]);

int main(){
  pthread_t threadID1, threadID2;
  pthread_attr_t threadAttr;

  displayArray(unsortedArray);

  return 0;
}

void displayArray(int args[]){
  int i;
  printf("Contents of Array:\n\t[");
  for (i = 0; i < MAX_SIZE; ++i){
    assert(args[i] >=  0 && i < MAX_SIZE);
    printf(" %d ", args[i]);
  }
  printf("]\n");
}
