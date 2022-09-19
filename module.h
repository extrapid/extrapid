#ifndef MODULE
#define MODULE
#include "moduletype.h"
#include<list>

struct Funcs
{
    std::list<FunctionList_t> start;
    std::list<FunctionList_t> before_thread_start;
    std::list<FunctionList_t> after_thread_start;
};
typedef class MODULEMANAGER{
    private:
    struct Funcs funcs;
    std::list<ModuleType_t> modules;
    std::list<BindList_t> binds;
    public:
    void LoadModules(const char*dir);
    Funcs* GetFuncs();
    std::list<BindList_t> GetBinds();
    void runStart();
    void runBeforeThreadStared(int sock,struct sockaddr);
    void runAfterThreadStared(int sock,struct sockaddr,pthread_t);
} ModuleManager;
ModuleManager* ManageModules();
typedef void (*CallbackStart)(void);
typedef void (*CallbackBeforeThreadStart)(int sock,struct sockaddr);
typedef void (*CallbackAfterThreadStart)(int sock,struct sockaddr,pthread_t);

#endif