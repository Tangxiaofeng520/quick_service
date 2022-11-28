//
// Created by diandian on 9/8/22.
//
#include <signal.h>
#include "qs.h"
#include <iostream>
#include "LuaManager.h"
void test_lua (qs* qs_obj)
{
    qs_obj->get_service_mgr()->new_service(make_shared<string>("agent"));
}


int main(){
    std::cout<<"工程开始";
    new qs();
    qs* qs_obj = qs::inst;
    qs_obj->init();
    qs_obj->start_workers();
    qs_obj->start_socketworkers();
    //test_lua ( qs_obj);
    sleep(5000);
    cout<<"sleep  finsih";
    //qs_obj->close_socketworkers();
}


