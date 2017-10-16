/*
 * Author: Fernando Cuevas
 * Class: CPSC 351, TUE/THUR 1:00-2:15
 * Professor: Prof. W. McCarthy
 * Due: 10/19/2017
 * Purpose: Use threads to sort an array of integers. Do this by spliting the array into two
 *          sub arrays sort those and then merge/sort the array in another thread.
 * Notes: Used bubble sort to sort the array of ints. Program  accounts for even/odd sized arrays, further exploration
 *        would be to randonly generating the numbers
 *        in the array.
 * */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_SIZE    12
/*#define MAX_SIZE    13*/    /*for odd testing*/
#define ARRAY_START  0
#define SORT_THREADS     2

/*Global Vars for the threads as well as func used*/
int unsortedArray[MAX_SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 10, -1}; /* for even testing */
/*int unsortedArray[MAX_SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 10, -1, -5}; /* uncomment to check with odd*/
int sortedArray[MAX_SIZE];
pthread_mutex_t lock;

void displayArray(int args[], int start, int end);
void getStartEnd(int aSize, int* start, int* end);
void *thread_SortA(int start_stop[]);
void *thread_SortB(int start_stop[]);
void *thread_Merge();


int main(){

    printf("Thread Sort:\n");
    printf("Pre-SOrt");
    displayArray(unsortedArray, ARRAY_START, MAX_SIZE);

    /*Start and end of array*/
    int start, end;
    /*pass the size of the array and pass start and end as pointers so we can get the start and end points of the
     * array. This helps us split it into two sub arrays we can work with*/
    getStartEnd(sizeof(unsortedArray) / sizeof(unsortedArray[0]), &start, &end);
    //printf("PRE -- %d, %d", start, end);
    int secondHalf[2] = {start, end}; /* will give us arguments for sorting second half of array*/
    int firstHalf[2] = {0, end/2}; /*will give us args sorting fist had of array start = 0 end = end / 2*/
                                    /* the divide by 2 is wrong for odd size, still sorts but if we have end == 13 then
                                     *  13/6 is 6.5 a float but b/c end is an int it gets converted and looses the .5
                                     *  hence why we get 6 which is correct but the way i get it is not*/
    int *thread_args[2] = {firstHalf, secondHalf}; /* array of arrays for the threads*/

    /*Thread Sorting*/
    printf("--------------------------------------------\n");
	pthread_t thread_id[SORT_THREADS];              /*Create an array of sort threads*/
    pthread_t merge_id;
	pthread_mutex_init(&lock, NULL);
    void* func_args[SORT_THREADS] = {thread_SortA, thread_SortB};  /* array of functions to pass to thread
                                                                    * during creation*/
    int checkThread; /*used to make sure return num of pthread create and join was not 0, if it was exit program*/
	int i;
    /*Create threads*/
	for (i = 0; i < SORT_THREADS; i++){
        checkThread = pthread_create(&thread_id[i], NULL, func_args[i], thread_args[i]);
        if(checkThread != 0){
            fprintf(stderr, "FAiled tp create thread %d", i);
            exit(EXIT_FAILURE);
        }
	}
    /* Join sort threads*/
	for (i = 0; i < SORT_THREADS; i++){
        checkThread = pthread_join(thread_id[i], NULL);

        if(checkThread != 0){
            fprintf(stderr, "FAiled tp joing thread %d", i);
            exit(EXIT_FAILURE);
        }
	}
    /* Thread Merging
     * After sort threads are done create a new thread that merges them
     * if all of the threads run in parallel then the merge thread gets an uncompleted copy
     * if we let the first two threads sort and then create the merge it dosent matter that thread 1 or 2 executes fist
     * so long as the sort threads sort and finish we will have an array where each half is sorted but not complete
     * this allows the merge thread to sort/merge, hopefully this is an adequate implementation.*/
    checkThread = pthread_create(&merge_id, NULL, thread_Merge, NULL);
    if(checkThread != 0){ /*error checking*/
        fprintf(stderr, "FAiled tp create merge thread");
        exit(EXIT_FAILURE);
    }
    checkThread = pthread_join(merge_id, NULL);
    if(checkThread != 0){
        fprintf(stderr, "FAiled tp joing merge thread %d", checkThread);
        exit(EXIT_FAILURE);
    }
    printf("--------------------------------------------\n");
    printf("Done Sorting");
    displayArray(sortedArray, ARRAY_START, MAX_SIZE);
	return 0;
}


void displayArray(int args[], int start, int end){
 	printf("\nContents of Array:\n\t[");
	int i;
	for (i = start; i < end; ++i){
 		assert(args[i] != NULL && i < end);
   		printf(" %d ", args[i]);
 	}
 		printf("]\n");
}

void getStartEnd(int aSize,int* start, int* end){
    assert(start);
    assert(end);
    if(MAX_SIZE % 2 == 0){
        /*gest the starting and ending points for array*/
        *end = aSize; /* sizeof(nums[0]); cannnot use size of sinze im getting soze of the pointer to the array*/
        /*since array used is even we dont need to check if odd and get midpoint*/
        *start = *end / 2;
        //printf("ahlf == %d, asize == %d\n", *start, *end);
    }else {
        int i;
        int sum = 0;
        for (i = 1; i <= aSize; i++){ /*Find the midpoint if even*/
            sum = sum + i;
        }
        sum = sum / MAX_SIZE;
        *end = aSize;
        *start = sum;

    }
    assert(start); /*make sure when the values come in they are not NULL and that they are not NULL when they leave*/
    assert(end);
}

void *thread_SortA(int start_stop[]) {
    int i;
    int j;
    printf("(%d, %d) --> ", start_stop[0], start_stop[1]);
    for (i = 0; i < 5; i++){
        for (j = start_stop[0]; j < start_stop[1] - i - 1; j++){
            if (unsortedArray[j] > unsortedArray[j + 1]){
                pthread_mutex_lock(&lock);
                int temp = unsortedArray[j];
                unsortedArray[j] = unsortedArray[j + 1];
                unsortedArray[j + 1] = temp;
                pthread_mutex_unlock(&lock);
            }
            assert(unsortedArray[j] < unsortedArray[j + 1]);  /*Make sure that we swapped correctly*/
        }
    }
    pthread_mutex_lock(&lock);
    printf("THREAD 1");
    displayArray(unsortedArray, start_stop[0], start_stop[1]);
    pthread_mutex_unlock(&lock);

}

void *thread_SortB(int start_stop[]){
    int x;
    int y;
    //pthread_mutex_lock(&lock);
    printf("(%d, %d) --> ", start_stop[0], start_stop[1]);
    for (x = 0; x < 5; x++){
        for (y = start_stop[0]; y < (start_stop[1] - x - 1); y++){
           // printf("in sencond for loop");
            if (unsortedArray[y] > unsortedArray[y + 1]){
                pthread_mutex_lock(&lock);
                int temp = unsortedArray[y];
                unsortedArray[y] = unsortedArray[y + 1];
                unsortedArray[y + 1] = temp;
                pthread_mutex_unlock(&lock); /*Make sure that we swapped correctly*/
            }
            assert(unsortedArray[y] < unsortedArray[y + 1]);
        }
    }
    pthread_mutex_lock(&lock);
    printf("THREAD 2");
    displayArray(unsortedArray, start_stop[0], start_stop[1]);
    pthread_mutex_unlock(&lock);

}

void *thread_Merge(){
    int i;
    int j;
    //pthread_mutex_lock(&lock);
    for (i = 0; i < MAX_SIZE; i++){
        sortedArray[i] = unsortedArray[i];
        assert(sortedArray[i] == unsortedArray[i]);
    }
    printf("Merging sorts... --> Thread 3");
    displayArray(sortedArray, ARRAY_START, MAX_SIZE);
    //pthread_mutex_unlock(&lock);

    /*final merge sort*/
    for (i = 0; i < MAX_SIZE; i++){
        for(j = 0; j < MAX_SIZE - i - 1; j++){
            if(sortedArray[j] > sortedArray[j + 1]){
                pthread_mutex_lock(&lock);
                int temp = sortedArray[j];
                sortedArray[j] = sortedArray[j + 1];
                sortedArray[j + 1] = temp;
                pthread_mutex_unlock(&lock);
            }
            assert(sortedArray[j] < sortedArray[j + 1]); /* Make sure that we swapped correctly*/
        }
    }


}
