// queue is fifo
// process the trade requests in order
// unconsumed probably don't want to use unconsumed for the semaphores
// if one consumer is faster that consumer will consume more (part of the tests)
// we should see more blockchain x in the print out

// need to track what's happening in the queue at a particular moment
// track over the time and history of our whole execution how many requests were produced and consumed

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

int buffer[16];