# 欢迎使用extrapid SDK

## 1.extrapid SDK概述

extrapid SDK(以下简称SDK)是一个为extrapid程序开发模块的开发工具包，使用SDK可以更方便快捷的为extrapid编写模块

## 2.使用extrapid SDK开发模块
### (1) 开发前的准备
1.Linux操作系统环境  
2.能够使用C语言开发简单的程序  
3.gcc编译器  
4.extrapid SDK开发包

### (2) 如何创建一个模块
extrapid模块以动态链接库的形式存在，extrapid会在启动时尝试加载符合要求的模块。每个模块必须包含一个可以从外部访问的函数 `ModuleType_t _init_(int)`主程序启动后将会调用模块内的_init_函数，模块只需要将自己的信息通过该函数返回即可  
`ModuleType_t`类型来储存所有与模块相关的信息，其为一个结构体，无需开发者手动为其赋值，只需要使用SDK中的几个函数即可为其赋值  
模块可以为主程序提供数个函数，他们会分别在主程序运行的不同时段被调用，以下为他们的调用时段：  
|调用时段|解释|参数|返回值|
|-------|---|----|------|
|start|主程序初始化完成后|void|void|
|before_thread_start|与客户端建立连接后|int sock,struct sockaddr|void|
|after_thread_start|客户连接处理函数启动后|int sock,struct sockaddr,pthread_t|void|

以上函数的创建方法将在下文详细叙述  
  
SDK函数解释:  
**函数原型**  
```C
ModuleType_t SDK_CreateModule(const char *name, const char *description,int version);
```
|参数|类型|含义|
|-|-|-|
|name|const char*|模块名称|
|description|const char*|模块描述|
|version|int|模块版本|

返回值：创建的模块信息，需要作为_init_函数的返回值返回  
  

函数作用:创建一个包含模块信息的结构体稍后作为_init_函数的返回值 
  
函数用法:在_init_函数中使用它并传入这个模块的信息，将其返回值储存在变量里

示例:  
```C
ModuleType_t mod=SDK_CreateModule("HTTP","This is a http module",1);
```
**函数原型**  
```C
void SDK_AddBindPort(ModuleType_t*mod,int port,void (*callback)(int sock,struct sockaddr));
```
|参数|类型|含义|
|-|-|-|
|mod|ModuleType_t*|之前创建的模块信息结构体|
|callback|void (*callback)(int sock,struct sockaddr))|处理连接的回调函数|

函数作用：申请一个端口，并为该端口上的发生的连接设置回调函数处理  
  
函数用法：传入需要监听的端口，并传入处理连接的回调函数  
  
示例:
```C
//回调函数
void callback(int sock,struct sockaddr)
{
    char str[]="hello";
    send(sock,str,strlen(str)+1,0);
    close(sock);
}

//在_init_中的调用
SDK_AddBindPort(&mod,1234,callback);
```
**函数原型**  
```C
void SDK_AddFunction(ModuleType_t*mod,int function_run_time,void*function,unsigned char precedence);
```
|参数|类型|含义|
|-|-|-|
|mod|ModuleType_t*|之前创建的模块信息结构体|
|function_run_time|int|调用该函数的时段|
|function|void*|要调用的函数|
precedence|unsigned char|同一时段函数调用的优先级|

函数作用:设置某一时段调用的函数  
  
函数用法:传入模块信息结构体的地址，设置调用该函数的时段，时段可使用**宏定义**，并将被调用的函数指针传入，最后设置在该时段这个函数调用的优先级，值越小优先级越高，取值范围是0-127,同优先级调用顺序随机  
|宏定义|值|含义|
|-|-|-|
|RUN_AT_START|1|程序启动后|
|RUN_AT_BEFORE_THREAD_START|2|与客户端建立连接后|
|RUN_AT_AFTER_THREAD_START|3|客户连接处理函数启动后|

*三个时段即文档开头处描述的三个时段，参数及返回值详见开头处描述*  

示例:  
```C
//定义在与客户端建立连接后执行的函数
void func(int sock,struct sockaddr)
{
    printf("我在与客户端建立连接后被调用\n");
}

//在_init_中注册
SDK_AddFunction(&mod,RUN_AT_BEFORE_THREAD_START,func,10);
```

## 3.模块开发示例
详见example.c