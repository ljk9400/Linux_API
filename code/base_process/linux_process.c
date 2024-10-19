/*
 * 文件名: LinuxProcess.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程操作例程
 */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include "linux_process.h"



// 父进程结束后的，子进程形成僵尸进程会把自己的父进程设为PID=1的init进程，等待init进程回收
// 创建子进程测试
int fun_fork()
{
    pid_t pid;      // 用于存储 fork 返回值，表示进程 ID
    pid = fork();   // 创建子进程

    if (pid < 0)    // 创建子进程失败
    {
        fprintf(stderr, "Fork failed.\n");
        return 1;
    } 
    else if (pid == 0)   // 子进程
    {
        printf("Hello from the child process! (Child PID: %d, Parent PID: %d)\n", getpid(), getppid());
    } 
    else  // 父进程
    {
        printf("Hello from the parent process! (Child PID: %d)\n", pid);
        wait(NULL); // 等待子进程结束, 不然会形成僵尸进程
        printf("Child process has terminated.\n");
    }

    printf("fork hello...\n");
    /*
    *   这里的代码将被父进程和子进程共同执行
    */
    
    return 0;
}

// 封装获取线程ID
pid_t gettid(void)
{
    return syscall(SYS_gettid);
}

// 获取线程、进程ID
int fun_getID()
{
    pid_t pid  = getpid();  // 进程ID
    pid_t ppid = getppid(); // 父进程ID
    pid_t pgid = getpgrp(); // 进程组ID
    pid_t tid  = gettid();  // 线程ID
    
    printf("进程ID:   %d\n", pid);
    printf("父进程ID: %d\n", ppid);
    printf("进程组ID: %d\n", pgid);
    printf("线程ID:  %d\n", tid);
    printf("主线程ID: %d\n", pid); // 主线程 ID 就是进程 ID
    return 0;
}

// 函数调试信息打印
typedef int(*FunctionCallback)();
int function_print(char* name, void* callback)
{
    int ret = 0;
    printf("{=====[%s()] test start=====\n", name);
    FunctionCallback func = (FunctionCallback)callback;
    ret = func();
    printf("------[%s()] test end-------}\n\n", name);
    return ret;
}


int main_test(int argc, char* argv[])
{
    printf("输入的命令行参数个数为: %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("参数 %d: %s\n", i, argv[i]);
    }
    function_print("fun_fork",  fun_fork);
    function_print("fun_getID", fun_getID);
    return 0;
}

// 测试程序
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}