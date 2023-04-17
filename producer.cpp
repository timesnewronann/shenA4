

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
    //initialize shared data structure
    SHARED_DATA *sharedData = (SHARED_DATA *)argument;

    //default sleep time 
    int sleepTime = 0;

    // declare the type of coin being passed through
    int coinType; 

    // declare the requested type of coin
    RequestType requestedType;


    //CHECK THE COIN TYPE DEPENDING ON THE THREAD
    // check if the coin is a bitcoin
    if(sharedData->isBitCoin){
        requestedType = Bitcoin; // set the requestedType to bitcoin
        coinType = bitcoinSignature;
    }
    else{ // if not bticoin the type is ethereum
        requestedType = Ethereum;
        coinType = ethereumSignature;
    }


    
    // producing a new coin
    while (true) // true is the constant is 1
    {
        // if the coin is a bitcoin
        if(sharedData->isBitCoin){
            sleepTime = sharedData->bitProducingTime; // set the sleepTime to the bitcoin producing time 

            // need to check the amount of bitcoin produced
            sem_wait(&sharedData->bitCoinsInBuffer);

        }
        else // ethereum sleep time if is not bitcoin
        {
            sleepTime = sharedData->ethProductingTime; // set the sleepTime to the ethereum producing time 
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

        // add the type to the request queue through pushing into the queue
        sharedData->buffer.push(&requestedType);

        // produced and inRequestQueue reflect numbers *after* adding the current request. (For logging purposes)
        sharedData->coinsProduced[coinType]++;
        sharedData->coinsInRequestQueue[coinType]++;
        log_request_added(requestedType,sharedData->coinsProduced,sharedData->coinsInRequestQueue);

        // release the lock
        sem_post(&sharedData->mutex);

        /*
        * ... EXITING CRITICAL SECTION
        */

        // signal the consumer semaphore to alert there is a new request available (increment the number of items in the queue)
        sem_post(&sharedData->unconsumed);

        //check if the sum of bit and eth equals the total number of requests --> signal
        //when request production is complete
        // if(sharedData->coinsProduced[bitcoinSignature] + sharedData->coinsProduced[ethereumSignature] == sharedData->numRequests){
        //     sem_post(&sharedData->precedence);
        // }

        // Break out of the loop when the total number of requests have been produced
        if(sharedData->coinsProduced[bitcoinSignature] + sharedData->coinsProduced[ethereumSignature] == sharedData->numRequests){
            break;
        }

        //call logging 
    }
}
