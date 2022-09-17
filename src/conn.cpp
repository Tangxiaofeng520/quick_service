//
// Created by diandian on 9/16/22.
//

#include <cassert>
#include "conn.h"

conn_mgr::conn_mgr(){
    assert(pthread_rwlock_init(&connsLock, NULL)==0);
}

conn_mgr::~conn_mgr(){
    pthread_rwlock_destroy(&connsLock)
}

#include "conn.h"
void conn_mgr :: add_conn (int fd,uint32_t id, conn::TYPE type)
{

}