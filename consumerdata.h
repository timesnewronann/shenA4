#ifndef CS480A4_CONSUMERDATA_H
#define CS480A4_CONSUMERDATA_H

#include <queue>
#include <semaphore.h>
#include "cryptoexchange.h"
#include "shareddata.h"


using namespace std;
class CONSUMER_DATA{
    public: 
        SHARED_DATA *sharedData;
        ConsumerType consume;
        int consumingTime;
        
};

#endif //CS480A4_CONSUMERDATA_H