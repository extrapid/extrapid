#include "modulesdk.h"
#include <stdlib.h>
ModuleType_t SDK_CreateModule(const char *name, const char *description, int version)
{
    ModuleType_t m = {0};
    m.module_name = name;
    m.description = description;
    m.version = version;
    m.protocol = PROTOCOL;
    return m;
}
void SDK_AddBindPort(ModuleType_t *mod, int port, void (*callback)(int sock, struct sockaddr))
{
    BindList_t *temp = (BindList_t *)malloc(sizeof(BindList_t));
    temp->bindport.callback = callback;
    temp->bindport.port = port;
    temp->next = mod->bindlist_head;
    mod->bindlist_head=temp;
}
void SDK_AddFunction(ModuleType_t*mod,int function_run_time,void*function,unsigned char precedence)
{
    FunctionList_t*temp=(FunctionList_t*)malloc(sizeof(FunctionList_t));
    temp->func=function;
    temp->precedence=precedence;
    switch (function_run_time)
    {
    case RUN_AT_START:
        temp->next= mod->start.start;
        mod->start.start=temp;
        break;
    case RUN_AT_BEFORE_THREAD_START:
        temp->next= mod->start.before_connect_client;
        mod->start.before_connect_client=temp;
        break;
    case RUN_AT_AFTER_THREAD_START:
        temp->next= mod->start.after_connect_client;
        mod->start.after_connect_client=temp;
        break;
    default:
    free(temp);
        break;
    }
}
