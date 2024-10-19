/*
 * 文件名: LinuxEnv.c
 * 作者: ljk
 * 创建时间: 2023-07-25
 * 文件描述: 环境变量操作例程
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../base_common.h"
#include "linux_environ.h"




// getenv获取环境变量
int api_getenv(const char* path) {
    const char* env_var = path;
    char* value = getenv(env_var);
    
    if (value != NULL) {
        printf("Value of %s: %s\n", env_var, value);
    } else {
        printf("Environment variable %s not found.\n", env_var);
    }
    
    return 0;
}

// putenv设置环境变量
int api_putenv(const char* path, const char* value) {
    const char* env_var   = path;
    const char* in_value = value;

    char* env_str = (char*)malloc(strlen(env_var) + strlen(in_value) + 2); // +2 for '=' and null terminator
    if (env_str == NULL) {
        perror("Memory allocation error");
        return 1;
    }
    
    sprintf(env_str, "%s=%s", env_var, in_value);
    
    if (putenv((char*)env_str) != 0) {
        perror("Error setting environment variable");
        return 1;
    }
    
    char* env_value = getenv(env_var);
    
    if (env_value != NULL) {
        printf("Value of %s: %s\n", env_var, env_value);
    } else {
        printf("Environment variable %s not found.\n", env_var);
    }
    
    return 0;
}

// setenv设置环境变量 overwrite 非0：覆盖已经存在的环境变量值 0：不覆盖已经存在环境变量值
int api_setenv(const char* path, const char* value, int over) {
    const char* env_var  = path;
    const char* in_value = value;
    int overwrite = over;

    // 设置环境变量
    int ret = setenv(env_var, value, overwrite);

    if (ret != 0) {
        perror("Error setting environment variable");
        return 1;
    }

    // 获取环境变量并打印
    char* env_value = getenv(env_var);

    if (env_value != NULL) {
        printf("Value of %s: %s\n", env_var, env_value);
    } else {
        printf("Environment variable %s not found.\n", env_var);
    }

    return 0;
}

extern char** environ;
// environ 展示环境变量
int api_showenv() {
    char **env = environ;

    while(*env)
    {
        printf("%s\n",*env);
        env++;
    }
    return 0;
}

int fun_getenv() {
    return api_getenv("HOME");
}

int fun_putenv() {
    return api_putenv("MY_VARIABLE", "hello");
}

int fun_setenv() {
    return api_setenv("MY_VARIABLE", "ni hao!", 1);
}

int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(fun_getenv);
    FUNCTION_PRINT_API(fun_putenv);
    FUNCTION_PRINT_API(fun_setenv);
    FUNCTION_PRINT_API(api_showenv);
    return 0;
}

// 环境测试例程
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}