//
// Created by diandian on 10/29/22.
//
#include "service_mgr.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
service_mgr::service_mgr()
{
    pthread_rwlock_init(&servicesLock,NULL);
}

service_mgr::~service_mgr()
{
    pthread_rwlock_destroy(&servicesLock);
}

shared_ptr<service> service_mgr::get_service(uint32_t sid){
    shared_ptr<service> srv = NULL;
    pthread_rwlock_rdlock(&servicesLock);
    {
        unordered_map<uint32_t, shared_ptr<service>>::iterator iter = services.find (sid);
        if (iter != services.end()){
            srv = iter->second;
        }
    }
    pthread_rwlock_unlock(&servicesLock);
    return srv;
}

shared_ptr<service> service_mgr::new_service(){
    uint32_t sid = get_maxid();
    auto srv = make_shared<service>();
    pthread_rwlock_wrlock(&servicesLock);
    {
        srv->id = get_maxid();
        services.emplace(srv->id, srv);
    }
    pthread_rwlock_unlock(&servicesLock);

    //srv.init()
    return srv;
}

uint32_t  service_mgr::get_maxid() {
    return  maxid ++ ;
}

void service_mgr::service_exit(uint32_t id){
    shared_ptr<service> srv = get_service(id);
    if (!srv) return;
    srv->exit();

}