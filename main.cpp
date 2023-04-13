#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <semaphore.h>

#include "log.h"
#include "consumer.h"
#include "producer.h"
#include "cryptoexchange.h"
#include "shareddata.h"

#define DEFAULT_NO_DELAY 0
#define BADFLAG 0
#define BUFFERSIZE 16
#define DEFAULT_NUM_REQUESTS 100

using namespace std;

int main(int argc, char **argv)
{
    // consider storing the initialization of these variables in a separate class if they are shared
    int numRequests;
    int xConsumingTime;
    int yConsumingTime;
    int bitProducingTime;
    int ethProductingTime;

    int option; // will be used for command line switch
    int numParse;

    // used to past in the arguments
    while ((option = getopt(argc, argv, "r:x:y:b:e:")) != -1)
    {
        switch (option)
        {
        // Total number of trade requests
        case 'r':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NUM_REQUESTS;
            }
            numRequests = numParse;
            break;
        // blockChain X consuming time
        case 'x':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            xConsumingTime = numParse;
            break;

        // blockChain Y consuming time
        case 'y':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            yConsumingTime = numParse;
            break;
        // bitCoin consuming time
        case 'b':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            bitProducingTime = numParse;
            break;

        // etherum consuming time
        case 'e':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            ethProductingTime = numParse;
            break;
        // Case that is not specified in the list of optional arguments.
        default:
            exit(BADFLAG);
        }
    }

    SHARED_DATA sharedData;

    // on start create two producers and two consumers
    // Semaphore init -> don't use mutex or you're fucked

    // producer threads

    /*
     * Initialization of semaphores
     */

    // sem_t mutex;          // for the mutex lock of the broker queue (critical section)
    // sem_t availableSlots; // space in the buffer
    // sem_t unconsumed;     // items in the buffer
    sem_t precedence; // main waiting for the last item to be consumed before exiting

    sem_init(&sharedData.mutex, 0, 1);
    sem_init(&sharedData.availableSlots, 0, BUFFERSIZE);
    sem_init(&sharedData.unconsumed, 0, 0);
    sem_init(&precedence, 0, 0);

    /*
     * Create all producer and consumer threads at the same time.
     */

    pthread_t producerThreadBitcoin;
    // pthread_t producerThreadEtherum;

    pthread_t consumerBlockX;
    // pthread_t consumerBlockY;

    if(pthread_create(&producerThreadBitcoin, NULL, &producer, &sharedData) != 0){
        return BADFLAG;
    }
    if(pthread_create(&consumerBlockX, NULL, &consumer, &sharedData) != 0){
        return BADFLAG;
    }

    // wait for consumer to consume last item
    // use barrier

    // we do not know before hand which consumer will consume the last request

    // if we do not use the semaphore to control the order of execution

    // will have interleaving execution
    // while loop for producer/consumer we don't control the switching

    // once finished
    sem_destroy(&sharedData.mutex);
    sem_destroy(&sharedData.availableSlots);
    sem_destroy(&sharedData.unconsumed);
    sem_destroy(&precedence);
}