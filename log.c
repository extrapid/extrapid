#include "log.h"
#include <malloc.h>
#include <string.h>
#include <unistd.h>
char *logPath;
char buf[1024];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static const char *levelColors[] = {"\033[94m", "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[35m", "\033[35m"};

static const char *levelStrings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

void logInit(const char *setLogPath)
{

    logPath = (char *)malloc(strlen(setLogPath) + 1);
    strcpy(logPath, setLogPath);
    if (access(setLogPath, F_OK))
    {
        if (mkdir(logPath, S_IRWXU) == -1)
        {
            printf("%s\n", strerror(errno));
        }
    }

    extrapidLog(LOG_INFO, "libLog", "Init libLog");
}

void extrapidLog(const int logLevel, const char *moduleName, const char *fmt, ...)
{
    pthread_mutex_lock(&lock);

    time_t timep;
    time(&timep);
    struct tm *nowTime;
    nowTime = localtime(&timep);

    char timeString[22] = {0};
    snprintf(timeString, 22, "[%d-%s%d-%s%d_%s%d:%s%d:%s%d]", 1900 + nowTime->tm_year, nowTime->tm_mon < 9 ? "0" : "", 1 + nowTime->tm_mon, nowTime->tm_mday < 10 ? "0" : "", nowTime->tm_mday, nowTime->tm_hour < 10 ? "0" : "", nowTime->tm_hour, nowTime->tm_min < 10 ? "0" : "", nowTime->tm_min, nowTime->tm_sec < 10 ? "0" : "", nowTime->tm_sec);

    char logTimeLevelLib[128] = {0};
    char logToFile[128] = {0};
    sprintf(logTimeLevelLib, "%s %s%-5s\033[0m[%s] ", timeString, levelColors[logLevel], levelStrings[logLevel], moduleName);
    sprintf(logToFile, "%s %-5s[%s] ", timeString, levelStrings[logLevel], moduleName);

    memset(buf, 0, sizeof(buf));

    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    printf("%s%s\n", logTimeLevelLib, buf);

    FILE *fp;
    char fileName[1024];
    sprintf(fileName, "%s/%d-%d-%d.log", logPath, 1900 + nowTime->tm_year, 1 + nowTime->tm_mon, nowTime->tm_mday);
    if ((fp = fopen(fileName, "a")) == NULL)
        printf("%s%s\n", logTimeLevelLib, strerror(errno));
    fprintf(fp, "%s%s\n", logToFile, buf);
    fclose(fp);

    pthread_mutex_unlock(&lock);
}
