
#include"threadcontrol.h"
#include<stdlib.h>
pthread_mutex_t ThreadLinkLock=PTHREAD_MUTEX_INITIALIZER;
TC_ThreadLink_t *head;

TC_ThreadLink_t* TC_GetThreadLink(TC_ThreadID_t tid);
void TC_AddThreadLink(TC_ThreadLink_t threadlink);
int TC_RemoveThreadLink(TC_ThreadLink_t*threadlink);
int TC_CreateThread(void *(*thread_func)(void*), void *argv);

int TC_RemoveThreadLink(TC_ThreadLink_t*threadlink)
{
    TC_ThreadLink_t *temp=head;
    while (temp!=NULL)
    {
        if (threadlink==temp)
        {
            if (temp->prev!=NULL)
            {
                temp->prev->next=temp->next;
            }
            else
            {
                head=temp->next;
            }
            if(temp->next!=NULL)
            {
                temp->next->prev=temp->prev;
            }
            free(temp);
            return 0;
        }
        temp=temp->next;
    }
    return -1;
}
void TC_AddThreadLink(TC_ThreadLink_t threadlink)
{
    TC_ThreadLink_t*temp=(TC_ThreadLink_t*)malloc(sizeof(TC_ThreadLink_t));
    *temp=threadlink;
    temp->next=head;
    head->prev=temp;
    temp->prev=NULL;
    head=temp;
}

TC_ThreadLink_t* TC_GetThreadLink(TC_ThreadID_t tid)
{
    TC_ThreadLink_t *temp=head;
    while (temp!=NULL)
    {
        if (temp->tid==tid)
        {
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}
int TC_CreateThread(void *(*thread_func)(void*), void *argv)
{
    //为线程创建资源
    
    TC_ThreadLink_t temp;
    temp.files=NULL;
    temp.isrun=0;
    temp.mempool=ML_CreateMemPool(1024*10);
    temp.start_time=time(NULL);
    temp.detached=0;
    temp.ret=NULL;
    pthread_mutex_lock(&ThreadLinkLock);
    TC_AddThreadLink(temp);
    pthread_create(&(temp.tid),NULL,thread_func,argv);
}
