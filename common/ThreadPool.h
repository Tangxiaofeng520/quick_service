//
// Created by diandian on 11/18/22.
//

#ifndef QUICK_SERVICE_THREADPOOL_H
#define QUICK_SERVICE_THREADPOOL_H


#include <stdio.h>
#include <pthread.h>
#include <functional>
#include <vector>
#include <queue>

class ThreadPool {
public:
    typedef void *(WrokerFunc)(void* arg);

    struct Task {
        WrokerFunc* run;
        void* arg;
    };

    explicit ThreadPool(int thread_num);
    ~ThreadPool();
    void addTask(WrokerFunc* func, void* arg);

private:
    std::queue<Task*> task_queue_;
    std::vector<pthread_t> thread_list_;
    bool is_running_;  // note: is_running_不用原子变量或者锁操作可能存在卡死问题
    pthread_mutex_t mutex_;
    pthread_cond_t condition_;

    static void* thread_routine(void* pool_ptr);
    void thread_worker();
};




#endif //QUICK_SERVICE_THREADPOOL_H


