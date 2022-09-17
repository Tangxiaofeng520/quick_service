//
// Created by diandian on 9/16/22.
//

#include <cassert>
#include "conn.h"
#include <iostream>
#include <memory>


conn_mgr::conn_mgr(){
    assert(pthread_rwlock_init(&connsLock, NULL)==0);
}

conn_mgr::~conn_mgr(){
    pthread_rwlock_destroy(&connsLock);
}

void conn_mgr :: add_conn (int fd,uint32_t id, conn::TYPE type)
{
    auto new_conn = make_shared<conn>();
    new_conn->fd = fd;
    new_conn->type = type;
    new_conn->serviceId = id;
    pthread_rwlock_wrlock(&connsLock);
    {
        conns.emplace(fd, new_conn);
    }
    pthread_rwlock_unlock(&connsLock);
}

shared_ptr<conn> conn_mgr::get_conn(int fd)
{
    shared_ptr<conn> find_conn = NULL;
    pthread_rwlock_rdlock(&connsLock);
    {
        unordered_map<uint32_t, shared_ptr<conn>>::iterator iter = conns.find(fd);
        if (iter != conns.end()){
            find_conn = iter->second;
        }
    }
    return find_conn;
};

bool conn_mgr::remove_conn(int fd){
    int result;
    pthread_rwlock_wrlock(&connsLock);
    {
        result = conns.erase(fd);
    }
    pthread_rwlock_unlock(&connsLock);
    return result == 1;
}

