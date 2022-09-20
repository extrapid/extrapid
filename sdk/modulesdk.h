#include "moduletype.h"
#include<sys/types.h>
#include<sys/socket.h>
#define RUN_AT_START 1
#define RUN_AT_BEFORE_THREAD_START 2
#define RUN_AT_AFTER_THREAD_START 3

ModuleType_t SDK_CreateModule(const char *name, const char *description,int version);
void SDK_AddBindPort(ModuleType_t*mod,int port,void (*callback)(int sock,struct sockaddr));
void SDK_AddFunction(ModuleType_t*mod,int function_run_time,void*function,unsigned char precedence);