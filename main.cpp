#include "module.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include "log.h"
void StartThread();
int main()
{
    
    logInit("./log");
    extrapidLog(LOG_INFO,"MAIN","开始加载模块");
    auto modules=ManageModules();
    modules->LoadModules(".");
    modules->runStart();
    StartThread();
    pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);
    return 0;
}
