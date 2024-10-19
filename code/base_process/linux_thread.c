/*
 * 文件名: LinuxProcess.c
 * 作者: ljk
 * 创建时间: 2023-08-03
 * 文件描述: 线程操作例程
 */
#include <sys/syscall.h> // 添加这行头文件以支持 syscall 函数和 SYS_gettid 常量
#include <unistd.h>      // 添加这行头文件以支持 sleep 函数
#include <stdlib.h>      // 添加这行头文件以支持 exit 函数
#include <pthread.h>     // 添加这行头文件以支持 pthread 相关函数
#include <stdio.h>
#include "linux_thread.h"

/*
* 1、线程创建必须有主线程回收或者自己分离式退出释放资源，不然最多创建32767个线程。
* 2、pthread_detach(pthread_self());自己分离式，主线程中pthread_join不起作用，直接返回。
* 3、
*/

// 正常线程
void* thread_function(void* arg) 
{
    int thread_num = *(int*)arg;
    printf("Thread %d is running. TID:%d\n", thread_num, (int)syscall(SYS_gettid));
    // 线程执行任务...
    printf("Thread %d is done.\n", thread_num);
    pthread_exit(NULL); // 退出线程
}

// 分离式退出线程
void* thread_function_self(void* arg) 
{
    pthread_detach(pthread_self());
    int thread_num = *(int*)arg;
    printf("Thread %d is running. TID:%d\n", thread_num, (int)syscall(SYS_gettid));
    // 线程执行任务...
    printf("Thread %d is done.\n", thread_num);
    pthread_exit(NULL); // 退出线程
}

// 固定点退出线程
void* thread_function_exit(void* arg) 
{
    int thread_num = *(int*)arg;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);

    while(1)
    {
        printf("Thread %d is running. TID:%d\n", thread_num, (int)syscall(SYS_gettid));
        // 线程执行任务...
        sleep(1);
        printf("Thread %d is done.\n", thread_num);
        pthread_testcancel(); // 线程退出点，线程只能从这里退出
        thread_num+=100;
    }
}

// 等待式线程创建
int fun_thread()
{
    pthread_t threads;
    int thread_args = 10;
    int rc;

    printf("MASTER TID:%d\n", (int)syscall(SYS_gettid));

    // 创建线程
    rc = pthread_create(&threads, NULL, thread_function, (void*)&thread_args);
    if (rc) {
        printf("ERROR: return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    
    // 等待线程退出
    rc = pthread_join(threads, NULL);
    if (rc) {
        printf("ERROR: return code from pthread_join() is %d\n", rc);
        exit(-1);
    }
    
    printf("All threads are done. %ld\n", threads);
    return 0;
}

// 分离式线程创建
int fun_thread_self()
{
    pthread_t threads;
    int thread_args = 9;
    int rc;

    printf("MASTER TID:%d\n", (int)syscall(SYS_gettid));

    // 创建线程
    rc = pthread_create(&threads, NULL, thread_function_self, (void*)&thread_args);
    if (rc) {
        printf("ERROR: return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    
    printf("All threads are done. %ld\n", threads);
    return 0;
}

// 固定点退出线程创建，线程取消
int fun_thread_exit()
{
    #define NUM_THREADS 5
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int rc, i;

    printf("MASTER TID:%d\n", (int)syscall(SYS_gettid));

    // 创建多个线程
    for (i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i+20;
        rc = pthread_create(&threads[i], NULL, thread_function_exit, (void*)&thread_args[i]);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    sleep(2);
    
    // 等待所有线程退出
    for (i = 0; i < NUM_THREADS; i++) {
        // 取消线程
        pthread_cancel(threads[i]);
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            printf("ERROR: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    printf("All threads are done.\n");
    return 0;
}

// 线程属性设置线程创建
int fun_thread_set()
{
    pthread_t threads;
	pthread_attr_t attr;
    size_t stack_size;
    int detach_state;
    int policy;
    struct sched_param param;
    // 初始化线程属性对象
	int ret = pthread_attr_init(&attr); 

    // 获取线程栈大小
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("线程堆栈大小: %zu bytes\n", stack_size);

    // 获取线程的分离状态
    pthread_attr_getdetachstate(&attr, &detach_state);
    if (detach_state == PTHREAD_CREATE_JOINABLE) {
        printf("线程的状态: joinable\n");
    } else if (detach_state == PTHREAD_CREATE_DETACHED) {
        printf("线程的状态: detached\n");
    }

    // 获取线程调度策略
    pthread_attr_getschedpolicy(&attr, &policy);
    switch (policy) {
        case SCHED_FIFO:
            printf("线程的调度策略: FIFO\n");
            break;
        case SCHED_RR:
            printf("线程的调度策略: Round Robin\n");
            break;
        case SCHED_OTHER:
            printf("T线程的调度策略: Other\n");
            break;
    }

    // 获取线程调度参数
    pthread_attr_getschedparam(&attr, &param);
    printf("线程的调度参数: %d\n", param.sched_priority);

    // 设置线程大小
    int stacksize = 1*1024*1024; 
	if((ret = pthread_attr_setstacksize(&attr, stacksize)) != 0){
		printf("线程大小设置错误\n");
	}
    // 设置线程为分离式
    if((ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0){
		printf("线程分离设置错误\n");
	}
	pthread_create(&threads,&attr, thread_function,NULL);
	if((ret = pthread_attr_destroy(&attr)) != 0){
		printf("线程属性销毁错误\n");
	}
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
    function_print("fun_thread",      fun_thread);
    function_print("fun_thread_self", fun_thread_self);
    function_print("fun_thread_exit", fun_thread_exit);
    function_print("fun_thread_set",  fun_thread_set);
    return 0;
}

// 测试程序
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}