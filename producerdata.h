#ifndef CS480A4_PRODUCERDATA_H
#define CS480A4_PRODUCERDATA_H


#include "cryptoexchange.h"
#include "shareddata.h"


using namespace std;
class PRODUCER_DATA{
    public: 
        SHARED_DATA *sharedData;
        RequestType request;
        int producingTime;
        
};

#endif //CS480A4_PRODUCERDATA_H