/*
 * Samantha Foley
 *
 * Project 4: Dining Philosophers
 * Deadlocking Algorithm
 *
 */
#include "diners.h"


int main(int argc, char * argv[]) {
    if (argc < 1) {
        printf("ERROR: no arguments were entered!\n");
        exit(-1);
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
        semaphore_create(&chopstick[i], 1);//initialize chopstick semaphores to 1
    }
    
    srandom(time(NULL)); //seed random num generator before starting threads
    /* create threads for each diner*/
    for(i = 0; i < numDiners; i++) {
        rtntid = pthread_create(&threads[i], NULL, philosopher, (void *)(intptr_t)i);
        if(rtntid != 0) {
            printf("ERROR: return code from pthread_create is %d\n", rtntid);
        }
    }
    sleep(time_to_run); //main prog sleeps for designated time in seconds
    stillEating = 1; //set doneEating to false

    /*join threads for each diner*/

    for(i = 0; i < numDiners; i++) {
        pthread_join(threads[i], NULL);
    }
    semaphore_destroy(&mutex);
    printf("------------------------------------------------\n");
    /*print the number of times eaten and thought in seperate loop to allow processes to finish*/
    for(i = 0; i < numDiners; i++) {
        semaphore_destroy(&chopstick[i]);
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
    do {
        
        philosophers[tid].numThought++;
        semaphore_wait(&mutex); //signal mutex to block stdout
        printf("Philosopher %3d: Think\n",tid);
        semaphore_post(&mutex);
        
        usleep((random()%TIME_TO_SLEEP)); //think...

        semaphore_wait(&chopstick[tid]); //wait on i-th chopstick
        semaphore_wait(&chopstick[(tid+1) % numDiners]); //wait on next chopstick
        
        
        semaphore_wait(&mutex);//only allow one thread access to sys.out
        printf("Philosopher %3d: ..... Eat!\n",tid);
        semaphore_post(&mutex);
        
        usleep((random()%TIME_TO_SLEEP));//eat...
        philosophers[tid].numAte++;
        
        semaphore_wait(&mutex);
        printf("Philosopher %3d: ..... ... Done!\n",tid);
        semaphore_post(&mutex);
        
        semaphore_post(&chopstick[tid]); //signal i'th chopstick
        semaphore_post(&chopstick[(tid +1) % numDiners]); //signal next chopstick
        


    } while (stillEating == TRUE);
    
    pthread_exit(NULL);
}
