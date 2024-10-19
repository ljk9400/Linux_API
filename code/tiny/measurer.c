/*
 * 文件名: measurer.c
 * 作者: ljk
 * 创建时间: 2023-07-26
 * 文件描述: 测量操作例程
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>



// 测量函数执行时间
void measure_time(int (*func)()) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    func();
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Function execution time: %f seconds\n", cpu_time_used);
}

// 获取当前进程 CPU 使用率
double get_cpu_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1000000.0;
}

// 获取当前进程内存使用率
long get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // 返回最大驻留集大小（以KB为单位）
}

// 获取当前 GPU 使用率（todo）
double get_gpu_usage() {
    // 这里应该调用实际的 GPU 使用率获取函数
    // 例如：NVIDIA GPU 可以使用 NVIDIA Management Library (NVML)
    // 这里只是一个模拟返回值
    return 0.0;
}

// 获取当前进程线程数量
int get_thread_count() {
    int count = 0;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/task", getpid());
    DIR *dir = opendir(path);
    if (dir) {
        while (readdir(dir)) {
            count++;
        }
        closedir(dir);
    }
    // 减去 "." 和 ".." 目录
    return count - 2;
}

// 获取当前进程线程名称
void get_thread_names() {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/task", getpid());
    DIR *dir = opendir(path);
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                char thread_name_path[512];
                snprintf(thread_name_path, sizeof(thread_name_path), "/proc/%d/task/%s/comm", getpid(), entry->d_name);
                FILE *file = fopen(thread_name_path, "r");
                if (file) {
                    char thread_name[256];
                    if (fgets(thread_name, sizeof(thread_name), file)) {
                        printf("Thread ID: %s, Name: %s", entry->d_name, thread_name);
                    }
                    fclose(file);
                }
            }
        }
        closedir(dir);
    }
}

// 原始函数
int fun_measurer() {
    printf("sleep 2s\n");
    sleep(1);
    printf("CPU usage: %f seconds\n", get_cpu_usage());
    printf("MEM usage: %ld KB\n", get_memory_usage());
    printf("GPU usage: %f%%\n", get_gpu_usage());
    get_thread_count();
    get_thread_names();
    sleep(1);
    return 1;
}

int main(int argc, char* argv[]) {
    measure_time(fun_measurer);
    return 0;
}