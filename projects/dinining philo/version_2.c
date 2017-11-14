/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project: Dining Philosophers Problem
 *
 * Authors: Fernando C
 *          Matthew M
 *
 * Class: CPSC 351 Tue/Thur
 * Due: 11/14/2017
 *
 * Notes: Used Mutex and semaphors,
 *        MUtex to enter critical area and see if we can get BOTH chopsticks at once,
 *        But while trying to get the chopstick, we decrement the sem count if we cannot get two at once we
 *        wait until we can and then proceed.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define NUM_PHILO     5
#define PICKUP        0
#define PUTDOWN       1
#define FULL_STOMACH  3
/*/global vars for threads(aka philosophers)*/
/*/ 4 chopsticks where FALSE means they are not picked up/not in use and TRUE means they have been picked up/in use*/
char phil_state[NUM_PHILO] = {'T','T', 'T', 'T','T'};
pthread_mutex_t canEat;

sem_t amt;
const char* semaphore_name = "/amt";
/* FUNCTIONS USED
 * dining_table   : thread functions that will be the philosophers requesting/realeasing chopsticks until done eating
 * waiter         : Requestes to pick up and put down the chopsticks will be made here.
 * check_on_table : get statuse of the table
 * */
void* dining_table(void* param);
void  waiter(int phil_num, int request);
void  check_on_table(int bites);

int main() {
    pthread_t philosophers[NUM_PHILO];
    pthread_attr_t phil_attr;
    pthread_attr_init(&phil_attr);
    pthread_mutex_init(&canEat, NULL); /*   init mutex to default value */

    if((sem_init(&amt, 0, 5)) == -1){
        fprintf(stderr, "sem_open failed\n");
    }
    /*Table to display the output*/
    printf("=========================================================================\n");
    printf("|                    ====== PHILOSOPHERS ======                         |\n");
    printf("=========================================================================\n");
    printf("   Phil_0   |     Phil_1   |     Phil_2   |     Phil_3   |    Phil_4    |\n");
    check_on_table(0);

    int i;
    int check;
    /* CREATE THREAD */
    for(i = 0; i < NUM_PHILO; i++){
        if(pthread_create(&philosophers[i], &phil_attr, dining_table, (void*) i) != 0){ // if error on create terminate
            printf("Error creating thread # %d", i);
            exit(1);
        }
    }

    /* JOIN THREAD */
    for(i = 0; i < NUM_PHILO; i++){
        if(pthread_join(philosophers[i], NULL) != 0){ // if error on join terminate
            printf("Error joining thread # %d", i);
            exit(1);
        }
    }

    check_on_table(0); /* final check*/
    printf("\n...DONE.\n");


    sem_close(&amt);
    sem_unlink(semaphore_name);

    return 0;
}

void* dining_table(void* param){
    int phil_num = (int)param; // atoi keeps going if not successful, look for alt conversion if you can!
    int bitesTake = 0;

    /*
     * while eaten is less than 10 (means while the guy has not eaten more than 10 times)
     *      think for a while (make the guy wait a random amount of time maybe vary between 1-5 seconds before they request)
     *      pickup(chopstick[i], chopstick[i+1 mod 5]) (the mod 5 takes care if i is 4 since the table is circular then 4+1%5 == 0)
     *      eat; (where eat means he will increment his local var of bites taken and eat for a while then let go)
     *      putdown (guy puts down the chopsticks use and then goes back to thinking)
     * */
    while(bitesTake != FULL_STOMACH){

        /*THINKNING*/
        sleep(2);
        /*REQUEST*/
        if(phil_state[phil_num] == 'T') {
            phil_state[phil_num] = 'H';
            pthread_mutex_lock(&canEat);
            waiter(phil_num, PICKUP);
            pthread_mutex_unlock(&canEat);
        }

        /* eating */
        /*LET GO*/
        if(phil_state[phil_num] == 'E') {
            sem_wait(&amt); /*  aquire two chop sticks then update and eat*/
            sem_wait(&amt); /* if cannot aquire two will wait until released*/
            sleep(2);
            pthread_mutex_lock(&canEat);
            check_on_table(bitesTake);
            waiter(phil_num, PUTDOWN);
            bitesTake++;
            pthread_mutex_unlock(&canEat);
            sem_post(&amt); /* let go of chopsticks*/
            sem_post(&amt);
        }

        /*In the event that a phil is hungry he keeps checking if he can get the sticks */
        if(phil_state[phil_num] == 'H'){
            //
            pthread_mutex_lock(&canEat);
            waiter(phil_num, PICKUP);
            pthread_mutex_unlock(&canEat);
        }
    }
    phil_state[phil_num] = 'T';
}

void waiter(int phil_num, int request){
    if(request == PICKUP){
        /*Try to pick up chopsticks, else go back to being hungry*/
        /*first check to the left and right */
        /*then check one more to the left and one more to the right to make sure*/
        if(phil_state[phil_num] == 'H' && phil_state[(phil_num+4) % 5] != 'E' && phil_state[(phil_num + 1) % 5] != 'E'){
                phil_state[phil_num] = 'E';
        }
    }else if(request == PUTDOWN){
        phil_state[phil_num] = 'T';
    }
}

void check_on_table(int bites){
    int i;
    for(i = 0; i < NUM_PHILO; i++){
        if     (phil_state[i] == 'T')
            printf("   thinking   |");

        else if(phil_state[i] == 'H')
            printf("   *hungry*   |");

        else if(phil_state[i] == 'E')
            printf("   EATING(%d) |", bites+1);
        // printf("%c     |", phil_state[i]);
    }
    printf("\n---------------------------------------------------------------------------\n");
}
