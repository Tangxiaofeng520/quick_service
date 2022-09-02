#pragma once
#include <queue>
#include <memory>
#include "service.h"
#include <pthread.h>
#include <unistd.h>
#include "lock.h"
#include "worker.h"


class worker;

class qs
{
    public:
    //单例
    static qs* inst;
public:
    void start();
    //全局队列操作
    shared_ptr<service> pop_global_msg_queue();
    void push_global_msg_queue(shared_ptr<service> srv);
    pthread_spinlock_t globalLock;
    void start_workers();
private:
    queue<shared_ptr<service>> global_msg_queue;  //全局队列

    int WORKER_NUM = 3;
    vector<worker*> workers;
    vector<thread*> threads;
    vector<
};


