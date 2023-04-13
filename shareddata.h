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

        queue<RequestType> buffer;
};

#endif //CS480A4_SHAREDDATA_H