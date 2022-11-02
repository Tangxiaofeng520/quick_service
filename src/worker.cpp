#include "worker.h"

#include <memory>
#include <iostream>
#include "service.h"
#include "qs.h"
void worker::operator()(){
    cout<<" worker id ="<< id << " working  "<<endl;
    shared_ptr<service> srv = qs::inst->pop_global_msg_queue();
    if (!srv) {
        //涨停工作线程
        cout<<" worker id ="<< id << " working !srv "<<endl;
        sleep(1000);
    }
    else
    {   cout<<" worker id ="<< id << " working !process_msgs "<<endl;
        srv->process_msgs(eachNum);

    }
};
worker::worker(int id,int eachnum) {
    id = id;
    eachNum = eachnum;
    cout<<"create worker id ="<< id << " eachnum = " << eachNum <<endl;
}