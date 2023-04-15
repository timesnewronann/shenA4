

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
    sharedData->currentProductionNumber = 0;
    int sleepTime = 0;
    // create the item type depending on the argument we pass
    // RequestType requestedType = *((RequestType *)argument);
    
    RequestType *requestedType;

    int coins[2];
    int coinType; 

    if(sharedData->isBitCoin){
        requestedType = new RequestType(Bitcoin);
        coinType = bitcoinSignature;
    }
    else{
        requestedType = new RequestType(Ethereum);
        coinType = ethereumSignature;
    }

    

    // do production first
    while (true) // true is the constant is 1
    {
   
        coins[coinType]++;

        usleep(sleepTime);

        if(sharedData->isBitCoin){
            sleepTime = sharedData->bitProducingTime;
        }
        else // ethereum sleep time if is not bitcoin
        {
            sleepTime = sharedData->ethProductingTime;
        }
        // sleep outside the critical region to access the broker queue
        usleep(sleepTime);
        
        sharedData->currentProductionNumber++;

        // produce the item to place into the buffer

        
        // check if we reached the max production
        // make sure we have room on the buffer
        sem_wait(&sharedData->availableSlots);

        /*
        * ENTERING CRITICAL SECTION...
        */

        // lock the queue
        sem_wait(&sharedData->mutex);

        // add the type to the request queue
        sharedData->buffer.push(requestedType);
        // release the lock
        sem_post(&sharedData->mutex);

        /*
        * ... EXITING CRITICAL SECTION
        */

        // signal the consumer semaphore to alert there is a new request available
        sem_post(&sharedData->unconsumed);

        //check if the sum of bit and eth equals the total number of requests --> signal
        //when request production is complete
        if(coins[bitcoinSignature] + coins[ethereumSignature] == sharedData->numRequests){
            sem_post(&sharedData->precedence);
        }

        //call logging 
    }
}
