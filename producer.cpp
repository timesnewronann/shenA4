

// in producer thread can look at the shared data strucure for what we need to pass in for each individual thread
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "cryptoexchange.h"
#include "shareddata.h"
#include "log.h"

#define bitcoinSignature 0
#define ethereumSignature 1

using namespace std;

void *producer(void *argument)
{
    SHARED_DATA *sharedData = (SHARED_DATA *)argument;
    int sleepTime = 0;
    // create the item type depending on the argument we pass
    // RequestType requestedType = *((RequestType *)argument);
    
    

    

    int coinType; 

    RequestType requestedType;
    if(sharedData->isBitCoin){
        requestedType = Bitcoin;
        coinType = bitcoinSignature;
    }
    else{
        requestedType = Ethereum;
        coinType = ethereumSignature;
    }

    

    // do production first
    while (true) // true is the constant is 1
    {

        usleep(sleepTime);

        if(sharedData->isBitCoin){
            sleepTime = sharedData->bitProducingTime;
        }
        else // ethereum sleep time if is not bitcoin
        {
            sleepTime = sharedData->ethProductingTime;
        }

        // produce the item to place into the buffer
        usleep(sleepTime);
        
        
        // make sure we have room on the buffer
        sem_wait(&sharedData->availableSlots);

        /*
        * ENTERING CRITICAL SECTION...
        */

        // lock the queue
        sem_wait(&sharedData->mutex);

        // add the type to the request queue
        sharedData->buffer.push(&requestedType);

        // produced and inRequestQueue reflect numbers *after* adding the current request.
        sharedData->coinsProduced[coinType]++;
        sharedData->coinsInRequestQueue[coinType]++;
        log_request_added(requestedType,sharedData->coinsProduced,sharedData->coinsInRequestQueue);

        // release the lock
        sem_post(&sharedData->mutex);

        /*
        * ... EXITING CRITICAL SECTION
        */

        // signal the consumer semaphore to alert there is a new request available
        sem_post(&sharedData->unconsumed);

        //check if the sum of bit and eth equals the total number of requests --> signal
        //when request production is complete
        if(sharedData->coinsProduced[bitcoinSignature] + sharedData->coinsProduced[ethereumSignature] == sharedData->numRequests){
            sem_post(&sharedData->precedence);
        }

        //call logging 
    }
}
