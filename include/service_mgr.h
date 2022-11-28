#pragma once
#include "Msg.h"
#include <queue>
#include "service.h"
#include <unordered_map>
#include <map>
class service_mgr
{
public:
    uint32_t maxid = 0;
    pthread_rwlock_t servicesLock;   //读写锁
    service_mgr();
    ~service_mgr();
    typedef unordered_map<uint32_t, shared_ptr<service>> id_srvs_umap;
    id_srvs_umap id_2_srvs;
    map<shared_ptr<string> ,id_srvs_umap > name_2_id;
    shared_ptr<service> get_service(uint32_t sid);

    uint32_t get_maxid();

    shared_ptr<service> new_service(shared_ptr<string> type);

    void service_exit(uint32_t id);

    void CheckAndPutGlobal(shared_ptr<service> srv);
};

