/*
 * 文件名: LinuxSyslog.c
 * 作者: ljk
 * 创建时间: 2023-07-31
 * 文件描述: 资源获取测试操作例程
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../base_common.h"
#include "linux_resource.h"




// getpriority 获取进程或进程组的调度优先级
int api_getpriority() {
    int pid = getpid();
    // 获取当前进程的优先级
    int current_priority = getpriority(PRIO_PROCESS, pid);
    printf("当前进程的优先级：%d\n", current_priority);

    // 设置当前进程的优先级为较高的值
    int new_priority = current_priority - 5;
    if (setpriority(PRIO_PROCESS, pid, new_priority) == 0) {
        printf("已将当前进程的优先级设置为：%d\n", new_priority);
    } else {
        perror("设置进程优先级失败");
        return 1;
    }
    return 0;
}

// getrlimit 获取进程的资源限制
int api_getrlimit() {
    struct rlimit rlim;
    // 获取当前进程的数据段大小限制
    if (getrlimit(RLIMIT_DATA, &rlim) == 0) 
    {
        printf("当前进程的数据段大小限制: soft=%ld, hard=%ld\n", rlim.rlim_cur, rlim.rlim_max);
    } else {
        perror("获取数据段大小限制失败");
        return 1;
    }

    // 设置数据段大小限制
    rlim.rlim_cur = 1024 * 1024;  // 设置软限制为 1 MB
    rlim.rlim_max = 2048 * 1024;  // 设置硬限制为 2 MB
    if (setrlimit(RLIMIT_DATA, &rlim) == 0) 
    {
        printf("已设置当前进程的数据段大小限制: soft=%ld, hard=%ld\n", rlim.rlim_cur, rlim.rlim_max);
    } else {
        perror("设置数据段大小限制失败");
        return 1;
    }
    return 0;
}

// getrusage 获取进程资源使用情况
int api_getrusage() {
    struct rusage usage;
    // 获取当前进程的资源使用情况
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("用户态运行时间：%ld 秒 %ld 微秒\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("内核态运行时间：%ld 秒 %ld 微秒\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
        printf("最大的常驻集大小：%ld KB\n", usage.ru_maxrss);
        printf("次缺页错误：%ld\n", usage.ru_minflt);
        printf("主缺页错误：%ld\n", usage.ru_majflt);
        printf("从文件系统读入的块数：%ld\n", usage.ru_inblock);
        printf("输出到文件系统的块数：%ld\n", usage.ru_oublock);
    } else {
        perror("获取资源使用情况失败");
        return 1;
    }
    return 0;
}

int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(api_getpriority);
    FUNCTION_PRINT_API(api_getrlimit);
    FUNCTION_PRINT_API(api_getrusage);
    return 0;
}

// 环境测试例程
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}