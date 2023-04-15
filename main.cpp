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
#define BITCOIN_CAPACITY 5
#define BUFFERSIZE 16
#define ETHEREUM_CAPACITY 16
#define DEFAULT_NUM_REQUESTS 100

using namespace std;

int main(int argc, char **argv)
{
    // consider storing the initialization of these variables in a separate class if they are shared
    // int numRequests;
    // int xConsumingTime;
    // int yConsumingTime;
    // int bitProducingTime;
    // int ethProductingTime;

    int option; // will be used for command line switch
    int numParse;
    SHARED_DATA sharedData;

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
            sharedData.numRequests = numParse;
            break;
        // blockChain X consuming time
        case 'x':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            sharedData.xConsumingTime = numParse;
            break;

        // blockChain Y consuming time
        case 'y':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            sharedData.yConsumingTime = numParse;
            break;
        // bitCoin consuming time
        case 'b':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            sharedData.bitProducingTime = numParse;
            break;

        // etherum consuming time
        case 'e':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            sharedData.ethProductingTime = numParse;
            break;
        // Case that is not specified in the list of optional arguments.
        default:
            exit(BADFLAG);
        }
    }


    //initialize coinsProduced and coinsInRequestQueue arrays for logging purposes
    sharedData.coinsProduced[0] = 0;
    sharedData.coinsProduced[1] = 0;

    sharedData.coinsInRequestQueue[0] = 0;
    sharedData.coinsInRequestQueue[1] = 0;

    //initialize coinsConsumed array for logging purposes
    sharedData.coinsConsumed[0] = 0;
    sharedData.coinsConsumed[1] = 0;

    

    // on start create two producers and two consumers
    // Semaphore init -> don't use mutex or you're fucked

    // producer threads

    /*
     * Initialization of semaphores
     */

    // sem_t mutex;          // for the mutex lock of the broker queue (critical section)
    // sem_t availableSlots; // space in the buffer
    // sem_t unconsumed;     // items in the buffer
    //sem_t precedence; // main waiting for the last item to be consumed before exiting

    if(sem_init(&sharedData.mutex, 0, 1) == -1){
        cerr << "mutex semaphore failed" << endl;
    }
    if(sem_init(&sharedData.availableSlots, 0, BUFFERSIZE) == -1){
        cerr << "avaiableSlots semaphore failed" << endl;
    }
    if(sem_init(&sharedData.unconsumed, 0, 0) == -1){
        cerr << "unconsumed semaphore failed" << endl;
    }
    if(sem_init(&sharedData.precedence, 0, 0) == -1){
        cerr << "precedence sempahore failed" << endl;
    }
    if(sem_init(&sharedData.bitCoinsInBuffer, 0, BITCOIN_CAPACITY) == -1){
        cerr << "bitCoinsInBuffer semaphore failed" << endl;
    }
    if(sem_init(&sharedData.ethereumInBuffer, 0, ETHEREUM_CAPACITY) == -1){
        cerr << "ethereumInBuffer semaphore failed" << endl;
    }

    /*
     * Create all producer and consumer threads at the same time.
     */

    pthread_t producerThreadBitcoin;
    pthread_t producerThreadEtherum;

    pthread_t consumerBlockX;
    pthread_t consumerBlockY;

    sharedData.isBitCoin = true;

    if(pthread_create(&producerThreadBitcoin, NULL, &producer, &sharedData) != 0){
        return BADFLAG;
    }

    sharedData.isBitCoin = false;

    if(pthread_create(&producerThreadEtherum, NULL, &producer, &sharedData) != 0)
    {
        return BADFLAG;
    }

    sharedData.isBlockX = true;
    if(pthread_create(&consumerBlockX, NULL, &consumer, &sharedData) != 0){
        return BADFLAG;
    }

    sharedData.isBlockX = false;
    if(pthread_create(&consumerBlockY, NULL, &consumer, &sharedData) != 0)
    {
        return BADFLAG;
    }

    // wait for consumer to consume last item
    // use barrier

    // we do not know before hand which consumer will consume the last request

    // if we do not use the semaphore to control the order of execution

    // will have interleaving execution
    // while loop for producer/consumer we don't control the switching

    sem_wait(&sharedData.precedence);
    log_production_history(sharedData.coinsProduced, sharedData.coinsConsumed);

    // once finished
    sem_destroy(&sharedData.mutex);
    sem_destroy(&sharedData.availableSlots);
    sem_destroy(&sharedData.unconsumed);
    sem_destroy(&sharedData.precedence);
}