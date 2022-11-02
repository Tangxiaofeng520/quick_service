#include "qs.h"
#include <pthread.h>
#include "worker.h"
#include "thread"
#include "service_mgr.h"
#include "memory"
#include "work_mgr.h"
using namespace std;


qs* qs::inst;
qs::qs(/* args */)
{
    inst = this;
}

void qs::init()
{
    serviceMgr = new service_mgr();
}

void qs::start()
{
    pthread_spin_init(&globalLock, PTHREAD_PROCESS_PRIVATE );
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
    bool in_global = service->in_global();
    service->pushMsg(msg);
    if (!in_global)
    {
        pthread_spin_lock(&globalLock);
        {
            global_msg_queue.push(service);
            service->set_in_global(true);
        }
        pthread_spin_unlock(&globalLock);
    }
    //唤起进程，不放在临界区里面  todo
}

void qs::start_workers()
{
    work_mgr* workerMgr = new work_mgr();
    workerMgr->creat_works();
}