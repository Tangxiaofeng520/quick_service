#include "worker.h"

#include <memory>
#include "service.h"
#include "qs.h"
void worker::operator()(){
    shared_ptr<service> srv = qs::inst->pop_global_msg_queue();
    if (!srv) {
        //涨停工作线程

    }
    else
    {
        srv->process_msg();

    }
};
worker::worker(int id,int eachnum) {
    id = id;
    eachnum = eachnum;
}