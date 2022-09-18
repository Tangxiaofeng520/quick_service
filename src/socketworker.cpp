#include "socketworker.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <cassert>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include "conn.h"
#define BACKLOG 128
using namespace std;
conn_mgr * Conn_mgr = NULL;
//class conn_mgr;
void
socketworker::init() {
    efd = epoll_create(1024);
    assert(efd > 0);
     Conn_mgr = conn_mgr::getInstance();
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
    int fd = ev.data.fd;
    auto Conn = Conn_mgr->get_conn(fd);
    if(Conn == NULL){
        cout << "OnEvent error, conn == NULL" << endl;
        return;
    }
    //事件类型
    bool isRead = ev.events & EPOLLIN;
    bool isWrite = ev.events & EPOLLOUT;
    bool isError = ev.events & EPOLLERR;

    if (Conn->is_listen_conn() )
    {
        if (isRead){
            onaccept(Conn);
        }
    }
    else
    {

    }


};

void socketworker::onaccept(shared_ptr<conn> Conn)
{

};

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
    Conn_mgr->add_conn(listen_fd,0,conn::TYPE::LISTEN);
    add_event(listen_fd);
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
    return 0;
}

void socketworker::add_event(int fd) {
    cout << "AddEvent fd " << fd << endl;
    //添加到epollP
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        cout << "AddEvent epoll_ctl Fail:" << strerror(errno) << endl;
    }
}