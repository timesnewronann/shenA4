

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
#define microTonano 1000

using namespace std;

void *producer(void *argument)
{
    PRODUCER_DATA *producerData = (PRODUCER_DATA *)argument;
    
    // cout << "TYPE: " << producerData->request << endl;
    // cout << "BITPRODUCETIME: " << producerData->producingTime  << endl;
    
    
    // do production first
    while (true){ // true is the constant is 1
    


     // sleep at the beginning
     // produce the item before accessing critical section
      usleep(microTonano * producerData->producingTime);

    
        if(producerData->request == Bitcoin){

            // need to check the amount of bitcoin produced: no more than 5 bitcoins in buffer at a time
            sem_wait(&producerData->sharedData->bitCoinsInBuffer);

        }
        if(producerData->request == Ethereum){
            sem_wait(&producerData->sharedData->ethereumInBuffer);
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

        cout << "bitcoin sig " << producerData->sharedData->coinsProduced[bitcoinSignature] << endl;
        cout << "etherum sig " << producerData->sharedData->coinsProduced[ethereumSignature] << endl;
        cout << "number of requests" << producerData->sharedData->numRequests << endl;
        // Break out of the loop when the total number of requests have been produced
        if(producerData->sharedData->coinsProduced[bitcoinSignature] + producerData->sharedData->coinsProduced[ethereumSignature] == producerData->sharedData->numRequests){
            //wait for consumer to finish before ending producer thread
            cout << "TOTAL REQUESTS REACHED ******************" << endl;
            break;
            /*
            * TRY:
            *pthread_exit(argument);
            */
        }
    }
}
