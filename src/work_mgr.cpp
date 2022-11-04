//
// Created by diandian on 11/1/22.
//
#include <iostream>
#include "worker.h"
#include "work_mgr.h"
#include "qs.h"
work_mgr::work_mgr()
{
    pthread_cond_init(&sleepCond, NULL);
    pthread_mutex_init(&sleepMtx, NULL);
}

work_mgr::~work_mgr()
{
    cout<<"~~work_mgr"<<endl;
    pthread_cond_destroy(&sleepCond);
    pthread_mutex_destroy(&sleepMtx);
}

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

//进入休眠
void work_mgr::worker_wait() {
    pthread_mutex_lock(&sleepMtx);
    {
        sleepCount++;
        // 会先给sleepMtx 解锁，让线程休眠，sleepMtx加锁（线程被唤醒）
        pthread_cond_wait(&sleepCond, &sleepMtx);
        sleepCount--;
    }
    pthread_mutex_unlock(&sleepMtx);
}


void work_mgr::check_and_weakup() {
    if (sleepCount <= 0 ) return;
    int globalLen = qs::inst->get_globalLen();
    //此时去 sleepCount 应当加锁 ，但是考虑的性能问题  允许错误
    if( WORKER_NUM - sleepCount <= globalLen ) {
        cout << "weakup" << endl;
        pthread_cond_signal(&sleepCond);
    }

}
void work_mgr::wait()
{
    if (workertheads[0]) {
        cout<<"work_mgr::wait()1"<<endl;
        workertheads[0]->join();
        cout<<"work_mgr::wait()2"<<endl;
    }
}