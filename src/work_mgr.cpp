//
// Created by diandian on 11/1/22.
//
#include "worker.h"
#include "work_mgr.h"


void work_mgr::creat_works(){
    for (int i = 0;i<WORKER_NUM;i++)
    {
        worker* Worker = new worker(i, 2<<i);
        workers.push_back(Worker);
        thread* wt = new thread(*Worker);
        workertheads.push_back(wt);
    }
}