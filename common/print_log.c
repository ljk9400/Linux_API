/*
 * 文件名: LinuxLog.c
 * 作者: ljk
 * 创建时间: 2023-07-25
 * 文件描述: va_list用法
 */
#include <stdarg.h>
#include <stdio.h>
// 禁用 assert,必须放<assert.h>前
#define NDEBUG
#include <string.h> 
#include <assert.h>
#include <errno.h>
#include <stdio.h>

// 1、宏定义打印
#define __DEBUG__
#ifdef __DEBUG__
    #define DPRINTF(...) printf(__VA_ARGS__)  // 宏打印函数定义 
#else
    #define DPRINTF(...)
#endif

// 打印
void log_print(const char *msg, ...) {
    #define LOG_BUF_SIZE           (1024)
    va_list ap;    
    char message[LOG_BUF_SIZE] = {0}; 
    va_start(ap, msg);  
    vsnprintf(message, LOG_BUF_SIZE, msg, ap);  
    va_end(ap);
    fprintf(stdout, "%s", message);
    fprintf(stderr, "%s", message);
}

// 打印文件，函数，行号
int log_debug(char* info) {
    printf("%s: %s %s line %d\n", info, __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

// 获取错误码, 全局变量errno
int api_get_err_code() {
    printf("Error code: %d\n", errno);
    return errno;
}

// 打印错误信息, strerror
int api_print_err_code() {
    int  errnum = errno;  
    char *error_msg = strerror(errnum);
    printf("Error code: %d, message: %s\n", errnum, error_msg);
    return 0;
}

// 打印错误信息, perror
int print_err_info() {
    perror("Error message");
    return 0;
}

// 断言测试assert
int api_assert(int a) {
    printf("assert test\n");
    // 断言 b 不为 0
    assert(a != 0);
    printf("input:%d\n", a);
    return 0;
}

int main(int argc, char* argv[]) {
    log_debug("log_debug test");
    log_print("sdd:", 1314, "\n");
    DPRINTF("Debug: %d\n", 520);
    FILE *file = fopen("nonexistent.txt", "r");
    if (file == NULL) {
        int err = api_get_err_code();
        api_print_err_code();
        print_err_info();
    }
    api_assert(1);
    api_assert(0);
}