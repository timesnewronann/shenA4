// can pass in an indicator to create a consumer
// synchronize to broker before we can consume
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "cryptoexchange.h"
#include "shareddata.h"
#include "log.h"
#include "consumerdata.h"
#include "producerdata.h"

using namespace std;

void consumer(void *argument)
{
    
    // item type bitcoin;
    // item type consumer;
    // item type ethereum;
    // create blockchain x first
    // create all of them at the same time

    SHARED_DATA *sharedData = (SHARED_DATA *)argument; 

    PRODUCER_DATA *producerData = (PRODUCER_DATA *)argument;
    
    int sleepTime = 0;
    RequestType *requestedType; // declare the item type
    ConsumerType consumer; // delcare the item type 


    if (sharedData->isBlockX)
    {
        consumer = BlockchainX;
    }
    else
    {
        consumer = BlockchainY;
    }

    while (true){
        // wait for the unconsumed semaphore -> block until something is available to consume 
        sem_wait(&sharedData->unconsumed);

        /*
        * ENTERING CRITICAL SECTION ...
        */
        // lock the queue
        sem_wait(&sharedData->mutex);

        // get the item from the queue
        requestedType = sharedData->buffer.front();
        sharedData->buffer.pop(); // pop off the queue


        //increment the amount of coins consumed (For logging purposes)
        sharedData->coinsConsumed[consumer][*requestedType]++;

        //decrement the amount of coins in the queue because we popped out from the queue (For logging purposes)
        sharedData->coinsInRequestQueue[*requestedType]--;


        // check for the pointer *requestedType when testing 
        log_request_removed(consumer,*requestedType, sharedData->coinsConsumed[consumer],sharedData->coinsInRequestQueue); 

        // unlock
        sem_post(&sharedData->mutex);

        /*
        * ... EXITING CRITICAL SECTION
        */

        

        // alert there is now a spot in the buffer
        sem_post(&sharedData->availableSlots);

        // process the request
        /*if(sharedData->isBitCoin)
        {
            sleepTime = sharedData->xConsumingTime;
        }
        */

       //bitcoin was consumed so add one more available space to bitcoinsInBuffer
        if(*requestedType == Bitcoin)
        {
            sem_post(&sharedData->bitCoinsInBuffer);
        }
        if(*requestedType == Ethereum){
            sem_post(&sharedData->ethereumInBuffer);
        }
       
        if(sharedData->isBlockX)
        {
            sleepTime = sharedData->xConsumingTime;
        }
        else{
            sleepTime = sharedData->yConsumingTime;
        }
        
       

        // simulate the consumption with sleep -> consume or use item
        usleep(sleepTime);




        // need to break out of the while loop
        if(sharedData->coinsProduced[0] + sharedData->coinsProduced[1] == sharedData->numRequests && sharedData->buffer.size() == 0)
        {
            if(!sharedData->buffer.size()){
                sem_post(&sharedData->precedence); // offset the producer waiting for consumer to finish
                sem_post(&sharedData->precedence); // offset main waiting for consumer to finish
            }
            break;
        }
        
    }

    
}