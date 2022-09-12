#pragma once

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


    int start_socket(unsigned int port);

    void add_event(int fd);

    int remove_event(int fd);
};


