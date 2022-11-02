//
// Created by diandian on 11/1/22.
//
#include <iostream>
#include "worker.h"
#include "work_mgr.h"


void work_mgr::creat_works(){
    for (int i = 0;i<WORKER_NUM;i++)
    {
        worker* Worker = new worker(i, 2<<i);
        workers.push_back(Worker);
        thread* wt = new thread(*Worker);
        //wt->join();
        workertheads.push_back(wt);
    }
    //wait();
}

//void work_mgr::start_works(){
//    vector<thread*>::iterator it;
//    for(it = workertheads.begin();it!=workertheads.end();it++)
//
//}

void work_mgr::wait()
{
    if (workertheads[0]) {
        cout<<"work_mgr::wait()1"<<endl;
        workertheads[0]->join();
        cout<<"work_mgr::wait()2"<<endl;
    }
}