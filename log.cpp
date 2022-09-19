#include "log.h"

char *logPath;
char buf[1024];
int pos=0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static const char *levelColors[] = {"\033[94m", "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[35m", "\033[35m"};

static const char *levelStrings[] = {"TRACE", "DEBUG", "INFO",  "WARN","ERROR", "FATAL"};

void printChar(const char ch) {
    buf[pos++] = ch;
}

void printInt(const int dec) {
    if (dec == 0) return;
    printInt(dec / 10);
    buf[pos++] = (char)(dec % 10 + '0');
}

void printStr(const char *ptr) {
    while (*ptr) {
	buf[pos++] = *ptr;
	ptr++;
    }
}

void logInit(char *setLogPath = "/tmp/extrapid") {
    logPath = setLogPath;
    log(LOG_INFO, "libLog", "Init libLog");
}

void log(const int logLevel, const char *moduleName, const char *fmt, ...) {
    pthread_mutex_lock(&lock);

    time_t timep;
    time(&timep);
    struct tm *nowTime;
    nowTime = localtime(&timep);

    char timeString[22] = {0};
    snprintf(timeString, 22, "[%d-%d-%d_%d:%d:%d]", 1900 + nowTime->tm_year, 1 + nowTime->tm_mon, nowTime->tm_mday, nowTime->tm_hour, nowTime->tm_min, nowTime->tm_sec);

    char logTimeLevelLib[128] = {0};
    sprintf(logTimeLevelLib, "%s %s%-5s\033[0m[%s] ", timeString, levelColors[logLevel], levelStrings[logLevel], moduleName);

    memset(buf,0,sizeof(buf));pos=0;

    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
	    buf[pos++] = *fmt;
            fmt++;
        } else {
            fmt++;
            switch (*fmt) {
            case 'c': {
                char valChar = va_arg(ap, int);
                printChar(valChar);
                fmt++;
                break;
            }
            case 'd': {
                int valInt = va_arg(ap, int);
                printInt(valInt);
                fmt++;
                break;
            }
            case 's': {
                char *valStr = va_arg(ap, char *);
                printStr(valStr);
                fmt++;
                break;
            }
            default: {
                printChar(*fmt);
                fmt++;
            }
            }
        }
    }
    va_end(ap);
    
    printf("%s%s\n", logTimeLevelLib, buf);
    
    FILE *fp;
    char fileName[14];
    snprintf(fileName, 14, "%d-%d-%d.log", 1900 + nowTime->tm_year, 1 + nowTime->tm_mon, nowTime->tm_mday);
    if((fp = fopen(fileName, "a")) == NULL)
	printf("%s%s\n", logTimeLevelLib, strerror(errno));
    fprintf(fp, "%s%s\n", logTimeLevelLib, buf);

    pthread_mutex_unlock(&lock);
}

int main() {
    logInit(".");
    log(LOG_WARN, "123", "hello");
    return 0;
}
