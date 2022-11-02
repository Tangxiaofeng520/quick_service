//
// Created by diandian on 9/8/22.
//
#include <signal.h>
#include "qs.h"
#include <iostream>
int main(){
    std::cout<<"工程开始";
    new qs();
    qs* qs_obj = qs::inst;
    qs_obj->start_workers();
    qs_obj->start_socketworkers();
    sleep(10000);
    qs_obj->close_socketworkers();
}