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
        sem_t etheriumInBuffer; // there can be no more tahn 16 ethereum requests in the buffer at one time.
        sem_t precedence; // for ensuring completion of last item to be consumed before exiting main thread.  
        queue<RequestType*> buffer; //the broker queue
        

        /*
        * Option Arguments
        */
        int numRequests;
        int xConsumingTime;
        int yConsumingTime;
        int bitProducingTime;
        int ethProductingTime;


        int currentProductionNumber; //keep track of the number of processed requests <= numRequests
        bool isBitCoin;
        bool isBlockX;


};

#endif //CS480A4_SHAREDDATA_H