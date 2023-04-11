#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>


#include "log.h"
#include "consumer.h"
#include "producer.h"

using namespace std;

int main()
{
    int option; // will be used for command line switch 


    // used to past in the arguments
    while (( option = getopt()))


    // on start create two producers and two consumers
    // Semaphore init -> don't use mutex or you're fucked 
  

    //producer threads
    pthread_t producerThreadBitcoin;
    pthread_t producerThreadEtherum;

    pthread_t consumerThreadBitcoin;
    pthread_t consumerThreadBitcoin;



    


}