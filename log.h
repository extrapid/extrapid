#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

void printChar(const char ch);
void printInt(const int dec);
void printStr(const char *ptr);

#ifdef __cplusplus
extern "C"{
#endif
void logInit(char *logPath);
void extrapidLog(const int logLevel, const char *moduleName, const char *fmt, ...);
#ifdef __cplusplus
}
#endif
