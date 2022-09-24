#include "module.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <list>
#include <string>
#include "etp_log/log.h"
enum CallbackType
{
    Start = 1,
    BeforeThreadStart,
    AfterThreadStart
};
Funcs* ModuleManager::GetFuncs()
{
    return &funcs;
}
std::list<BindList_t> ModuleManager::GetBinds()
{
    return binds;
}

void ModuleManager::LoadModules(const char *dir)
{
    extrapidLog(LOG_INFO, "LoadModules", "设定的模块目录为%s\n", dir);
    printf(">>开始搜索目录<<\n");
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        extrapidLog(LOG_ERROR, "LoadModules", "目录打开失败,尝试创建目录");
        mkdir(dir,0777);
        dp = opendir(dir);
        if (dir==NULL)
        {
            extrapidLog(LOG_ERROR, "LoadModules", "目录创建失败");
            return;
        }
    }
    struct dirent *dir_entry = NULL;
    //获取，目录下有多少个文件
    int filenum = 0;
    while (1)
    {
        dir_entry = readdir(dp);
        if (dir_entry == NULL)
        {
            break;
        }
        extrapidLog(LOG_INFO, "LoadModules", "发现文件%s", dir_entry->d_name);
        if (dir_entry->d_type == 8 && strstr(dir_entry->d_name, ".so") && strstr(dir_entry->d_name, "etp_module_"))
        {
            
	    extrapidLog(LOG_INFO, "LoadModules", "找到模块:%s", dir_entry->d_name);
            filenum++;
        }
    }
    //将目录指针返回开头
    seekdir(dp, 0);
    //定义一个二维数组存放文件名
    char filenames[filenum][128];
    int i = 0;
    //保存文件名
    while (1)
    {
        dir_entry = readdir(dp);
        if (dir_entry == NULL)
        {
            break;
        }
        if (dir_entry->d_type == 8 && strstr(dir_entry->d_name, ".so") && strstr(dir_entry->d_name, "etp_module_"))
        {
            strcpy(filenames[i], dir_entry->d_name);
            i++;
        }
    }
    closedir(dp);
    printf(">>目录搜索完成<<\n");
    //尝试注册并尝试创建链表
    if (filenum==0)
    {
        extrapidLog(LOG_INFO,"LoadModules","没有找到模块");
        return;
    }
    printf("\n>>开始注册模块<<\n");
    typedef ModuleType_t (*_init_)(int);
    char strtemp[256];
    for (int i = 0; i < filenum; i++)
    {
        sprintf(strtemp, "%s/%s", dir, filenames[i]);
        void *lib = dlopen(strtemp, RTLD_LAZY);
        if (lib == NULL)
        {
            extrapidLog(LOG_ERROR, "LoadModules", "打开%s错误,错误原因是:%s", strtemp, dlerror());
            continue;
        }
        _init_ init = (_init_)dlsym(lib, "_init_");
        if (init == NULL)
        {
            extrapidLog(LOG_WARN, "LoadModules", "模块:%s无法注册，原因是%s，你可以尝试与开发者联系", filenames[i], dlerror());
            dlclose(lib);
        }
        else
        {

            ModuleType_t md = init(1);
            modules.push_back(md);
            FunctionList_t *fctemp = md.start.start;
            int funcnum = 0;
            while (fctemp) //注册start函数列表
            {
                funcs.start.push_back(*fctemp);
                fctemp = fctemp->next;
                funcnum++;
            }
            fctemp = md.start.before_connect_client;
            while (fctemp) //注册before函数
            {
                funcs.before_thread_start.push_back(*fctemp);
                fctemp = fctemp->next;
                funcnum++;
            }
            fctemp = md.start.after_connect_client;
            while (fctemp) //注册after函数
            {
                funcs.before_thread_start.push_back(*fctemp);
                fctemp = fctemp->next;
                funcnum++;
            }
            //注册端口回调
            BindList_t *bltemp = md.bindlist_head;
            BindList_t *blstack;
            int blnum = 0;
            while (bltemp)
            {
                binds.push_back(*bltemp);
                bltemp = bltemp->next;
                blnum += 1;
            }
            extrapidLog(LOG_INFO, "LoadModules", "模块%s注册成功，包含%d个函数，需要%d个端口\n", filenames[i], funcnum, blnum);
        }
    }
    auto compare_func=[](FunctionList_t a, FunctionList_t b)
                    { return a.precedence < b.precedence; };

    funcs.start.sort(compare_func);
    funcs.before_thread_start.sort(compare_func);
    funcs.after_thread_start.sort(compare_func);
    printf(">>模块注册完成<<\n");

    return;
}
void ModuleManager::runStart()
{
    typedef void (*f)(void);
    for (auto it:funcs.start)
    {
        ((f)it.func)();
    }
}
void ModuleManager::runBeforeThreadStared(int sock,struct sockaddr addr)
{
    typedef void (*f)(int,struct sockaddr);
    for (auto it:funcs.before_thread_start)
    {
        ((f)it.func)(sock,addr);
    }
}
void ModuleManager::runAfterThreadStared(int sock,struct sockaddr addr,pthread_t tid)
{
    typedef void (*f)(int,struct sockaddr,pthread_t);
    for (auto it:funcs.before_thread_start)
    {
        ((f)it.func)(sock,addr,tid);
    }
}
void ModuleManager::ListModules()
{
    printf("========列出模块列表========\n");
    for (auto it :modules)
    {
        printf("%s\n",it.module_name);
    }
    printf("==========列出完成==========\n");
}

ModuleManager modules;
ModuleManager *ManageModules();
ModuleManager *ManageModules()
{
    return &modules;
}
