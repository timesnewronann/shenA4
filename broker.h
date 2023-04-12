#include <pthread.h>
#include <queue>
class Broker{
public:
    pthread_mutex_t queue_mutex;
    
};