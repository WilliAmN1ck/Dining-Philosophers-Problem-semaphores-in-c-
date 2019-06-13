/*
 * Samantha Foley
 *
 * Project 4: Dining Philosophers (Common header)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "semaphore_support.h"


/*****************************
 * Defines
 *****************************/
#define TRUE  0
#define FALSE 1

/* 1.0 second = 1000000 usec */
//#define TIME_TO_SLEEP 100000
/* 0.5 second = 500000 usec */
//#define TIME_TO_SLEEP 50000
#define TIME_TO_SLEEP 5000

#define CLOCKS_PER_SECOND 1000000
#define THINKING   0
#define HUNGRY     1
#define EATING     2


/*****************************
 * Structures
 *****************************/
struct phil {
    int numAte;
    int numThought;
    //int done; //0= false, 1 = true -- probably do not need
};
typedef struct phil phil;

/*****************************
 * Global Variables
 *****************************/

semaphore_t mutex; //semaphore to protect access to taking and releasing a certain chopstick
semaphore_t *chopstick; //semaphore to protect each individual chopstick
int *state; //pointer to an array of the state of each philosopher
int stillEating; //global var for threads to check if they are done
phil* philosophers; //pointer to the array of philosophers struct
int numDiners; //the number of philosophers dining

/*****************************
 * Function Declarations
 *****************************/
/*
 * function for threads to make a new dining philosopher
 */
void* philosopher(void* threadId);

/*
 * function for threads to take a chopstick if it can
 */
void take_chopsticks(int i);
/*
 * function for threads to release chopsticks
 */
void release_chopsticks(int i);

/*
 * function to test each thread if a philosopher should switch to the EATING stage
 */
void test(int i);
