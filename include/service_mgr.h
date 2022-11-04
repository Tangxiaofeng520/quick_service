#pragma once
#include "Msg.h"
#include <queue>
#include "service.h"
#include <unordered_map>
class service_mgr
{
public:
    uint32_t maxid = 0;
    pthread_rwlock_t servicesLock;   //读写锁
    service_mgr();
    ~service_mgr();
    unordered_map<uint32_t, shared_ptr<service>> services;

    shared_ptr<service> get_service(uint32_t sid);

    uint32_t get_maxid();

    shared_ptr<service> new_service();

    void service_exit(uint32_t id);
};

