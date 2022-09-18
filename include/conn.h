//
// Created by diandian on 9/16/22.
// 套接字是由操作系统管理，conn是对套接字的一层封装，可自由加属性
//


#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "pthread.h"

using namespace std;

class conn{
public:
    enum TYPE {          //消息类型
        LISTEN = 1,
        CLIENT = 2,
    };

    uint8_t type;
    int fd;
    uint32_t serviceId;
    bool is_listen_conn();
};

//conn
class conn_mgr {
public:
    conn_mgr(/* args */);
    ~conn_mgr();
    void add_conn(int fd, uint32_t id, conn::TYPE type);
    shared_ptr<conn> get_conn(int fd);
    bool remove_conn(int fd);
    static  conn_mgr *getInstance();
    static mutex m_mutex;
private:
    unordered_map<uint32_t, shared_ptr<conn>> conns;
    pthread_rwlock_t connsLock; //读写锁
    static conn_mgr  *m_pInstance;

};
