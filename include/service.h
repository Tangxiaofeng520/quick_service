#pragma once
#include "Msg.h"
#include <queue>

class service
{
public:
    queue<shared_ptr<basemsg>> msg_queue;
    service(/* args */);
    ~service();
};

