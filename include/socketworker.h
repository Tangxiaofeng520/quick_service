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
    void onEvent(); //epoll 事件处理
    ~socketworker();
};

socketworker::socketworker(/* args */)
{
}

socketworker::~socketworker()
{
}
