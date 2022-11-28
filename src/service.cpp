#include "service.h"
#include "pthread.h"

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "qs.h"
#include "LuaAPI.h"
#include <memory>

service::service() {
    pthread_spin_init(&queueLock, PTHREAD_PROCESS_PRIVATE );
}

service::~service() {
    cout<<"~~service"<<endl;
    pthread_spin_destroy(&queueLock);
    OnExit();
}

void service::init(){
    //新建Lua虚拟机
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    //注册qs系统API
    LuaAPI::Register(luaState);
    cout << "type = " << type <<endl;
    //执行Lua文件
    string filename = "../srv_lua/" + *type + "/init.lua";
    int isok = luaL_dofile(luaState, filename.data());
    if(isok == 1){ //成功返回值为0，失败则为1.
        cout << "run lua fail:" << lua_tostring(luaState, -1) << endl;
    }
    lua_getglobal(   luaState,"OnInit");
    lua_pushinteger(luaState,1);
    isok = lua_pcall(luaState,1,0,0);
    if(isok == 1){ //成功返回值为0，失败则为1
        cout << "call lua func fail:" << lua_tostring(luaState, -1) << endl;
    }

}

void service::OnExit() {
    //关闭Lua虚拟机
    lua_close(luaState);
}

void service::pushMsg(shared_ptr<basemsg> msg) {
    pthread_spin_lock(&queueLock);
    {
        msg_queue.push(msg);
    }
    pthread_spin_unlock(&queueLock);
};

shared_ptr<basemsg> service::popMsg() {
    shared_ptr<basemsg> msg = make_shared<basemsg>();
    pthread_spin_lock(&queueLock);
    {
        if (!msg_queue.empty()) {
            msg = msg_queue.front();
            msg_queue.pop();
        };
    }
    pthread_spin_unlock(&queueLock);
    return msg;
};


bool service::in_global()
{
    return inGlobal;
}

void service::set_in_global(bool flag){
    inGlobal = flag;
}

void service::on_msg(shared_ptr<basemsg> msg){
    cout<<"serviceid == "<<id<<" ::on_msg "<<endl;
    if (msg->type == basemsg::TYPE::SOCKET_ACCEPT)
    {
//        这里用到了动态类型转换。代
//        码中的dynamic_pointer_cast是动态类型转换的方法，可以将
//        shared_ptr＜BaseMsg＞转换成shared_ptr＜ServiceMsg＞类型，代码
//        中的auto相当于shared_ptr＜ServiceMsg＞。
//        基类转换为子类，dynamic_cast普通的类，dynamic_pointer_cast为智能指针时。
        auto m = dynamic_pointer_cast<socket_accept_msg>(msg);
        on_accept_msg(m);
    };

    if (msg->type == basemsg::TYPE::SERVICE)
    {
        auto m = dynamic_pointer_cast<ServiceMsg>(msg);
        on_service_msg(m);
    };

    if (msg->type == basemsg::TYPE::SOCKET_RW)
    {
        auto m = dynamic_pointer_cast<socket_rw_msg>(msg);
        on_rw_msg(m);
    };
};

void service::on_accept_msg(shared_ptr<basemsg> msg){
    cout<<"serviceid == "<<id<<" ::on_accept_msg "<<endl;
}

void service::on_service_msg(shared_ptr<ServiceMsg> msg){
    cout<<"serviceid == "<<id<<" ::on_service_msg "<<endl;
}

void service::on_rw_msg(shared_ptr<socket_rw_msg> msg){
    cout<<"serviceid == "<<id<<" ::on_rw_msg "<<endl;
    int fd = msg->fd;
    //可读
    if(msg->isread) {
        shared_ptr<conn> conn_obj = qs::inst->get_conn_mgr()->get_conn(fd);
        conn_obj->read_buff();
    }
    //可写（注意没有else）
    if(msg->iswrite) {
        shared_ptr<conn> conn_obj = qs::inst->get_conn_mgr()->get_conn(fd);
        if(conn_obj){
            conn_obj->OnWriteable();
        }
    }
}

bool service::process_msg()
{
    auto msg = popMsg();
    if (msg) {
        on_msg(msg);
        return true;
    }
    return false;
}

void service::process_msgs(int num){
    for(int i=0; i<num; i++) {
        bool succ = process_msg();
        if(!succ){
            break;
        }
    }
}

void service::exit(){
    cout<<"serviceid == "<<id<<" ::exit "<<endl;
}