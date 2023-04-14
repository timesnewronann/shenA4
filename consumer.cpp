// can pass in an indicator to create a consumer
// synchronize to broker before we can consume
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "cryptoexchange.h"
#include "shareddata.h"

using namespace std;

void consumer(void *argument)
{
    int sleepTime = 0;
    // item type bitcoin;
    // item type consumer;
    // item type ethereum;
    // create blockchain x first
    // create all of them at the same time

    SHARED_DATA *sharedData = (SHARED_DATA *)argument;
    RequestType *requestedType;

    while (true)
    {
        // wait for the unconsumed semaphore
        sem_wait(&sharedData->unconsumed);

        /*
        * Accessing critical section
        */

        // lock the queue
        sem_wait(&sharedData->mutex);

        // get the item from the queue
        requestedType = sharedData->buffer.front();
        sharedData->buffer.pop(); // pop off the queue

        // unlock
        sem_post(&sharedData->mutex);

        // alert there is now a spot in the buffer
        sem_post(&sharedData->availableSlots);

        // process the request

        // simulate the consumption with sleep 
        usleep(sleepTime);
        
    }

    if(!sharedData->buffer.size() && sharedData->currentProductionNumber == sharedData->numRequests){
        sem_post(&sharedData->precedence);
    }
}