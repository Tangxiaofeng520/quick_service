//
// Created by xiaofeng on 2022/9/3.
//

#ifndef QUICK_SERVICE_CONN_H
#define QUICK_SERVICE_CONN_H

#pragma  once
using namespace std;
#include "stdint.h"
class conn {
public:
    explicit conn(const uint8_t &type) : type(type) {}

public:
    enum TYPE {
        LISTEN = 1;
        CLIENT = 2;
    };
    int fd;
    uint8_t type;
    uint32_t serviceid;
};
#endif //QUICK_SERVICE_CONN_H
