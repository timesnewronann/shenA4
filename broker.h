// have parameters for producers and consumers and a shared buffer for the
// broker queue
#ifndef BROKER_H
#define BROKER_H

#include <queue>
#include <semaphore.h>

#include "cryptoexchange.h"
#include "shareddata.h"
using namespace std;

class Broker
{
public:
       // request Queue
    queue<RequestType *> requestQueue;
};

#endif