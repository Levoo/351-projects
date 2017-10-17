/*
 * Purpose:
 * Notes:
 * */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define NTHREADS 3 //rename to block thread
#define ROW 3
#define COL
typedef struct{
    int row;
    int col;
    int block;
}parameters;
                            /*    A           B           C  */
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
char sblock[9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'}; /*To determin which block we are checking*/
pthread_mutex_t locker;

void *checkBlock(void* checkRC);


int main(){

    pthread_t thread_ids[NTHREADS];
    pthread_mutex_init(&locker, NULL);

    int index;
    int j;
    int checkThread;
    parameters *data;
    for (index = 0; index < NTHREADS; index++){
        for(j = 0; j < 3; j++) {
            data = (parameters *) malloc(sizeof(parameters));
            (*data).row = j + index;
            (*data).col = j + index;
            (*data).block = j + index;
            //printf("in data,  row == %d col == %d\n", data->row, data->col);
            checkThread = pthread_create(&thread_ids[index], NULL, checkBlock, (void *) data);
            if (checkThread != 0) {
                fprintf(stderr, "FAiled tp create thread %d", index);
                exit(EXIT_FAILURE);
            }

            //printf("Thread # %d created\n", index);
        }
        printf("%d %d >> loops stuff\n", index, j);
    }


    for (index = 0; index < NTHREADS; index++){
        checkThread = pthread_join(thread_ids[index], NULL);
        if(checkThread != 0) {
            fprintf(stderr, "FAiled tp join thread %d, ERROR CODE: %d", index, checkThread);
            exit(EXIT_FAILURE);
        }
    }


    return 0;
}

void *checkBlock(void* checkRC){
    /*checking [0][n-1] && [n-1][0]*/
    pthread_mutex_lock(&locker);
    parameters *threadData = (parameters*) checkRC;
    printf("----------------------< %c >-------------------------\n", sblock[(*threadData).block]);
    int i;
    int j;
    int isUnique;
    int rowCount = 0, columnCount = 0;
    int dupBit = 0;

    //check
    for(i = (*threadData).col; i < (*threadData).col+3; i++){
        isUnique = sudokoSolution1[(*threadData).row][i];
        for(j = 0; j < 9; j++){
            if(isUnique == sudokoSolution1[(*threadData).row][j]){
                rowCount++;
            }
        }

        for(j = 0; j< 9; j++){
            if(isUnique == sudokoSolution1[j][i]){
                columnCount++;
            }
        }
        if (rowCount > 1){dupBit++;}
        if (columnCount > 1){dupBit = dupBit + 2;}


        printf("Total of %d found in row: %d in col: %d. DUPS BIT (n0/r1/c2/b3): %d\n", isUnique, rowCount, columnCount, dupBit);

        rowCount = 0;
        columnCount = 0;
    }
    printf("-----\n");

    printf("----------------------< %c >-------------------------\n", sblock[(*threadData).block]);
    printf("\n");
    free(threadData);
    pthread_mutex_unlock(&locker);
}
