/*
 * 文件名: ThreadPoll.c
 * 作者: ljk
 * 创建时间: 2023-07-26
 * 文件描述: 线程池操作例程
 */
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

void *executeTask(void *arg);

static ThreadPool *pool = NULL;

// 销毁线程池
void destroyThreadPool() {
    if (pool) {
        pthread_mutex_lock(&pool->mutex);
        pool->stop = 1;
        pthread_cond_broadcast(&pool->not_empty);
        pthread_mutex_unlock(&pool->mutex);

        for (int i = 0; i < THREAD_POOL_SIZE; i++) {
            pthread_join(pool->threads[i], NULL);
        }

        free(pool->threads);
        free(pool->tasks);
        pthread_mutex_destroy(&pool->mutex);
        pthread_cond_destroy(&pool->not_full);
        pthread_cond_destroy(&pool->not_empty);
        free(pool);
        pool = NULL;
    }
}

// 初始化线程池
void initThreadPool(int capacity) {
    capacity = capacity>THREAD_POOL_CAPACITY?THREAD_POOL_CAPACITY:capacity;
    pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    pool->tasks = (Task *)malloc(sizeof(Task) * capacity);
    pool->capacity = capacity;
    pool->size = 0;
    pool->front = 0;
    pool->rear = 0;
    pool->stop = 0;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->not_full, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
}

// 创建线程池中的线程
void createThreadPool() {
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * THREAD_POOL_SIZE);
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        if (pthread_create(&pool->threads[i], NULL, executeTask, NULL) != 0) {
            perror("pthread_create");
            exit(1);
        }
        //pthread_detach(pool->threads[i]);
    }
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

    pool->tasks[pool->rear].function = function;
    pool->tasks[pool->rear].arg = arg;
    pool->rear = (pool->rear + 1) % pool->capacity;
    pool->size++;

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

        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->mutex);

        (task.function)(task.arg);
    }
}

// 获取线程池 API
pThreadPoolAPI getThreadPoolAPI() {
    pThreadPoolAPI api = (pThreadPoolAPI)malloc(sizeof(ThreadPoolAPI));
    api->initThreadPool = initThreadPool;
    api->createThreadPool = createThreadPool;
    api->destroyThreadPool = destroyThreadPool;
    api->addTask = addTask;
    return api;
}

// 销毁线程池 API
void destroyThreadPoolAPI(pThreadPoolAPI api) {
    free(api);
}

// 自定义任务函数
void taskFunction(void* idx) {
    printf(" --task-- %d start\n", *(int*)idx);
    int sleep_time =  time(NULL)%4000;
    printf(" --task-- %d end\n", *(int*)idx);
}

int main_test(int argc, char* argv[]) {
    pThreadPoolAPI api = getThreadPoolAPI();
    api->initThreadPool(100);
    api->createThreadPool();

    int idx[1000];
    for (int i = 0; i < 1000; i++) {
        idx[i] = i;
        api->addTask(taskFunction, &idx[i]);
    }
    sleep(10);

    api->destroyThreadPool(); // 销毁线程池
    destroyThreadPoolAPI(api); // 销毁结构体
    return 0;
}

int main(int argc, char* argv[]) 
{
    return main_test(argc, argv);
}