#pragma once

using namespace std;

class worker
{
private:
    /* data */
public:

    worker(int id, int eachnum);
    int id;
    int eachNum ; //单次处理消息数
    void operator()(); //线程函数
};


