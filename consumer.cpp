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

#define microToNano 1000

using namespace std;

// consumer does not need producers to be alive, as long as you have the queue you can run the consumer 


void consumer(void *argument)
{
    CONSUMER_DATA *consumerData = (CONSUMER_DATA*)argument;

    
    RequestType *requestedType; // declare the item type
    //ConsumerType consumer; // delcare the item type 

    // cout << "CONSUMERTYPE: " << consumerData->consumer << "has a consuming time of " << consumerData->consumingTime << endl;
    while (true){
        // wait for the unconsumed semaphore -> block until something is available to consume 
        sem_wait(&(consumerData->sharedData->unconsumed));

        /*
        * ENTERING CRITICAL SECTION ...
        */
        // lock the queue
        sem_wait(&consumerData->sharedData->mutex);

        // get the item from the queue
        requestedType = consumerData->sharedData->buffer.front();
        consumerData->sharedData->buffer.pop(); // pop off the queue


        //increment the amount of coins consumed (For logging purposes)
        consumerData->sharedData->coinsConsumed[consumerData->consumer][*requestedType]++;

        //decrement the amount of coins in the queue because we popped out from the queue (For logging purposes)
        consumerData->sharedData->coinsInRequestQueue[*requestedType]--;


        // check for the pointer *requestedType when testing 
        log_request_removed(consumerData->consumer,*requestedType, consumerData->sharedData->coinsConsumed[consumerData->consumer],consumerData->sharedData->coinsInRequestQueue); 

        // unlock
        sem_post(&consumerData->sharedData->mutex);

        /*
        * ... EXITING CRITICAL SECTION
        */

        

        // alert there is now a spot in the buffer
        sem_post(&consumerData->sharedData->availableSlots);


       //bitcoin was consumed so add one more available space to bitcoinsInBuffer
        if(*requestedType == Bitcoin)
        {
            sem_post(&consumerData->sharedData->bitCoinsInBuffer);
        }
        if(*requestedType == Ethereum){
            sem_post(&consumerData->sharedData->ethereumInBuffer);
        }
    
        
        // simulate the consumption with sleep -> consume or use item
        usleep(microToNano * consumerData->consumingTime);




        // need to break out of the while loop
        if(consumerData->sharedData->coinsProduced[0] + consumerData->sharedData->coinsProduced[1] == consumerData->sharedData->numRequests && consumerData->sharedData->buffer.size() == 0)
        {
            if(!consumerData->sharedData->buffer.size()){
                //sem_post(&consumerData->sharedData->precedence); // offset the producer waiting for consumer to finish
                sem_post(&consumerData->sharedData->precedence); // offset main waiting for consumer to finish
            }
            break;
        }
        
    }

    
}