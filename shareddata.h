#ifndef CS480A4_SHAREDDATA_H
#define CS480A4_SHAREDDATA_H

#include <queue>
#include <semaphore.h>
#include "cryptoexchange.h"

using namespace std;
class SHARED_DATA{
    public: 
        sem_t mutex;          // for the mutex lock of the broker queue (critical section)
        sem_t availableSlots; // space in the buffer
        sem_t unconsumed;     // items in the buffer

        sem_t bitCoinsInBuffer; //there can be no more than 5 bitcoin requests in the buffer at one time.
        sem_t ethereumInBuffer; // there can be no more than 16 ethereum requests in the buffer at one time.
        sem_t precedence; // for ensuring completion of last item to be consumed before exiting main thread.  
    
        queue<RequestType*> buffer; //the broker queue
        

        /*
        * Option Arguments
        */
        int numRequests;

        //moved to consumer
        //int xConsumingTime;
        //int yConsumingTime;

        //moved to producer_data
        // int bitProducingTime;
        // int ethProductingTime;

        //bool isBitCoin; //variable to declare bitcoin or ethereum production for the respective thread. 
        //bool isBlockX;  //variable to declare blockchain x or blockychain y for the respective thread. 

        unsigned int coinsProduced[2]; //array to monitor the number of bitcoin and etherum produced separately 
        unsigned int coinsInRequestQueue[2]; //array to monitor the influx/outflux of bitcoin and ethereum in the request queue
        unsigned int coinsConsumed[2][2]; //array to monitor bitcoin and ethereum produced from blockchain x and blockchain y

        //unsigned int *coinsConsumed[2][2];

};

#endif //CS480A4_SHAREDDATA_H