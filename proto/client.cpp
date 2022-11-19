//
// Created by diandian on 11/8/22.
//
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "user.pb.h"
using namespace std;
int m_sockfd;
#define BUFFER_SIZE                 1024
bool  ConnectToServer() {
    //SOCKET
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0) {
        perror("socket");
        return false;
    }

    /*初始换结构体*/
    struct sockaddr_in sockaddr;
    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockaddr.sin_port = htons(8002);

    //连接服务器
    if (connect(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("connect");
        close(m_sockfd);
        return false;
    }
    printf("...........connect success.............\n");

    return true;
};

//发送数据到服务器实现
int Send(void *buff, int bufflen)
{
    printf("...........Send begin.............\n");
    int code = send(m_sockfd, buff, bufflen, 0);
    std::cout<< "Send end code = " << code <<std::endl;
    return code;
}

int main(){
    IM::Account a1;
    a1.set_id(1);
    a1.set_name("first");
    a1.set_password("12345678");

    string serializeToStr;
    a1.SerializeToString(&serializeToStr);
    char dst[BUFFER_SIZE];
    strcpy(dst, serializeToStr.c_str());
//    IM::Account account2;
//    if(!account2.ParseFromString(serializeToStr))
//    {
//        cerr << "failed to parse student." << endl;
//        return -1;
//    }
//    cout << "反序列化：" << endl;
//    cout << account2.id() << endl;
//    cout << account2.name() << endl;
//    cout << account2.password() << endl;

    ConnectToServer();
    string helloFromClient = "hello from client";
    //cout<<helloFromClient.length()<<endl;
    //Send(&helloFromClient,helloFromClient.length())
    Send( dst, strlen(dst));
    
};
//编译
//g++ client.cpp *.pb.cc -o client -lprotobuf -std=c++11 -lpthread
//运行
//./client


