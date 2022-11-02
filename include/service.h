#pragma once
#include "Msg.h"
#include <queue>

class service
{
public:

    //唯一id
    uint32_t id;
    bool inGlobal;
    pthread_spinlock_t queueLock; //自旋锁(直到其他锁释放）
    queue<shared_ptr<basemsg>> msg_queue;
    service(/* args */);
    ~service();

    void pushMsg(shared_ptr<basemsg> msg);
    shared_ptr<basemsg> popMsg();

    void set_in_global(bool flag);

    bool in_global();

    void on_msg(shared_ptr<basemsg> msg);

    void on_accept_msg(shared_ptr<basemsg> msg);

    void on_service_msg(shared_ptr<basemsg> msg);

    void on_rw_msg(shared_ptr<basemsg> msg);

    void process_msg();
};

