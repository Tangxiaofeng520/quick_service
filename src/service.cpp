#include"service.h"


service::service() {
    pthread_spin_init(&queueLock, PTHREAD_PROCESS_PRIVATE );
}

service::service() {
    pthread_spin_destroy(&queueLock)
}

