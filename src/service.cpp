#include "service.h"
#include "pthread.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <memory>

service::service() {
    pthread_spin_init(&queueLock, PTHREAD_PROCESS_PRIVATE );
}

service::~service() {
    pthread_spin_destroy(&queueLock);
}

void service::pushMsg(shared_ptr<basemsg> msg) {
    pthread_spin_lock(&queueLock);
    {
        msg_queue.push(msg);
    }
    pthread_spin_unlock(&queueLock);
};

shared_ptr<basemsg> service::popMsg() {
    shared_ptr<basemsg> msg = make_shared<basemsg>();
    pthread_spin_lock(&queueLock);
    {
        if (!msg_queue.empty()) {
            msg = msg_queue.front();
            msg_queue.pop();
        };
    }
    pthread_spin_unlock(&queueLock);
    return msg;
};

bool service::in_global()
{
    return inGlobal;
}

void service::set_in_global(bool flag){
    inGlobal = flag;
}

void service::on_msg(shared_ptr<basemsg> msg){
    if (msg->type == basemsg::TYPE::SOCKET_ACCEPT)
    {
//        这里用到了动态类型转换。代
//        码中的dynamic_pointer_cast是动态类型转换的方法，可以将
//        shared_ptr＜BaseMsg＞转换成shared_ptr＜ServiceMsg＞类型，代码
//        中的auto相当于shared_ptr＜ServiceMsg＞。
//        基类转换为子类，dynamic_cast普通的类，dynamic_pointer_cast为智能指针时。
        auto m = dynamic_pointer_cast<socket_accept_msg>(msg);
        on_accept_msg(m);
    };

    if (msg->type == basemsg::TYPE::SERVICE)
    {
        auto m = dynamic_pointer_cast<ServiceMsg>(msg);
        on_service_msg(m);
    };

    if (msg->type == basemsg::TYPE::SOCKET_RW)
    {
        auto m = dynamic_pointer_cast<socket_rw_msg>(msg);
        on_rw_msg(msg);
    };
};

void service::on_accept_msg(shared_ptr<basemsg> msg){

}

void service::on_service_msg(shared_ptr<basemsg> msg){

}

void service::on_rw_msg(shared_ptr<basemsg> msg){

}

void service::process_msg()
{
    pthread_spin_lock(&queueLock);
    {
        auto msg = msg_queue.front();
        if (msg)
        {
            on_msg(msg);
            msg_queue.pop();
        };

    }
    pthread_spin_unlock(&queueLock);
}