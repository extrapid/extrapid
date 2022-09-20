#ifndef MODULETYPE
#define MODULETYPE
#include <sys/socket.h>
#define PROTOCOL 1
typedef struct FUNCTIONLIST
{
    unsigned char precedence;
    void *func;
    struct FUNCTIONLIST *next;
} FunctionList_t;
typedef struct
{
    FunctionList_t *start;
    FunctionList_t *before_connect_client;
    FunctionList_t *after_connect_client;
} funcs_t;
typedef struct
{
    int port;
    void (*callback)(int sock, struct sockaddr);
} BindPort_t;
typedef struct BINDLIST
{
    BindPort_t bindport;
    struct BINDLIST*next;
} BindList_t;
typedef struct MODULET
{
    long protocol;
    char *module_name;
    char *description;
    long version;
    funcs_t start;
    BindList_t*bindlist_head;
    struct MODULET*next;
} ModuleType_t;

#endif