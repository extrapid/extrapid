#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

void printChar(const char ch);
void printInt(const int dec);
void printStr(const char *ptr);


void init(char *logPath = "/tmp/extrapid");
void log(const int logLevel, const char *moduleName, const char *format, ...);