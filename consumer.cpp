// can pass in an indicator to create a consumer
// synchronize to broker before we can consume 


#include "cryptoexchange.h"


using namespace std;

void consumer(void *argument)
{
    

    // item type bitcoin;
    // item type consumer;
    RequestType item = *((RequestType *) argument);
    // create blockchain x first
    // create all of them at the same time

    /*
    while(true)
    {
        // block until something to consume
        unconsumed.wait(); // down


        //Access buffer exclusively
        mutex.wait(); // down
        Item = buffer.remove();
        mutex.signal(); // up

        AvailableSlots.signal(); // up
        consume(item); //consume or use item
    }
    */
}