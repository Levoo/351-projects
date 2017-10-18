/* same same, but using omp
 * Author: Fernando 
 * Class: CPSC 351*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRIALS 1000000 /*MODIFY THIS TO GET MORE PRECISES*/
#define SQUARE_BOUND 1
int hits;

void *carlos();

int main(){
    double yummyPI;
    int i;
    int thread = 0;
    srand(time(0));
    hits = 0;
    double xCoord, yCoord;
    #pragma omp parallel
    {
      /*thread = omp_get_thread_num(); /*Vefiy*/
      /*printf("thread: %d\n", thread); /* how many threads are being used*/
    #pragma omp for
        for(i = 0;i < TRIALS; i++){
            xCoord = (double)rand() / RAND_MAX; /*gen a number divide it by RAND_MAX*/
            yCoord = (double)rand() / RAND_MAX; /*where RAND_MAX is acatually the largest possible number
                                             * that can be generaated and is based on the input given to srand*/
            /*Circle is of d = 1, r = .5 so for (x, y) to be in circle (x^2 + y^2) <= 1*/
            if((xCoord*xCoord + yCoord*yCoord) <= SQUARE_BOUND){ hits++; }
        }
    }
    yummyPI = 4*((double)hits/TRIALS);
    printf("ACTUAL   PI: 3.141592\n");
    printf("ESTIMATE PI: %f\n", yummyPI);

    return 0;
}

/*Sample output since srand has time as the seed will get a diff result each time!
 * with thread output
 *  thread: 0
    thread: 2
    thread: 3
    thread: 1
    ACTUAL   PI: 3.141592
    ESTIMATE PI: 3.115048
    without thread output
    ACTUAL   PI: 3.141592
    ESTIMATE PI: 3.121596

 * */
