#include "qs.h"

#include <pthread.h>
#include <spinlock.h>
using namespace std;

qs* qs::inst;
qs::qs(/* args */)
{
    inst = this;
}

qs::~qs()
{
    
}

void qs::start()
{
    pthread_spin_init(&globalLock, PTHREAD_PROCESS_PRIVATE );
}

shared_ptr<service> qs::pop_global_msg_queue()
{
    shared_ptr<service> srv = NULL;
    pthread_spin_lock(&globalLock)
    {
            if (!global_msg_queue.empty())
            {
            ser = global_msg_queue.front();
            global_msg_queue.pop();
            }
    }
    pthread_spin_unlock(&globalLock)
    return srv

}

void qs::push_global_msg_queue(shared_ptr<service> srv)
{
   pthread_spin_lock(&globalLock)
   {
        global_msg_queue.push(srv);
   }
   pthread_spin_unlock(&globalLock)
   return ser

}


void qs::start_workers()
{
    for(i=0,i<WORKER_NUM,i++ ){
        worker *worker = new workers();
        workers.push_back(worker);
        thread *thread = new thread(*worker);
        threads.push_back(thread);
    }
}