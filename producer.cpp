

// in producer thread can look at the shared data strucure for what we need to pass in for each individual thread
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "cryptoexchange.h"
#include "shareddata.h"
#include "log.h"
#include "producerdata.h"

#define bitcoinSignature 0
#define ethereumSignature 1

using namespace std;

void *producer(void *argument)
{
    PRODUCER_DATA *producerData = (PRODUCER_DATA *)argument;
    
    // do production first
    while (true){ // true is the constant is 1
    // don't need to use isBitcoin can use the request type from sharedData

     // sleep at the beginning
     // produce the item to place into the buffer
      usleep(producerData->producingTime);

    
        if(producerData->request == Bitcoin){
            cout << "request is currently bitcoin" << endl;
            //sleepTime = bitcoin->bitProducingTime;

            // need to check the amount of bitcoin produced
            sem_wait(&producerData->sharedData->bitCoinsInBuffer);

        }
       
       
      
        
        // make sure we have room on the buffer
        sem_wait(&producerData->sharedData->availableSlots);

        /*
        * ENTERING CRITICAL SECTION...
        */

        // lock the queue
        sem_wait(&producerData->sharedData->mutex);

        // add the type to the request queue
        producerData->sharedData->buffer.push(&producerData->request);

        // produced and inRequestQueue reflect numbers *after* adding the current request. (For logging purposes)
        producerData->sharedData->coinsProduced[producerData->request]++;
        producerData->sharedData->coinsInRequestQueue[producerData->request]++;

        log_request_added(producerData->request,
        producerData->sharedData->coinsProduced,
        producerData->sharedData->coinsInRequestQueue);

        // release the lock
        sem_post(&producerData->sharedData->mutex);

        /*
        * ... EXITING CRITICAL SECTION
        */

        // signal the consumer semaphore to alert there is a new request available
        sem_post(&producerData->sharedData->unconsumed);

        // Break out of the loop when the total number of requests have been produced
        if(producerData->sharedData->coinsProduced[bitcoinSignature] + producerData->sharedData->coinsProduced[ethereumSignature] == producerData->sharedData->numRequests){
            //wait for consumer to finish before ending producer thread
            sem_wait(&producerData->sharedData->precedence);
            break;
        }

    }
}
