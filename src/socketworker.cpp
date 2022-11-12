#include "socketworker.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <cassert>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <csignal>
#include "conn.h"
#include "Msg.h"
#include "service.h"
#include "qs.h"
#define BACKLOG 128
using namespace std;
conn_mgr * Conn_mgr = NULL;
//class conn_mgr;
void
socketworker::init() {
    efd = epoll_create(1024);
    assert(efd > 0);
    Conn_mgr = qs::inst->get_conn_mgr();
    //service_mgr* serviceMgr = qs::inst->get_service_mgr();
    cout<< "socketworker::init finish"<<endl;
}

socketworker::~socketworker(){
    cout<<"~socketworker()"<<endl;
    //clone_socket();
}

void socketworker::operator()(){
    const int EVENT_SIZE = 64;
    struct epoll_event events[EVENT_SIZE];
    while (true)
    {
        int event_num = epoll_wait(efd,events,EVENT_SIZE,-1);
        for (int i = 0; i < event_num; i++)
        {
            epoll_event ev = events[i];
            onEvent(ev);
        }
    }
}

void socketworker::onEvent(epoll_event ev) {
    cout<<"onEvent"<<endl;
    int fd = ev.data.fd;
    auto Conn = Conn_mgr->get_conn(fd);
    cout<<"get_conn ok "<<endl;
    if(Conn == NULL){
        cout << "OnEvent error, conn == NULL" << endl;
        return;
    }
    //事件类型
    bool isRead = ev.events & EPOLLIN;
    bool isWrite = ev.events & EPOLLOUT;
    bool isError = ev.events & EPOLLERR;
    cout<<"onEvent"<<isRead<< isWrite <<isError <<endl;
    if (isError) {
        cout << "event error" << endl;
    };
    if (Conn->is_listen_conn() )
    {
        if (isRead){
            onaccept(Conn);
        }
    }
    else
    {
        onRW(Conn,isRead,isWrite);
    };


};

void socketworker::onRW(shared_ptr<conn> client_conn, bool isRead,bool isWrite )
{
    auto msg= make_shared<socket_rw_msg>();
    msg->type=basemsg::TYPE::SOCKET_RW ;
    msg->fd = client_conn->fd;
    msg->isread = isRead;
    msg->iswrite = isWrite;
    qs::inst->send_msg_2_service(client_conn->serviceId,msg);
}

void socketworker::onaccept(shared_ptr<conn> listenconn)
{
    sockaddr client_adr{};
    socklen_t client_len;
    cout<<"accept begin"<<endl;
    int client_fd = accept(listenconn->fd, &client_adr, &client_len);
    cout<<"client_fd "<<client_fd<<endl;
//    if (client_fd<0){
//        cout<<"client_fd =  "<<client_fd<<" errno = "<< errno << EMFILE <<endl;
//    }
    //设置非阻塞
    cout<<"add_event fcntl "<<endl;
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
    cout<<"add_event begin"<<endl;
    //创建service
    service_mgr* serviceMgr = qs::inst->get_service_mgr();
    auto srv = serviceMgr->new_service();

    //添加conn
    shared_ptr<conn>client_conn = Conn_mgr->add_conn(client_fd,srv->id,conn::TYPE::CLIENT);
    add_event(client_fd,(EPOLLIN | EPOLLET));
    cout<<"add_event end"<< endl;
    //创建message
    auto msg= make_shared<socket_accept_msg>();
    msg->type=basemsg::TYPE::SOCKET_ACCEPT ;
    msg->listenfd = listenconn->fd;
    msg->clientfd = client_fd;
    // 推给service
    cout<<"send_msg_2_service"<< endl;
    qs::inst->send_msg_2_service(client_conn->serviceId,msg);
    cout<<"send_msg_2_service"<< endl;
    cout << "socketworker::onaccept finish" << endl;
};

int socketworker::start_socket(uint32_t port) {
    //signal(SIGPIPE,SIG_IGN);
    //创建socket
    listen_fd = socket(AF_INET,SOCK_STREAM,0);
    cout<<"listen_fd = "<<listen_fd<<endl;
    if(listen_fd < 0){cout<<"listen_fd = "<<listen_fd<<endl;}

    //设置非阻塞
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);
    //创建地址结构
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind
    bool bReuseaddr=true;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bool));
    int r = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    if( r == -1){
        cout << "bind error,errno = " << errno<< endl;
        return -1;
    }
    //监听
    r = listen(listen_fd, BACKLOG); //see
    if( r < -1){
        cout << "listen error," << endl;
        return -1;
    }
    service_mgr* serviceMgr = qs::inst->get_service_mgr();
    auto srv = serviceMgr->new_service();
    Conn_mgr->add_conn(listen_fd,srv->id,conn::TYPE::LISTEN);
    add_event(listen_fd);
    cout<< "socketworker::start finish"<<endl;
    return listen_fd;
}
void socketworker::clone_socket(){
    Conn_mgr->remove_conn(listen_fd);
    close(listen_fd);
    remove_event(listen_fd);
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

void socketworker::mod_event(int fd, bool epollOut){
    cout << "ModifyEvent fd " << fd << " " << epollOut << endl;
    struct epoll_event ev;
    ev.data.fd = fd;

    if(epollOut){
        ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
    }
    else
    {
        ev.events = EPOLLIN | EPOLLET ;
    }
    epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev);

}

void socketworker::add_event(int fd ) {
    cout << "AddEvent fd " << fd << endl;
    //添加到epollP
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        cout << "AddEvent epoll_ctl Fail:" << strerror(errno) << endl;
    }
}

void socketworker::add_event(int fd ,int events) {
    cout << "AddEvent fd " << fd << endl;
    //添加到epollP
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        cout << "AddEvent epoll_ctl Fail:" << strerror(errno) << endl;
    }
}


