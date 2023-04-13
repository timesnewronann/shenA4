

// in producer thread can look at the shared data strucure for what we need to pass in for each individual thread
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "cryptoexchange.h"
#include "shareddata.h"

using namespace std;

void *producer(void *argument)
{
    int sleepTime = 0;

    SHARED_DATA sharedData;
    // create the item type depending on the argument we pass
    // RequestType requestedType = *((RequestType *)argument);
    RequestType *requestedType;

    // do production first
    while (true) // true is the constant is 1
    {

        // produce the item to place into the buffer
        requestedType = new RequestType(RequestTypeN);

        // check if we reached the max production
        // make sure we have room on the buffer
        sem_wait(&sharedData.availableSlots);
        // need available slots from shareddata

        // wait for the appropriate semaphore

        // lock the queue
        sem_wait(&sharedData.mutex);

        // add the type to the request queue
        sharedData.buffer.push(requestedType);
        // release the lock
        sem_post(&sharedData.mutex);

        // signal the consumer semaphore to alert there is a new request available
        sem_post(&sharedData.unconsumed);

        // sleep outside the critical region to access the broker queue
        sleepTime = 0; // change the sleepTime later through arguments
        usleep(sleepTime);
    }
}
