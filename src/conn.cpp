//
// Created by diandian on 9/16/22.
//

#include <cassert>
#include "conn.h"
#include <iostream>
#include <memory>
#include <mutex>
conn_mgr* conn_mgr::m_pInstance = nullptr;//懒汉式的写法
mutex conn_mgr::m_mutex;

bool conn::is_listen_conn()
{
    return type == conn::TYPE::LISTEN;
}

conn_mgr::conn_mgr(){
    cout<<"conn_mgr() " <<endl;
    int r = pthread_rwlock_init(&connsLock, NULL);
    if (r !=0 )
    {
        cout<<"pthread_rwlock_init fail r = "<< r <<endl;
    }
}

conn_mgr::~conn_mgr(){
    cout<<"~conn_mgr"<<endl;
    pthread_rwlock_destroy(&connsLock);
}

shared_ptr<conn> conn_mgr :: add_conn (int fd,uint32_t id, conn::TYPE type)
{
    auto new_conn = make_shared<conn>();
    new_conn->fd = fd;
    new_conn->type = type;
    new_conn->serviceId = id;
    int a = pthread_rwlock_wrlock(&connsLock);
    cout<<a;
    {
        conns.emplace(fd, new_conn);
    }
    pthread_rwlock_unlock(&connsLock);
    return new_conn;
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
    pthread_rwlock_unlock(&connsLock);
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
};

//懒汉式单例
conn_mgr * conn_mgr ::getInstance()
{

    m_mutex.lock();//防止多线程调 生成多个实例
    {
        if (m_pInstance == NULL)
            m_pInstance = new conn_mgr();
        return m_pInstance;
    }
    m_mutex.unlock();

};