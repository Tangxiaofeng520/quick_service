#include "socketworker.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <cassert>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#define BACKLOG 128
using namespace std;
void
socketworker::init() {
    efd = epoll_create(1024);
    assert(efd > 0);
}

void socketworker::operator()(){
    const int EVENT_SIZE = 64;
    struct epoll_event events[EVENT_SIZE];
    while (true)
    {
        int event_num = epoll_wait(efd,events,EVENT_SIZE,-1);
        assert(event_num >= 0);
        for (int i = 0; i < event_num; i++)
        {
            epoll_event ev = events[i];
            onEvent(ev);
        }
    }
}

void socketworker::onEvent(epoll_event ev) {




}

int socketworker::start_socket(uint32_t port) {
    int  listen_fd;
    //创建socket
    listen_fd = socket(AF_INET,SOCK_STREAM,0);
            assert(listen_fd < 0);
            //设置非阻塞
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);
    //创建地址结构
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind
    int r = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    if( r == -1){
        cout << "listen error, bind fail" << endl;
        return -1;
    }
    //监听
    r = listen(listen_fd, BACKLOG); //see
    assert(r < 0);
    return 0;
}

int socketworker::remove_event(int fd){
    cout<<"remove event"<<endl;
    int code = epoll_ctl(efd,EPOLL_CTL_DEL,fd,NULL);
    if (code < 0)
    {
        cout<<"remove event fail"<<endl;
    };
    return 0;
}

int socketworker::mod_event(int fd){
    return ;
}

int
