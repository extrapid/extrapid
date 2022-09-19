#include "../moduletype.h"
#include<stdio.h>
#include<stdlib.h>
void func()
{
    printf("I'm running\n");
}

ModuleType_t _init_(int i)
{
    printf("i=%d\n",i);
    ModuleType_t a;
    a.bindlist_head=NULL;
    a.description="test";
    a.module_name="test";
    a.protocol=1;
    FunctionList_t*temp=(FunctionList_t*)malloc(sizeof(FunctionList_t));
    temp->func=func;
    temp->next=NULL;
    temp->precedence=10;
    a.start.start=temp;
    a.start.after_connect_client=NULL;
    a.start.before_connect_client=NULL;
    a.version=1;
    return a;
}
