#ifndef  _LINUX_THREADPOOL_H__
#define  _LINUX_THREADPOOL_H__
#ifdef __cplusplus
extern "C" {
#endif

#define THREAD_POOL_SIZE 5 // 线程池大小
#define THREAD_POOL_CAPACITY 10000 // 线程池容量

// 任务结构体
typedef struct {
    void (*function)(void *arg);
    void *arg;
} Task;

// 线程池结构体
typedef struct {
    Task *tasks;
    int capacity;
    int size;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
    pthread_t *threads; // 线程数组
    int stop; // 标记线程池是否应该关闭
} ThreadPool;

// 线程池函数接口
typedef struct _ThreadPool_API {
    void (*initThreadPool)(int capacity);
    void (*createThreadPool)();
    void (*destroyThreadPool)();
    void (*addTask)(void (*function)(void *arg), void *arg);
} ThreadPoolAPI, *pThreadPoolAPI;

#ifdef __cplusplus
}
#endif
#endif