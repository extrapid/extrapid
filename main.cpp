#include "module.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include "log.h"
#include "cJSON.h"
#include <unistd.h>
void StartThread();
char *LogFilePath = NULL;
char *ModuleFilePath = NULL;

void LoadConfig();
int main()
{

    LoadConfig();
    logInit(LogFilePath);
    extrapidLog(LOG_INFO, "MAIN", "开始加载模块");
    auto modules = ManageModules();
    modules->LoadModules(ModuleFilePath);
    modules->runStart();
    StartThread();
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    pthread_mutex_lock(&lock);
    return 0;
}

void LoadConfig()
{
    //检查文件是否存在
    if (access("conf/config.json", F_OK))
    {
        printf("配置文件不存在,尝试生成配置文件\n");
        mkdir("conf", 0777);
        FILE *fp = fopen("conf/config.json", "w");
        if (fp == NULL)
        {
            printf("生成conf/config.json失败:%s\n", strerror(errno));
            exit(1);
        }
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "LogPath", "./logs");
        cJSON_AddStringToObject(json, "ModulePath", "Modules");
        char *p = cJSON_Print(json);
        fwrite(p, strlen(p) + 1, 1, fp);
        fclose(fp);
        cJSON_Delete(json);
        printf("配置文件conf/config.json创建完成\n");
    }
    //加载配置文件
    FILE *fp = fopen("conf/config.json", "r");
    if (fp == NULL)
    {
        printf("读取配置文件conf/config.json失败，原因是:%s\n", strerror(errno));
        exit(1);
    }
    fseek(fp, 0L, SEEK_END);
    size_t filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    if (filesize == 0)
    {
        printf("配置文件异常，请检查配置文件");
        exit(1);
    }
    char *jsonstr = (char *)malloc(filesize);
    fread(jsonstr, filesize, 1, fp);
    cJSON *json = cJSON_Parse(jsonstr);
    free(jsonstr);
    if (json == NULL)
    {
        printf("配置文件异常，请检查配置文件");
        exit(1);
    }
    //读取log path
    cJSON *temp = cJSON_GetObjectItem(json, "LogPath");
    if (temp == NULL || temp->type != cJSON_String || temp->valuestring == NULL)
    {
        cJSON_Delete(json);
        printf("配置文件异常，请检查配置文件");
        exit(1);
    }
    else
    {
        LogFilePath = (char *)malloc(strlen(temp->valuestring) + 1);
        strcpy(LogFilePath, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(json, "ModulePath");
    if (temp == NULL || temp->type != cJSON_String || temp->valuestring == NULL)
    {
        cJSON_Delete(json);
        printf("配置文件异常，请检查配置文件");
        exit(1);
    }
    else
    {
        ModuleFilePath = (char *)malloc(strlen(temp->valuestring) + 1);
        strcpy(ModuleFilePath, temp->valuestring);
    }
    fclose(fp);
}
int folder_mkdirs(char *folder_path)
{
    if (!access(folder_path, F_OK))
    { /* 判断目标文件夹是否存在 */
        return 1;
    }

    char path[256];      /* 目标文件夹路径 */
    char *path_buf;      /* 目标文件夹路径指针 */
    char temp_path[256]; /* 存放临时文件夹路径 */
    char *temp;          /* 单级文件夹名称 */
    int temp_len;        /* 单级文件夹名称长度 */

    memset(path, 0, sizeof(path));
    memset(temp_path, 0, sizeof(temp_path));
    strcat(path, folder_path);
    path_buf = path;

    while ((temp = strsep(&path_buf, "/")) != NULL)
    { /* 拆分路径 */
        temp_len = strlen(temp);
        if (0 == temp_len)
        {
            continue;
        }
        strcat(temp_path, "/");
        strcat(temp_path, temp);
        printf("temp_path = %s\n", temp_path);
        if (-1 == access(temp_path, F_OK))
        { /* 不存在则创建 */
            if (-1 == mkdir(temp_path, 0777))
            {
                return 2;
            }
        }
    }
    return 0;
}
