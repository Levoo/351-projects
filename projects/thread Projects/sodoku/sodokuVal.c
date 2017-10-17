/*
 * Author: Fernando Cuevas
 * Class: CPSC 351, TUE/THUR 1:00-2:15
 * Professor: Prof. W. McCarthy
 * Due: 10/19/2017
 * Purpose: Use multithreading to validate sudoku puzzle
 * Notes: I feel that just checking
 * */


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define NTHREADS 9
/**/

#define PUZZLE_LENGTH 9
#define CORRECT_SUM 45
typedef struct{
    int row;
    int col;
}parameters;

/*    A           B           C                 CORRECT */

int sudokoSolution1[9][9] =  { {6, 2, 4,   5, 3, 9,   1, 8, 7}, // 6, 2, 4, 5, 3, 9, 1, 8, 7
                               {5, 1, 9,   7, 2, 8,   6, 3, 4}, // 5, 1, 9, 7, 2, 8, 6, 3, 4
                               {8, 3, 7,   6, 1, 4,   2, 9, 5}, // 8, 3, 7, 6, 1, 4, 2, 9, 5

                               {1, 4, 3,   8, 6, 5,   7, 2, 9}, // 1, 4, 3, 8, 6, 5, 7, 2, 9
                               {9, 5, 8,   2, 4, 7,   3, 6, 1}, // 9, 5, 8, 2, 4, 7, 3, 6, 1
                               {7, 6, 2,   3, 9, 1,   4, 5, 8}, // 7, 6, 2, 3, 9, 1, 4, 5, 8

/*    D           E           F  */

                               {3, 7, 1,   9, 5, 6,   8, 4, 2}, // 3, 7, 1, 9, 5, 6, 8, 4, 2
                               {4, 9, 6,   1, 8, 2,   5, 7, 3}, // 4, 9, 6, 1, 8, 2, 5, 7, 3
                               {2, 8, 5,   4, 7, 3,   9, 1, 6} //  2, 8, 5, 4, 7, 3, 9, 1, 6

/*    G           H           I  */

};

/*  Incorrect output to check
int sudokoSolution1[9][9] =  { {6, 2, 6,   5, 3, 9,   1, 8, 7}, // 6, 2, 4, 5, 3, 9, 1, 8, 7
                               {5, 1, 9,   7, 2, 8,   6, 3, 4}, // 5, 1, 9, 7, 2, 8, 6, 3, 4
                               {8, 3, 7,   6, 1, 4,   2, 9, 5}, // 8, 3, 7, 6, 1, 4, 2, 9, 5

                               {1, 4, 3,   8, 6, 5,   7, 1, 9}, // 1, 4, 3, 8, 6, 5, 7, 2, 9
                               {9, 5, 8,   8, 4, 7,   3, 6, 1}, // 9, 5, 8, 2, 4, 7, 3, 6, 1
                               {7, 6, 2,   3, 9, 1,   4, 5, 8}, // 7, 6, 2, 3, 9, 1, 4, 5, 8

                               {3, 7, 1,   9, 5, 6,   8, 4, 2}, // 3, 7, 1, 9, 5, 6, 8, 4, 2
                               {4, 9, 6,   1, 8, 2,   5, 8, 3}, // 4, 9, 6, 1, 8, 2, 5, 7, 3
                               {2, 8, 5,   4, 1, 3,   9, 1, 6} //  2, 8, 5, 4, 7, 3, 9, 1, 6

};

*/


int validRow[PUZZLE_LENGTH] = {0};
int validCol[PUZZLE_LENGTH] = {0};
pthread_mutex_t locker;

void *isValid(void* checkRC);


int main(){

    pthread_t thread_ids[NTHREADS];
    pthread_mutex_init(&locker, NULL);

    int index;
    int checkThread;
    parameters *data;


/*CREATE*/

    for (index = 0; index < NTHREADS; index++){
            data = (parameters *) malloc(sizeof(parameters));
            (*data).row = index;
            (*data).col = index;
            checkThread = pthread_create(&thread_ids[index], NULL, isValid, (void *) data);
            if (checkThread != 0) {fprintf(stderr, "FAiled tp create thread %d", index); exit(EXIT_FAILURE);}
    }

/*JOIN*/

    for(index = 0; index < NTHREADS; index++){
        checkThread = pthread_join(thread_ids[index], NULL);
        if(checkThread != 0){fprintf(stderr, "FAiled tp join thread %d", index); exit(EXIT_FAILURE);};
    }

    printf("Checkin If Valid:\n");
    int isValid = 0;
/*set to zero and if stays as zero then valid else not valid*/

    for(index = 0; index < 9; index++){
        //printf("Row %d = %d || COL %d = %d \n", index, validRow[index], index, validCol[index]);
        if(validRow[index] == 1){
            isValid = validRow[index];
            printf("Invalid Row %d = %d \n", index+1, validRow[index]);
        }if(validCol[index] == 1){
            isValid = validCol[index];
            printf("Invalid Col %d = %d \n", index+1, validCol[index]);
        }
    }

    if (isValid != 0){
        printf("\nNot Valid Solution!\n");
    }else{
        printf("\nValid Solution!\n");
    }

    return 0;
}

void *isValid(void* checkRC){

/* sudoku tasks you to fill in the rows in columns with 1-9 with no repeats of 1-9 in the row and column
     * b/c of this the summation of the column and the row is 45(1+2+3+4+5+6+7+8+9)
     * therefore a cheap way to check if a solution is valid is to check row and col at the same time and see if
     * there sum is 45 if the row or/and the col's sum is not 45 then there must be duplicates and thus not a valid
     * solution. If all checks out then is a valid solution*/

    parameters *threadData = (parameters*) checkRC;
    int row = (*threadData).row;
    int col = (*threadData).col;

    int i, j;
    int rowSum;
/*will hold value of the row and the column*/

    int colSum;
    //check
    for(i = 0; i < PUZZLE_LENGTH; i++){ //get sum of just the row
        //pthread_mutex_lock(&locker);
        rowSum += sudokoSolution1[row][i];
        //pthread_mutex_unlock(&locker);
    }

    for(j = 0; j < PUZZLE_LENGTH; j++){ //get sum of just the col
        //pthread_mutex_lock(&locker);
        colSum += sudokoSolution1[j][col];
        //pthread_mutex_unlock(&locker);
    }
    pthread_mutex_lock(&locker);
   // printf("-----------------------------------------\n");
   // printf("checking row: %d || col: %d\n", row, col);
    //printf("rowSUM == %d || colSUM == %d \n", rowSum, colSum);
    if(rowSum != CORRECT_SUM){
/* if not equal then set row bit to 1.
                                         * so if row 0 (aka row 1) is say, 38 then not a valid solution.
                                         *  */

        validRow[row] = 1;
    }if(colSum != CORRECT_SUM){
        validCol[col] = 1;
    }//else{assert(rowSum == CORRECT_SUM); assert(colSum == CORRECT_SUM);}
   // printf("\n-----------------------------------------\n");

    pthread_mutex_unlock(&locker);
    free(threadData);
}
