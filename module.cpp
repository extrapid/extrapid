#include "module.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <stdlib.h>
funcs_t FuncHead = {0};
BindList_t *BindHead = NULL;
ModuleType_t *ModuleHead = NULL;

int M_LoadModules(const char *dir);
BindList_t *M_GetBindList();

BindList_t *M_GetBindList()
{
    return BindHead;
}

int M_LoadModules(const char *dir)
{
    printf("[log]设定的模块目录为%s\n", dir);
    printf("\n>>开始搜索目录<<\n");
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        perror("[WARNING]目录打开失败");
        return NULL;
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
        printf("[log]发现文件%s\n", dir_entry->d_name);
        if (dir_entry->d_type == 8 && strstr(dir_entry->d_name, ".so"))
        {
            printf("[log]找到模块:%s\n", dir_entry->d_name);
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
        if (dir_entry->d_type == 8 && strstr(dir_entry->d_name, ".so"))
        {
            strcpy(filenames[i], dir_entry->d_name);
            i++;
        }
    }
    closedir(dp);

    //尝试注册并尝试创建链表
    printf("\n>>开始注册模块<<\n");
    typedef ModuleType_t (*_init_)(int);
    char strtemp[256];
    for (int i = 0; i < filenum; i++)
    {
        sprintf(strtemp, "%s/%s", dir, filenames[i]);
        void *lib = dlopen(strtemp, RTLD_LAZY);
        if (lib == NULL)
        {
            printf("[WARNING]打开%s错误,错误原因是:%s\n", strtemp, dlerror());
            continue;
        }
        _init_ init = (_init_)dlsym(lib, "_init_");
        if (init == NULL)
        {
            printf("[WARNING] 模块:%s无法注册，原因是%s，你可以尝试与开发者联系\n", filenames[i], dlerror());
            dlclose(lib);
        }
        else
        {

            ModuleType_t md = init(1);
            ModuleType_t *temp = (ModuleType_t *)malloc(sizeof(ModuleType_t));
            *temp = md;
            temp->next = ModuleHead;
            ModuleHead = temp;
            FunctionList_t *fctemp = md.start->start;
            int funcnum=0;
            while (fctemp) //注册start函数列表
            {
                FunctionList_t *fcstack = (FunctionList_t *)malloc(sizeof(FunctionList_t));
                *fcstack = *fctemp;
                fcstack->next = FuncHead.start;
                FuncHead.start = fcstack;
                fctemp = fctemp->next;
                funcnum+=1;
            }
            fctemp = md.start->before_connect_client;
            while (fctemp) //注册before函数
            {
                FunctionList_t *fcstack = (FunctionList_t *)malloc(sizeof(FunctionList_t));
                *fcstack = *fctemp;
                fcstack->next = FuncHead.before_connect_client;
                FuncHead.before_connect_client = fcstack;
                fctemp = fctemp->next;
                funcnum+=1;
            }
            fctemp = md.start->after_connect_client;
            while (fctemp) //注册after函数
            {
                FunctionList_t *fcstack = (FunctionList_t *)malloc(sizeof(FunctionList_t));
                *fcstack = *fctemp;
                fcstack->next = FuncHead.before_connect_client;
                FuncHead.before_connect_client = fcstack;
                fctemp = fctemp->next;
                funcnum+=1;
            }
            //注册端口回调
            BindList_t *bltemp = md.bindlist_head;
            BindList_t *blstack;
            int blnum=0;
            while (bltemp)
            {
                blstack=(BindList_t*)malloc(sizeof(BindList_t));
                *blstack = *bltemp;
                blstack->next=BindHead;
                BindHead=blstack;
                bltemp=bltemp->next;
                blnum+=1;
            }
            printf("模块%s注册成功，包含%d个函数，需要%d个端口\n",filenames[i],funcnum,blnum);
        }
    }
    putchar('\n');
    return 0;
}
