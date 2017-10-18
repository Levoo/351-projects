/*
 * Author: Fernando
 * Class: CPSC 351*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TRIALS 1000000 /*MODIFY THIS TO GET MORE PRECISES*/
#define SQUARE_BOUND 1
int hits;

void *carlos();

int main(){
    double yummyPI;
    int i;
    pthread_t monty;

    pthread_create(&monty, NULL, carlos, NULL);
    pthread_join(monty, NULL);


    yummyPI = 4*((double)hits/TRIALS);
    printf("ACTUAL   PI: 3.141592\n");
    printf("ESTIMATE PI: %f", yummyPI);

    return 0;
}

void *carlos(){
    srand(time(0));
    hits = 0;
    double xCoord, yCoord;
    int i;
    for(i = 0;i < TRIALS; i++){
        xCoord = (double)rand() / RAND_MAX; /*gen a number divide it by RAND_MAX*/
        yCoord = (double)rand() / RAND_MAX; /*where RAND_MAX is acatually the largest possible number
                                             * that can be generaated and is based on the input given to srand*/
        /*Circle is of d = 1, r = .5 so for (x, y) to be in circle (x^2 + y^2) <= 1*/
        if((xCoord*xCoord + yCoord*yCoord) <= SQUARE_BOUND){ hits++; }
    }
}
/*Sample output since srand has time as the seed will get a diff result each time!
 *
 * ACTUAL   PI: 3.141592
   ESTIMATE PI: 3.141136
 * */
