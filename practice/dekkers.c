#include <pthread.h>
#include <stdio.h>

typedef enum defer{p0,p1}defer;
defer should_defer = p0;
int p0_wantsCritical = 0;
int p1_wantsCritical = 0;

void* P0(){
  int finished = 0;
  while(!finished){
    p0_wantsCritical = 1;
    while(p1_wantsCritical){
      if(should_defer == p0){
        p0_wantsCritical = 0;
        while(should_defer == p0) { /* do nothing */}
        p0_wantsCritical = 1;
      }
    }
    printf("p0 is critical\n");
    should_defer = p0;
    p0_wantsCritical = 0;
  }
  return NULL;

}

void* P1(){
  int finished = 0;
  while(!finished){
    p1_wantsCritical = 1;
    while(p0_wantsCritical){
      if(should_defer == p1){
        p1_wantsCritical = 0;
        while(should_defer == p1) { /* do nothing */}
        p1_wantsCritical = 1;
      }
    }
    printf("p1 is critical\n");
    should_defer = p1;
    p1_wantsCritical = 0;
  }
}


int main(){
    pthread_attr_t att;
    pthread_t thread0;
    pthread_t thread1;

    pthread_attr_init(&att);

    pthread_create(&thread0, &att, P0, NULL);
    pthread_create(&thread1, &att, P1, NULL);

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);


  return 0;
}
