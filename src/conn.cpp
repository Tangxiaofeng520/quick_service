//
// Created by diandian on 9/16/22.
//

#include <cassert>
#include "conn.h"
#include "qs.h"
#include "../proto/user.pb.h"

#include <iostream>
#include <memory>
#include <mutex>
#include <unistd.h>

void conn::read_buff(){
    const int BUFFSIZE = 512;
    char buff[BUFFSIZE];
    int len = 0;
    do {
        len = read(fd, &buff, BUFFSIZE);
        cout<< "read len = "<< len << endl;
        if(len > 0){
            OnSocketData(fd, buff, len);
        }
    }while(len == BUFFSIZE);

    if(len <= 0 && errno != EAGAIN) { //数据读完
        cout<<"buff read end"<<endl;
    }
}


void conn::OnSocketData(int fd, const char* buff, int len) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    cout << "OnSocketData" << fd << endl;
    //cout << "OnSocketData" << to_string(buff) << endl;
    string strData(buff);
//    IM::Account a1;
//    a1.set_id(1);
//    a1.set_name("first");
//    a1.set_password("12345678");
//
//    string serializeToStr;
//    a1.SerializeToString(&serializeToStr);
    IM::Account account2;
    if(!account2.ParseFromString(strData))
    {
        cerr << "failed to parse account2." << endl;
        return ;
    }
    cout << "反序列化：" << endl;
    cout << account2.id() << endl;
    cout << account2.name() << endl;
    cout << account2.password() << endl;
    google::protobuf::ShutdownProtobufLibrary();

}

void conn::EntireWriteWhenEmpty(shared_ptr<char> buff,streamsize len)
{
    char* s = buff.get() ;
    //谨记：>=0, -1&&EAGAIN, -1&&EINTR, -1&&其他
    streamsize n = write(fd, s, len);
    if(n < 0 && errno == EINTR) { }; //仅提醒你要注意
    cout << "EntireWriteWhenEmpty write n=" << n << endl;
    //情况1-1：全部写完
    if(n >= 0 && n == len) {
        return;
    }
    //情况1-2：写一部分（或没写入）
    if( (n > 0 && n < len) || (n < 0 && errno == EAGAIN) ) {
        auto obj = make_shared<WriteObject>();
        obj->start = n;
        obj->buff = buff;
        obj->len = len;
        writers.push(obj);
        socketworker* socketworker_obj = qs::inst->get_socketworker();
        socketworker_obj->mod_event(fd, true);  //修改 epoll events 让下次可读的时候通知
        return;
    }
    //情况1-3：真的发生错误
    cout << "EntireWrite write error " <<  endl;
}

void conn::EntireWriteWhenNotEmpty(shared_ptr<char> buff,streamsize len)
{
    auto obj = make_shared<WriteObject>();
    obj->start = 0;
    obj->buff = buff;
    obj->len = len;
    writers.push(obj);
}

//上层调用写入 buff
void conn::EntireWrite(shared_ptr<char> buff, streamsize len) {
    //情况1：没有待写入数据，先尝试写入
    if(writers.empty()) {
        EntireWriteWhenEmpty(buff, len);
    }
        //情况2：有待写入数据，添加到末尾
    else{
        EntireWriteWhenNotEmpty(buff, len);
    }
}

// epoll 通知可写 时写入
void conn::OnWriteable(){
    while(WriteFrontObj()){
        //循环
    }
    if (writers.empty()){
        socketworker* socketworker_obj = qs::inst->get_socketworker();
        socketworker_obj->mod_event(fd, false);
    }
}

//返回值:是否完整的写入了一条
bool conn::WriteFrontObj() {
    //没待写数据
    if(writers.empty()) {
        return false;
    }
    //获取第一条
    auto obj = writers.front();

    //谨记：>=0, -1&&EAGAIN, -1&&EINTR, -1&&其他
    char* s = obj->buff.get() + obj->start;
    int len = obj->len - obj->start;
    int n = write(fd, s, len);
    cout << "WriteFrontObj write n=" << n << endl;
    if(n < 0 && errno == EINTR) { }; //仅提醒你要注意
    //情况1-1：全部写完
    if(n >= 0 && n == len) {
        writers.pop(); //出队
        return true;
    }
    //情况1-2：写一部分（或没写入）
    if( (n > 0 && n < len) || (n < 0 && errno == EAGAIN) ) {
        obj->start += n;
        return false;
    }
    //情况1-3：真的发生错误
    cout << "EntireWrite write error " << endl;
    return true;
}

