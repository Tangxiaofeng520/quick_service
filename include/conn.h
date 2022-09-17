//
// Created by diandian on 9/16/22.
//
#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

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
};

class conn_mgr {
public:

    conn_mgr(/* args */);
    ~conn_mgr();
    void add_conn(int fd, uint32_t id, conn::TYPE type);
private:
    unordered_map<uint32_t, shared_ptr<conn>> conn;
    pthread_rwlock_t connsLock; //读写锁

};
