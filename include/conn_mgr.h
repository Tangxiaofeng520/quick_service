//
// Created by diandian on 11/9/22.
//

#ifndef QUICK_SERVICE_CONN_MGR_H
#define QUICK_SERVICE_CONN_MGR_H

#include "conn.h"
class conn_mgr {
public:
    conn_mgr(/* args */);
    ~conn_mgr();
    shared_ptr<conn> add_conn(int fd, uint32_t id, conn::TYPE type);
    shared_ptr<conn> get_conn(int fd);
    bool remove_conn(int fd);
    static  conn_mgr *getInstance();
    static mutex m_mutex;
private:
    unordered_map<uint32_t, shared_ptr<conn>> conns;
    pthread_rwlock_t connsLock; //读写锁
    static conn_mgr  *m_pInstance;

};


#endif //QUICK_SERVICE_CONN_MGR_H
