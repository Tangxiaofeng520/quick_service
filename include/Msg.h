#pragma once
#include <memory>
using namespace std;
/*
struct message {
	uint32_t source;
	int session;
	void * data;
	size_t sz;
};

struct message_queue {
	//struct spinlock lock;
    uint32_t handle;
	struct message *queue;
    struct message_queue *next;
};

struct global_queue {
	struct message_queue *head;
	struct message_queue *tail;
	//struct spinlock lock;
};
*/

class basemsg
{
private:
    /* data */
public:
    enum TYPE {          //消息类型
        SERVICE = 1, 
        SOCKET_ACCEPT = 2,
        SOCKET_RW = 3,
    }; 
    uint8_t type;           //消息类型
};


class ServiceMsg : public basemsg 
{
public:
    uint32_t source;
	int session;
	//void * data;
	shared_ptr<char> buff;
	size_t sz;
};

class socket_accept_msg :public basemsg{
public:
	int listenfd;//监听fd
	int clientfd;//链接的fd
};

class socket_rw_mag : public basemsg {
public:
	int fd;
	bool isread = false;
	bool iswrite = false;
};
