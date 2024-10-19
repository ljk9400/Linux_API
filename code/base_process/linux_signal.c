/*
 * 文件名: LinuxSignal.c
 * 作者: ljk
 * 创建时间: 2023-08-04
 * 文件描述: 信号操作例程
 */
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "linux_signal.h"



static int exit_cnt = 0;
void signal_handler(int signum)
{
    printf("Recevice signal %d\n", signum);
    exit_cnt = 1;
}

void alarm_handler(int signum) 
{
    printf("Alarm signal: %d\n", signum);
    exit_cnt = 2;
}

void kill_handler(int signum) 
{
    printf("Kill signal: %d\n", signum);
    exit_cnt = 3;
}

void loop_exit()
{
    while (1) {
        printf("Waiting for signal... %d\n", exit_cnt);
        if(exit_cnt){
            break;
        }
        sleep(1);
    }
    printf("loop exit\n");
}

// 信号测试
int fun_signal()
{
    // 设置 SIGINT 的处理函数为 signal_handler
    signal(SIGINT, signal_handler);
    // loop_exit();
    return 0;
} 

// 信号测试
int fun_alarm()
{
    // 设置 SIGALRM 的处理函数为 alarm_handler
    signal(SIGALRM, alarm_handler);

    // 设置定时器，5 秒后触发 SIGALRM 信号
    alarm(10);

    //loop_exit();
    return 0;
} 

// 信号测试
int fun_kill()
{
    pid_t pid;
    pid = fork();

    switch(pid)
    {
    case -1:
        perror("fork 失败");
        exit(1);
    case 0:
        // 设置 SIGALRM 的处理函数为 alarm_handler
        signal(SIGALRM, kill_handler);
        pause();
        if(exit_cnt){
            printf("Waiting for signal... %d\n", exit_cnt);
        }
        printf("子进程退出\n");
        exit(0);
    default:
        sleep(2);
        // 父进程向子进程发送信号
        printf("父进程向子进程(%d)发送信号\n", pid);
        kill(pid, SIGALRM);
        // 防止僵尸进程
        int status;
        pid_t child_pid = wait(&status);
        if (child_pid == -1) {
            perror("wait");
            return 1;
        }
        if (WIFEXITED(status)) {
            printf("Child process (PID=%d) exited with status: %d\n", child_pid, WEXITSTATUS(status));
        }
    }

    return 0;
} 

void ouch_handler(int signum) 
{
    printf("ouch signal: %d\n", signum);
    exit_cnt = 4;
}

// 信号测试
int fun_sigaction()
{
    struct sigaction act;
    act.sa_handler = ouch_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    // 检查替代原先信号，替代执行
    sigaction(SIGINT, &act, 0);
    //loop_exit();

    return 0;
}

// 打印信号
void print_sigset(const char* label, sigset_t* set) 
{
    printf("%s: ", label);
    for (int i = 1; i < NSIG; i++) {
        if (sigismember(set, i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

// 信号设置
int fun_sigset()
{
    // 定义并初始化信号集
    sigset_t signal_set;
    sigemptyset(&signal_set); // 初始化为空信号集

    // 添加信号到信号集
    sigaddset(&signal_set, SIGINT);
    sigaddset(&signal_set, SIGTERM);

    // 打印信号集
    print_sigset("Signal Set", &signal_set);

    // 删除信号从信号集
    sigdelset(&signal_set, SIGINT);

    // 打印修改后的信号集
    print_sigset("Modified Signal Set", &signal_set);

    // 填充所有信号到信号集
    sigfillset(&signal_set);

    // 打印填充后的信号集
    print_sigset("Filled Signal Set", &signal_set);
    return 0;
}

// 信号设置
int fun_sighandle()
{
    // 设置信号处理函数
    signal(SIGINT, signal_handler);

    // 设置信号掩码，屏蔽 SIGINT 信号
    sigset_t old_set;
    sigemptyset(&old_set);
    sigaddset(&old_set, SIGINT);
    sigprocmask(SIG_BLOCK, &old_set, NULL);

    // 进程挂起，等待信号
    printf("Waiting for signal...\n");
    sigset_t pending_set;
    sigpending(&pending_set);
    print_sigset("Pending signals set", &pending_set);

    // 模拟进程的工作
    sleep(5);

    // 解除信号掩码，允许 SIGINT 信号传递给进程
    sigprocmask(SIG_UNBLOCK, &old_set, NULL);

    // 挂起进程直到接收到 SIGINT 信号
    printf("Waiting for signal (suspended)...\n");
    sigset_t suspend_set;
    sigemptyset(&suspend_set);
    sigsuspend(&suspend_set);
    printf("Resumed after receiving signal.\n");
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
    function_print("fun_kill",      fun_kill);
    function_print("fun_signal",    fun_signal);
    function_print("fun_alarm",     fun_alarm);
    function_print("fun_sigaction", fun_sigaction);
    function_print("loop_exit",  loop_exit);
    function_print("fun_sigset", fun_sigset);
    function_print("fun_sighandle", fun_sighandle);
    return 0;
}

// 测试程序
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}