#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>


#include "log.h"
#include "consumer.h"
#include "producer.h"

#define DEFAULT_NO_DELAY 0 
#define BADFLAG 0 
#define DEFAULT_NUM_REQUESTS 100


using namespace std;

int main(int argc, char** argv)
{
    //consider storing the initialization of these variables in a separate class if they are shared 
    int numRequests;
    int xConsumingTime;
    int yConsumingTime;
    int bitProducingTime;
    int ethProductingTime;

     /*
    * variable for switching through the command line in the getopt function
    * points to the argument that is referred by a specific case
    */
    int option;  
    //variable for parsing the optional arguments into integer values 
    int numParse;  

    while ( (option = getopt(argc, argv, "r:x:y:b:e:")) != -1) {
        switch (option) {
            //Total number of trade requests
            case 'r': 
                numParse = atoi(optarg);
                if(optarg == ""){
                    numParse = DEFAULT_NUM_REQUESTS;
                }
                numRequests = numParse;
                break;
            case 'x':
                numParse = atoi(optarg);
                if(optarg == ""){
                    numParse = DEFAULT_NO_DELAY;
                }
                xConsumingTime = numParse;
                break;

            case 'y':
                numParse = atoi(optarg);
                if(optarg == ""){
                    numParse = DEFAULT_NO_DELAY;
                }
                yConsumingTime = numParse;
                break;
            case 'b':
                numParse = atoi(optarg);
                if(optarg == ""){
                    numParse = DEFAULT_NO_DELAY;
                }
                bitProducingTime = numParse;
                break;
            case 'e':
                numParse = atoi(optarg);
                if(optarg == ""){
                    numParse = DEFAULT_NO_DELAY;
                }
                ethProductingTime = numParse;
                break;
            //Case that is not specified in the list of optional arguments. 
            default:
                exit(BADFLAG); 
        }
    }




    // on start create two producers and two consumers
    // Semaphore init -> don't use mutex or you're fucked 
  

    //producer threads:
    /*
    * Both producers immediately simulate production using a sleep() call
    * As soon as the sleep() call is done for a producer, they should try and add it to the queue
    * If there’s space on the queue, they should start producing on another item.
    */

    /*
    * You are recommended to write this project in stages. First, make a single producer and consumer function on a
    * generic request type function. Then, add multiple producers and consumers. Finally, introduce the multiple
    * types of requests.
    */

    pthread_t producerThreadBitcoin;
    pthread_t producerThreadEtherum;

    pthread_t consumerThreadBitcoin;
    pthread_t consumerThreadBitcoin;



    


}