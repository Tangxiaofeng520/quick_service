#pragma once

#include <sys/epoll.h>
#include <memory>
#include "conn.h"
class socketworker
{
private:
    /* data */
public:
    void init();
    void operator()(); //线程函数
    int efd; //操作epoll实例的 文件描述符
    int  listen_fd;

    ~socketworker();
    int start_socket(unsigned int port);

    void add_event(int fd,int events);

    int remove_event(int fd);

    int mod_event(int fd);

    void onEvent(epoll_event ev);//epoll 事件处理
    void onaccept(std::shared_ptr<conn> Conn);


    void onRW(shared_ptr<conn> client_conn, bool isRead, bool isWrite);

    void clone_socket();

    void add_event(int fd);
};


