#include "modulesdk.h"
#include<stdio.h>
#include<unistd.h>
void callback(int sock,struct sockaddr addr);
void start()
{
    printf("start\n");
}
void before(int sock,struct sockaddr addr)
{
    printf("before\n");
}
void after(int sock,struct sockaddr addr,pthread_t tid)
{
    printf("after");
}
ModuleType_t _init_(int i)
{
    ModuleType_t a =SDK_CreateModule("测试","这是一个测试模块",1);
    SDK_AddBindPort(&a,10086,callback);
    SDK_AddFunction(&a,RUN_AT_START,start,0);
    SDK_AddFunction(&a,RUN_AT_BEFORE_THREAD_START,before,0);
    SDK_AddFunction(&a,RUN_AT_AFTER_THREAD_START,after,0);
    return a;
}
void callback(int sock,struct sockaddr addr)
{
    char a[1024];
    recv(sock,a,1024,MSG_NOSIGNAL);
    a[1023]=0;
    printf("%s\n",a);
    close(sock);
}