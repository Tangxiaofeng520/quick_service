#include "qs.h"
#include <pthread.h>
#include <iostream>
#include "worker.h"
#include "thread"
#include "service_mgr.h"
#include "memory"
#include "work_mgr.h"
#include "socketworker.h"
using namespace std;


qs* qs::inst;
qs::qs(/* args */)
{
    inst = this;
    pthread_spin_init(&globalLock, PTHREAD_PROCESS_PRIVATE );
}

qs::~qs(){
    cout<<"~qs()"<<endl;
    pthread_spin_destroy(&globalLock );
}

void qs::init()
{
    serviceMgr = new service_mgr();
    connMgr = conn_mgr::getInstance();
}

void qs::start()
{
}

shared_ptr<service> qs::pop_global_msg_queue()
{

    shared_ptr<service> srv = NULL;
    pthread_spin_lock(&globalLock);
    {
            if (!global_msg_queue.empty())
            {
                srv = global_msg_queue.front();
                global_msg_queue.pop();
            }
    }
    pthread_spin_unlock(&globalLock);
    return srv;

}

shared_ptr<service> qs::push_global_msg_queue(shared_ptr<service> srv)
{
   pthread_spin_lock(&globalLock);
   {
        global_msg_queue.push(srv);
   }
   pthread_spin_unlock(&globalLock);
   return srv;

}

void qs::send_msg_2_service(int sid, shared_ptr<basemsg> msg) {
    auto service = serviceMgr->get_service(sid);
    if (!service){
        cout<<"service empty "<<endl;
    }
    bool in_global = service->in_global();
    service->pushMsg(msg);
    if (!in_global)
    {
        push_global_msg_queue(service);
        service->set_in_global(true);
    }
    //唤起进程，不放在临界区里面
    workerMgr->check_and_weakup();
}


void qs::start_workers()
{
    workerMgr = new work_mgr();
    workerMgr->creat_works();
}

void qs::start_socketworkers()
{
    socketWorker = new socketworker();
    socketWorker->init();

    socketWorker->start_socket(8002);
    socketthread = new thread(*socketWorker);
}

void qs::close_socketworkers()
{
    socketWorker->clone_socket();
}

int qs::get_globalLen(){
    return globalLen;
};

work_mgr* qs::get_work_mgr(){
    return workerMgr;
};


service_mgr* qs::get_service_mgr(){
    return serviceMgr;
};


conn_mgr* qs::get_conn_mgr(){
    return connMgr;
};

socketworker* qs::get_socketworker(){
    return socketWorker;
};
