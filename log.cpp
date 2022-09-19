#include "log.h"

char *logPath;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static const char *levelColors[] = {"\033[94m", "\033[36m", "\033[32m",
                                    "\033[33m", "\033[31m", "\033[35m",
                                    "\033[35m"};

static const char *levelStrings[] = {"TRACE", "DEBUG", "INFO",  "WARN",
                                     "ERROR", "FATAL", "PLAYER"};

void printChar(const char ch) { putchar(ch); }

void printInt(const int dec) {
    if (dec == 0) {
        return;
    }
    printInt(dec / 10);
    putchar((char)(dec % 10 + '0'));
}

void printStr(const char *ptr) {
    while (*ptr) {
        putchar(*ptr);
        ptr++;
    }
}

void logInit(char *setLogPath = "/tmp/extrapid") {
    logPath = setLogPath;
    log(LOG_INFO, "libLog", "Init libLog");
}

void log(const int logLevel, const char *moduleName, const char *fmt, ...) {
    time_t timep;
    time(&timep);
    struct tm *nowTime;
    nowTime = localtime(&timep);
    char timeString[22] = {0};
    snprintf(timeString, 22, "[%d-%d-%d_%d:%d:%d]", 1900 + nowTime->tm_year,
             1 + nowTime->tm_mon, nowTime->tm_mday, nowTime->tm_hour,
             nowTime->tm_min, nowTime->tm_sec);

    char *logTimeLevelLib = {0};
    sprintf(logTimeLevelLib, "%s %s%-5s\033[0m[%s] ", timeString,
            levelColors[logLevel], levelStrings[logLevel], moduleName);

    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
            putchar(*fmt);
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
}

int main() {
    logInit();
    log(LOG_WARN, "123", "");
    return 0;
}