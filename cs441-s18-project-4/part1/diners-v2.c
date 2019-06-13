/*
 * Samantha Foley
 *
 * Project 4: Dining Philosophers
 * Deadlock Free Algorithm
 *
 */
#include "diners.h"

// LEFT defined as ( (i + numDiners - 1) % numDiners)
// Right defined as ( (i + 1) % numDiners)
semaphore_t mutex; //-- suspend other processes until the mutex is signaled
semaphore_t *chopstick;
clock_t start;
clock_t current;
int *state;
int stillEating; //global var for threads to check if they are done
phil* philosophers;
int numDiners;

int main(int argc, char * argv[]) {
    philosophers = NULL;
    if (argc < 1) {
        printf("ERROR: no arguments were entered!\n");
        exit(1);
    }
    stillEating = 0; //set to TRUE
    
    numDiners = 5; //initialize numDiners to 5 by default
    int rtntid=0; //return'd thread id from pthread_create
    
    int time_to_run = atoi(argv[1]); //time to run for program is the first arg
    printf("Time To Live (seconds)       : %4d\n", time_to_run);
    if(argc > 1){ //if the user entered the number of Diners, change numDiners
        numDiners = atoi(argv[2]);
    }
    printf("Number of Dining Philosophers: %4d\n",numDiners);
    printf("------------------------------------------------\n");
    //allocate space
    chopstick = (semaphore_t*)malloc(sizeof(semaphore_t)*numDiners);
    philosophers = (phil*)malloc(sizeof(phil)*numDiners); //for storing numAte/Thought
    
    pthread_t threads[numDiners];
    state = (int*)malloc(sizeof(int)*numDiners);
    semaphore_create(&mutex, 1); //initialize mutex semaphore to 1
    int i = 0;
    /*initialize sates to THINKING and chopsticks[i] to 0*/
    for( i =0; i < numDiners; i++) {
        philosophers[i].numAte = 0;
        philosophers[i].numThought = 0;
        state[i] = 0;
        semaphore_create(&chopstick[i], 1);//initialize chopstick semaphores to 0
    }
    
    /* create threads for each diner*/
    srandom(time(NULL));
    for(i = 0; i < numDiners; i++) {
        rtntid = pthread_create(&threads[i], NULL, philosopher, (void *)(intptr_t)i);
        if(rtntid != 0) {
            printf("ERROR: return code from pthread_create is %d\n", rtntid);
        }
    }
    sleep(time_to_run); //main prog sleeps for designated time in seconds
    stillEating = 1; //set stillEating to FALSE

    /*join threads for each diner*/
  
    for(i = 0; i < numDiners; i++) {
        pthread_join(threads[i], NULL);
        semaphore_destroy(&chopstick[i]);
    }
    semaphore_destroy(&mutex);
      /*print the number of times eaten and thought
       * in a seperate loop to allow threads to join and update the data in philosophers
       */
    printf("------------------------------------------------\n");
    for(i = 0; i < numDiners; i++) {
        printf("Philosopher %4d: Ate %4d / Thought %d\n",i,philosophers[i].numAte, philosophers[i].numThought);
    }
    printf("------------------------------------------------\n");

    
    pthread_exit(NULL);
    if(philosophers != NULL) {
        free(philosophers);
        philosophers = NULL;
    }
    if(state != NULL) {
        free(state);
        state = NULL;
    }
    return 0;
}

void *philosopher(void* threadId) {
    int tid = (intptr_t)threadId;
    while(stillEating == TRUE) {
        
        printf("Philosopher %3d: Think!\n", tid); //print that the philosopher has started to think
        philosophers[tid].numThought++;
        usleep(random()%TIME_TO_SLEEP);//think()
        
        take_chopsticks(tid); //critical section
        philosophers[tid].numAte++;
        usleep(random()%TIME_TO_SLEEP);//eat()
        
        release_chopsticks(tid); //may need to implement deadlock free version here too
        //release prints out DONE!
    }
    pthread_exit(NULL);
}

void take_chopsticks(int i){
    //wait mutex
    semaphore_wait(&mutex); //wait mutex
    state[i] = HUNGRY;
    test(i); //state changed to hungry and will print Eat! here
    semaphore_post(&mutex);//signal mutex
    semaphore_wait(&chopstick[i]);//wait on i'th chopstick
}

void release_chopsticks(int i) {
    semaphore_wait(&mutex); //wait mutex
    state[i] = THINKING;

    printf("Philosopher %3d: ...... .... Done!\n", i);
    test(( (i + numDiners - 1) % numDiners)); //test left
    test(( (i + 1) % numDiners)); //test right
    semaphore_post(&mutex);//signal mutex
}

void test(int i) {
    //if this phil is hungry and the phils to the left and right are not eating...
    //this phil should start eating/change to eating state
    if(state[i] == HUNGRY &&
       state[( (i + numDiners - 1) % numDiners)] != EATING &&//left
       state[( (i + 1) % numDiners)] != EATING) { //right
        state[i] = EATING;

        printf("Philosopher %3d: ...... Eat!\n", i); //EATING
        semaphore_post(&chopstick[i]); //signal i'th chopstick
        //signal i'th chopstick
    }
}
