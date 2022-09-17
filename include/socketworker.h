#pragma once

#include <sys/epoll.h>

class socketworker
{
private:
    /* data */
public:
    void init();
    void operator()(); //线程函数
    int efd; //操作epoll实例的 文件描述符
    socketworker(/* args */);
    ~socketworker();


    int start_socket(unsigned int port);

    void add_event(int fd);

    int remove_event(int fd);

    int mod_event(int fd);

    void onEvent(epoll_event ev);//epoll 事件处理
};


