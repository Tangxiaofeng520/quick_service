#pragma once
#include <queue>
#include <memory>
#include "service.h"
#include <pthread.h>
#include "thread"
#include <unistd.h>
#include "worker.h"
#include "service_mgr.h"
//class Worker;
#include "socketworker.h"
class qs
{
    public:
    //单例
    static qs* inst;
    service_mgr* serviceMgr{};
    socketworker* socketWorker;

    void close_socketworkers();

public:
    void init();
    void start();
    //构造函数
    qs();
    //全局队列操作
    shared_ptr<service> pop_global_msg_queue();
    shared_ptr<service> push_global_msg_queue(shared_ptr<service> srv);
    pthread_spinlock_t globalLock{};
    void start_workers();
    void start_socketworkers();
//private:
    queue<shared_ptr<service>> global_msg_queue;  //全局队列

    int WORKER_NUM = 3;
    vector<worker*> workers;
    vector<thread*> threads;

    void send(shared_ptr<basemsg> msg);

    void send_msg_2_service(int sid, shared_ptr<basemsg> msg);


};


