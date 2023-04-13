

// in producer thread can look at the shared data strucure for what we need to pass in for each individual thread
#include <iostream>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "cryptoexchange.h"
#include "shareddata.h"

using namespace std;

void *producer(void *argument)
{
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
        sem_wait(&sharedData.availableSlots)
            // need available slots from shareddata

            // wait for the appropriate semaphore

            // lock the queue
            sem_wait(sharedData.);

        // add the type to the request queue
        sharedData.buffer.push(requestedType);
        // release the lock
        sem_post();

        // signal the consumer semaphore to alert there is a new request available
        sem_post();

        // sleep
        usleep();
    }
}
