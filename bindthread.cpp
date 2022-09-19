#include "module.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include <fcntl.h>
struct CONNECT{
    int sock;
    struct sockaddr addr;
    BindList_t *bindlist;
};
void*DealClient(void*argv);
void*thread(void*argv);
void*DealClient(void*argv)
{
    struct CONNECT temp=*((struct CONNECT*)(argv));
    free(argv);
    temp.bindlist->bindport.callback(temp.sock,temp.addr);
    pthread_exit(NULL);
}
void*thread(void*argv)
{
    struct CONNECT c=*((struct CONNECT*)(argv));
    free(argv);
    //设置监听
    listen(c.sock,5);
    //等待连接
    
    while (1)
    {
        struct CONNECT*temp=(struct CONNECT*)malloc(sizeof(struct CONNECT));
        socklen_t addr_len=sizeof(struct sockaddr);
        temp->sock=accept(c.sock,&(temp->addr),&addr_len);
        if (temp->sock==-1)
        {
            //建立连接失败
            printf("[ERROR] 建立连接失败:%s\n",strerror(errno));
            free(temp);
            continue;
        }
        //在这里模块处理连接

        //
        //创建新线程运行回调函数
        pthread_t tid;
        if (0!=pthread_create(&tid,NULL,DealClient,temp))
        {
            printf("[ERROR] 客户端处理线程创建失败\n");
            close(temp->sock);
            free(temp);
            continue;
        }
        pthread_detach(tid);

    }
    

}
void StartThread()
{
    BindList_t*temp=M_GetBindList();
    for (temp;temp;temp=temp->next)
    {
        pthread_t tid;
        struct CONNECT*c=(struct CONNECT*)malloc(sizeof(struct CONNECT));
        memset(c,0,sizeof(struct CONNECT));
        c->sock=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
        if ( 0>c->sock)
        {
            printf("[ERROR] 套接字创建失败:%s\n",strerror(errno));
            exit(1);
        }
        struct sockaddr_in addrin={0};
        addrin.sin_addr.s_addr=htonl(INADDR_ANY);
        addrin.sin_family=AF_INET;
        addrin.sin_port=htons(temp->bindport.port);
        if (bind(c->sock,(struct sockaddr*)(&addrin),sizeof(addrin))<0)
        {
            printf("[ERROR] 套接字绑定失败:%s\n",strerror(errno));
            exit(1);
        }
        c->bindlist=temp;
        if (0!=pthread_create(&tid,NULL,thread,&c))
        {
            printf("[ERROR] 创建监听线程失败\n");
            free(c);
            exit(1);
        }
        pthread_detach(tid);
    }
}