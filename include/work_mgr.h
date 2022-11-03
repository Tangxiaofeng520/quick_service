//
// Created by diandian on 11/1/22.
//

#include "worker.h"
#include <vector>
#include <thread>
#ifndef QUICK_SERVICE_WORK_MGR_H
#define QUICK_SERVICE_WORK_MGR_H


class work_mgr {
public:
    int WORKER_NUM = 3;              //工作线程数（配置）

    //休眠和唤醒
    pthread_mutex_t sleepMtx;
    pthread_cond_t sleepCond;
    int sleepCount = 0;        //休眠工作线程数

    vector<worker*> workers ;
    vector<thread*> workertheads;
    work_mgr(/* args */);
    ~work_mgr();
    void creat_works();

    void wait();

    void worker_wait();

    void check_and_weakup();
};



#endif //QUICK_SERVICE_WORK_MGR_H
