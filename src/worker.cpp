#include "worker.h"

#include <memory>
#include <iostream>
#include "service.h"
#include "qs.h"
#include "work_mgr.h"
void worker::operator()(){
    while(true){
        cout<<" worker id ="<< id << " working  "<<endl;
        shared_ptr<service> srv = qs::inst->pop_global_msg_queue();
        if (!srv) {
            //涨停工作线程
            cout<<" worker id ="<< id << " working !srv "<<endl;
            work_mgr* work_mgr = qs::inst->get_work_mgr();
            work_mgr->worker_wait();
        }
        else
        {   cout<<" worker id ="<< id << " working !process_msgs "<<endl;
            srv->process_msgs(eachNum);
            qs::inst->get_service_mgr()->CheckAndPutGlobal(srv);
        }

    }
};



worker::worker(int id,int eachnum) {
    id = id;
    eachNum = eachnum;
    cout<<"create worker id ="<< id << " eachnum = " << eachNum <<endl;
}