//
// Created by diandian on 9/16/22.
// 套接字是由操作系统管理，conn是对套接字的一层封装，可自由加属性
//


#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <queue>
#include "pthread.h"

using namespace std;

class WriteObject {
public:
    streamsize start;
    streamsize len;
    shared_ptr<char> buff;
};

class conn{
public:
    enum TYPE {          //消息类型
        LISTEN = 1,
        CLIENT = 2,
    };

    uint8_t type;
    int fd;
    uint32_t serviceId;
    queue<shared_ptr<WriteObject>> writers ;
    bool is_listen_conn();

    void read_buff();

    void EntireWriteWhenEmpty(shared_ptr<char> buff, streamsize len);

    void EntireWriteWhenNotEmpty(shared_ptr<char> buff, streamsize len);

    void EntireWrite(shared_ptr<char> buff, streamsize len);

    bool WriteFrontObj();

    void OnWriteable();

    void OnSocketData(int fd, char *buff, int len);

};


