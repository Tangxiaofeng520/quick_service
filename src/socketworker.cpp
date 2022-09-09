#include "socketworker.h"
#include <sys/epoll.h>
#include <cassert>

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
            onEvent();
        }
    }
}

void socketworker::onEvent() {}
