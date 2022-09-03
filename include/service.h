#pragma once
#include "Msg.h"
#include <queue>

class service
{
public:
    pthread_spinlock_t queueLock;
    queue<shared_ptr<basemsg>> msg_queue;
    service(/* args */);
    ~service();
};

