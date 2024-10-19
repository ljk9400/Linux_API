/*
 * 文件名: LinuxTimeOS.c
 * 作者: ljk
 * 创建时间: 2023-07-27
 * 文件描述: 
 */
//含了 <time.h> 中的所有内容，并增加了对更精确时间测量和定时功能的支持。
#include <sys/time.h>
#include <unistd.h> // 包含这个头文件来声明 sleep 和 usleep 函数
#include <stdlib.h>
#include <stdio.h>
#include "../base_common.h"
#include "linux_time_os.h"




// gettimeofday获取当前时间
int api_gettimeofday() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0) 
    {
        printf("Seconds: %ld\n", tv.tv_sec);
        printf("Microseconds: %ld\n", tv.tv_usec);
    } 
    return 0;
}

// 获取当前系统允许时间
int api_uptime() {
    long long _ltime = 0;
    char szline[1024] = {0};
    FILE* fp = fopen("/proc/uptime", "r");//打开系统文件查看
	if(!fp) {
		perror("fopen /proc/uptime");
		return -1;
	}
	// 获取运行时间和空闲时间
	fgets(szline, sizeof(szline), fp);
    fclose(fp);
    _ltime = atoi(szline);
    printf("/proc/uptime:%s %lld\n", szline, _ltime);
    return 0;
}

// 延时秒
int api_delay_s(int seconds) {
    sleep(seconds);
    return 0;
}

// 延时微秒
int api_delay_us(int microseconds) {
    usleep(microseconds);
    return 0;
}

#include <time.h>
// 延时纳秒
int api_delay_ns(int cnt) {
    struct timespec sleep_time;
    sleep_time.tv_sec  = cnt / 1000000000;
    sleep_time.tv_nsec = cnt % 1000000000;

    struct timespec remaining_time;
    int result = nanosleep(&sleep_time, &remaining_time);
    while (result != 0) {
        // 如果 nanosleep 被中断，使用剩余的时间再次休眠
        sleep_time = remaining_time;
        result = nanosleep(&sleep_time, &remaining_time);
    }
    return 0;
}

int fun_sleep() {
    int time_ns = 1000000000;
    int t_us = time_ns/1000;
    int t_s = t_us/1000/1000;
    printf("sleep %d\n", t_s);
    api_delay_s(t_s);
    printf("usleep %dus\n", t_us);
    api_delay_us(t_us);
    printf("nslepp %dns\n", time_ns);
    api_delay_ns(time_ns);
    return 0;
}

int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(api_gettimeofday);
    FUNCTION_PRINT_API(api_uptime);
    FUNCTION_PRINT_API(fun_sleep);
    return 0;
}

// 测试例程
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}