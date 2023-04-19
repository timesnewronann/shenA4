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
#include "producerdata.h"
#include "consumerdata.h"

#define DEFAULT_NO_DELAY 0
#define BADFLAG 0
#define BITCOIN_CAPACITY 5
#define BUFFERSIZE 16
#define ETHEREUM_CAPACITY 16
#define DEFAULT_NUM_REQUESTS 100

using namespace std;

int main(int argc, char **argv)
{
    /*
    * Initialize data structures holding relevant semaphores, variables, and types specific to
    * the desired production and consumption
    */
    
    SHARED_DATA sharedData;
    // initialize buffer queue 
    sharedData.buffer = queue<RequestType*>();

     // Set the default values for the arguments
    sharedData.numRequests = DEFAULT_NUM_REQUESTS;

    /*
    * For logging purposes:
    * initialize coinsProduced and coinsInRequestQueue arrays in order to monitor the
    * number of bitcoin and ethereum produced separately
    */
    sharedData.coinsProduced[0] = 0;
    sharedData.coinsProduced[1] = 0;

    sharedData.coinsInRequestQueue[0] = 0;
    sharedData.coinsInRequestQueue[1] = 0;

    /*
    * For logging purposes:
    * initialize the coinsConsumed array to monitor the bitcoin and ethereum produced 
    * in each blockchain. 
    */
    sharedData.coinsConsumed[0][0] = 0;
    sharedData.coinsConsumed[0][1] = 0;
    sharedData.coinsConsumed[1][0] = 0;
    sharedData.coinsConsumed[1][1] = 0;

    PRODUCER_DATA bitcoinProducerData;
    bitcoinProducerData.sharedData = &sharedData;
    bitcoinProducerData.request = Bitcoin; //passing in enum for bitcoin
    bitcoinProducerData.producingTime = DEFAULT_NO_DELAY;

    PRODUCER_DATA ethereumProducerData;
    ethereumProducerData.sharedData = &sharedData;
    ethereumProducerData.request = Ethereum; //passing in enum for ethereum
    ethereumProducerData.producingTime = DEFAULT_NO_DELAY;
    
    CONSUMER_DATA xData;
    xData.sharedData = &sharedData;
    xData.consumer = BlockchainX;
    xData.consumingTime = DEFAULT_NO_DELAY;
    
    
    CONSUMER_DATA yData;
    yData.sharedData = &sharedData;
    yData.consumer = BlockchainY; 
    yData.consumingTime= DEFAULT_NO_DELAY; 
    
    
    
    
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
            sharedData.numRequests = numParse;
            break;
        // blockChain X consuming time
        case 'x':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            xData.consumingTime = numParse;
            break;

        // blockChain Y consuming time
        case 'y':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            yData.consumingTime = numParse;
            break;
        // bitCoin production time
        case 'b':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            bitcoinProducerData.producingTime = numParse;
            break;

        // etherum production time
        case 'e':
            numParse = atoi(optarg);
            if (optarg == "")
            {
                numParse = DEFAULT_NO_DELAY;
            }
            ethereumProducerData.producingTime = numParse;
            break;
        // Case that is not specified in the list of optional arguments.
        default:
            exit(BADFLAG);
        }
    }

    /*
     * Initialization of semaphores
     */

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


    if(pthread_create(&producerThreadBitcoin, NULL, &producer, &bitcoinProducerData) != 0){
        return BADFLAG;
    }
    

    if(pthread_create(&producerThreadEtherum, NULL, &producer, &ethereumProducerData) != 0){
        return BADFLAG;
    }

    
    if(pthread_create(&consumerBlockX, NULL, &consumer, &xData) != 0){
        return BADFLAG;
    }

    if(pthread_create(&consumerBlockY, NULL, &consumer, &yData) != 0)
    {
        return BADFLAG;
    }


    // we do not know before hand which consumer will consume the last request
    // if we do not use the semaphore to control the order of execution
    // will have interleaving execution



    // barrier: wait for consumer to consume last item
    sem_wait(&sharedData.precedence);

    // convert the consumed array to hold the values 
    unsigned int *coinsConsumed[] = {sharedData.coinsConsumed[BlockchainX], sharedData.coinsConsumed[BlockchainY]};


    log_production_history(sharedData.coinsProduced, coinsConsumed);

    // Comment out for now
    // once finished
    sem_destroy(&sharedData.mutex);
    sem_destroy(&sharedData.availableSlots);
    sem_destroy(&sharedData.unconsumed);
    sem_destroy(&sharedData.precedence);

}