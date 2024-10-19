/*
 * 文件名: ThreadPoll.c
 * 作者: ljk
 * 创建时间: 2023-07-26
 * 文件描述: 线程池 增加监控 动态均衡
 */
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>



#define THREAD_POOL_SIZE_MIN 2 // 最小线程池大小
#define THREAD_POOL_SIZE_MAX 10 // 最大线程池大小
#define THREAD_POOL_CAPACITY 10000 // 线程池容量
void *executeTask(void *arg);
void *monitorTaskLoad(void *arg);

// 日志等级
typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

static LogLevel currentLogLevel = LOG_LEVEL_DEBUG;

// 日志函数
void logMessage(LogLevel level, const char *format, ...) {
    if (level > currentLogLevel) return;

    const char *levelStr;
    switch (level) {
        case LOG_LEVEL_ERROR: levelStr = "ERROR"; break;
        case LOG_LEVEL_WARN: levelStr = "WARN"; break;
        case LOG_LEVEL_INFO: levelStr = "INFO"; break;
        case LOG_LEVEL_DEBUG: levelStr = "DEBUG"; break;
        default: levelStr = "UNKNOWN"; break;
    }

    va_list args;
    va_start(args, format);
    printf("[%s] ", levelStr);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

// 任务结构体
typedef struct {
    void (*function)(void *arg);
    void *arg;
    int taskId; // 任务编号
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
    int threadCount; // 当前线程数量
    int stop; // 标记线程池是否应该关闭
    int taskCounter; // 任务计数器
} ThreadPool;

static ThreadPool *pool = NULL;

// 销毁线程池
void destroyThreadPool() {
    if (pool) {
        pthread_mutex_lock(&pool->mutex);
        pool->stop = 1;
        pthread_cond_broadcast(&pool->not_empty);
        pthread_mutex_unlock(&pool->mutex);

        for (int i = 0; i < pool->threadCount; i++) {
            pthread_join(pool->threads[i], NULL);
        }

        free(pool->threads);
        free(pool->tasks);
        pthread_mutex_destroy(&pool->mutex);
        pthread_cond_destroy(&pool->not_full);
        pthread_cond_destroy(&pool->not_empty);
        free(pool);
        pool = NULL;

        logMessage(LOG_LEVEL_INFO, "线程池已销毁");
    }
}

// 初始化线程池
void initThreadPool(int capacity) {
    pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    pool->tasks = (Task *)malloc(sizeof(Task) * capacity);
    pool->capacity = capacity;
    pool->size = 0;
    pool->front = 0;
    pool->rear = 0;
    pool->stop = 0;
    pool->taskCounter = 0;
    pool->threadCount = THREAD_POOL_SIZE_MIN;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->not_full, NULL);
    pthread_cond_init(&pool->not_empty, NULL);

    logMessage(LOG_LEVEL_INFO, "线程池已初始化，容量为 %d", capacity);
}

// 创建线程池中的线程
void createThreadPool() {
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * THREAD_POOL_SIZE_MAX);
    for (int i = 0; i < THREAD_POOL_SIZE_MIN; i++) {
        if (pthread_create(&pool->threads[i], NULL, executeTask, NULL) != 0) {
            logMessage(LOG_LEVEL_ERROR, "线程创建失败");
            exit(1);
        }
        pthread_detach(pool->threads[i]);
    }

    // 创建守护线程
    pthread_t monitorThread;
    if (pthread_create(&monitorThread, NULL, monitorTaskLoad, NULL) != 0) {
        logMessage(LOG_LEVEL_ERROR, "守护线程创建失败");
        exit(1);
    }
    pthread_detach(monitorThread);

    logMessage(LOG_LEVEL_INFO, "线程池中的线程已创建，数量为 %d", THREAD_POOL_SIZE_MIN);
}

// 添加任务
void addTask(void (*function)(void *arg), void *arg) {
    pthread_mutex_lock(&pool->mutex);
    while (pool->size == pool->capacity && !pool->stop) {
        pthread_cond_wait(&pool->not_full, &pool->mutex);
    }

    if (pool->stop) {
        pthread_mutex_unlock(&pool->mutex);
        return;
    }

    int taskId = ++pool->taskCounter;
    pool->tasks[pool->rear].function = function;
    pool->tasks[pool->rear].arg = arg;
    pool->tasks[pool->rear].taskId = taskId;
    pool->rear = (pool->rear + 1) % pool->capacity;
    pool->size++;

    logMessage(LOG_LEVEL_DEBUG, "任务 %d 已入队", taskId);

    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->mutex);
}

// 执行任务
void *executeTask(void *arg) {
    while (1) {
        pthread_mutex_lock(&pool->mutex);
        while (pool->size == 0 && !pool->stop) {
            pthread_cond_wait(&pool->not_empty, &pool->mutex);
        }

        if (pool->stop && pool->size == 0) {
            pthread_mutex_unlock(&pool->mutex);
            return NULL;
        }

        Task task = pool->tasks[pool->front];
        pool->front = (pool->front + 1) % pool->capacity;
        pool->size--;

        logMessage(LOG_LEVEL_DEBUG, "任务 %d 开始执行", task.taskId);

        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->mutex);

        (task.function)(task.arg);

        logMessage(LOG_LEVEL_DEBUG, "任务 %d 执行完毕", task.taskId);
    }
}

// 监控任务负载的守护线程
void *monitorTaskLoad(void *arg) {
    while (!pool->stop) {
        sleep(5); // 每5秒检查一次任务负载

        pthread_mutex_lock(&pool->mutex);
        int taskLoad = (pool->size * 100) / pool->capacity;

        if (taskLoad < 30 && pool->threadCount > THREAD_POOL_SIZE_MIN) {
            logMessage(LOG_LEVEL_INFO, "任务负载低于30%%，减少工作线程数量");
            pool->threadCount = THREAD_POOL_SIZE_MIN;
        } else if (taskLoad > 70 && pool->threadCount < THREAD_POOL_SIZE_MAX) {
            logMessage(LOG_LEVEL_INFO, "任务负载高于70%%，增加工作线程数量");
            for (int i = pool->threadCount; i < THREAD_POOL_SIZE_MAX; i++) {
                if (pthread_create(&pool->threads[i], NULL, executeTask, NULL) != 0) {
                    logMessage(LOG_LEVEL_ERROR, "线程创建失败");
                    exit(1);
                }
                pthread_detach(pool->threads[i]);
            }
            pool->threadCount = THREAD_POOL_SIZE_MAX;
        }

        pthread_mutex_unlock(&pool->mutex);
    }
    return NULL;
}

// 将上面函数封装成类似于类的结构体
typedef struct ThreadPool_API {
    void (*initThreadPool)(int capacity);
    void (*createThreadPool)();
    void (*destroyThreadPool)();
    void (*addTask)(void (*function)(void *arg), void *arg);
} ThreadPool_API, *pThreadPool_API;

// 自定义任务函数
void taskFunction(void* idx) {
    int taskId = *(int*)idx;
    logMessage(LOG_LEVEL_DEBUG, "任务 %d 正在运行", taskId);
    printf(" --task-- %d\n", taskId);
    // sleep随机时间, 并打印随机时间
    int sleep_time =  time(NULL)%4000;
    printf(" --task-- %d sleep: %d豪秒\n", taskId, sleep_time);
    srand(sleep_time);
    printf(" --task-- %d sleep end\n", taskId);
}

// 获取线程池 API
pThreadPool_API getThreadPoolAPI() {
    pThreadPool_API api = (pThreadPool_API)malloc(sizeof(ThreadPool_API));
    api->initThreadPool = initThreadPool;
    api->createThreadPool = createThreadPool;
    api->destroyThreadPool = destroyThreadPool;
    api->addTask = addTask;
    return api;
}

// 销毁线程池 API
void destroyThreadPoolAPI(pThreadPool_API api) {
    free(api);
}

int main(int argc, char* argv[]) {
    pThreadPool_API api = getThreadPoolAPI();
    api->initThreadPool(100);
    api->createThreadPool();

    int idx[10];
    for (int i = 0; i < 10; i++) {
        idx[i] = i;
        api->addTask(taskFunction, &idx[i]);
    }

    sleep(15);

    api->destroyThreadPool(); // 销毁线程池
    destroyThreadPoolAPI(api); // 销毁结构体
    return 0;
}