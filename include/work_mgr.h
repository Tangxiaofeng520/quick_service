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
    vector<worker*> workers ;
    vector<thread*> workertheads;
    void creat_works();

    void wait();
};



#endif //QUICK_SERVICE_WORK_MGR_H
