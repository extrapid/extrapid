#ifndef THREADCONTROL
#define THREADCONTROL
#include "threadcontrol.h"
#include "mempool.h"
#include <pthread.h>
#include <time.h>
#include <stdio.h>
typedef pthread_t TC_ThreadID_t;
struct FileLink {
    FILE*fp;
    struct FileLink*prev;
    struct FileLink*next;
};

typedef struct ThreadLink
{
    ML_Pool_t mempool;       //线程的内存池资源
    pthread_t tid; //线程的tid
    time_t start_time;       //启动时间
    struct FileLink*files;
    char isrun;              //是否正在运行
    char detached;
    void*ret;//返回值
    pthread_mutex_t lock;//线程锁
    struct ThreadLink *prev;
    struct ThreadLink *next;
} TC_ThreadLink_t;

#endif