/*
 * 文件名: LinuxMutex.c
 * 作者: ljk
 * 创建时间: 2023-07-15
 * 文件描述: linux锁操作例程
 */
#include <sys/mman.h>  
#include <sys/types.h> 
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include "linux_thread_mutex.h"



/**
 * 1、文件锁
*/
int fun_filemutex(const char* path)
{
    int fd;
    struct flock fl;
    // 打开文件
    fd = open(path, O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 设置文件锁
    fl.l_type   = F_WRLCK; // 排他锁
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0; // 锁定整个文件
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl");
        close(fd);
        return 1;
    }

    // 在此进行对文件的读写操作...

    // 解除文件锁
    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl");
        close(fd);
        return 1;
    }
    // 关闭文件
    close(fd);
    return 0;
}


/**
 * 2、线程锁
*/
pthread_mutex_t Mutex_A = PTHREAD_MUTEX_INITIALIZER, Mutex_B;// 初始化方式1

// 初始化方式2 需要pthread_mutex_destroy释放
int init_Mutex()
{
    pthread_mutex_init(&Mutex_B, NULL);
    return 0;
}

int destroy_Mutex()
{
    pthread_mutex_destroy(&Mutex_B);
    return 0;
}

int lock_Mutex()
{
    pthread_mutex_lock(&Mutex_B);
    return 0;
}

int trylock_Mutex()
{
    return pthread_mutex_trylock(&Mutex_B);;
}

int unlock_Mutex()
{
    pthread_mutex_unlock(&Mutex_B);
    return 0;
}

// 测试用例
int Number0 = 0;
static void *FunCall1(void *Arg)
{
    for (int Counter = 0; Counter < 10; Counter++)
    {   
        lock_Mutex();
        Number0--;
        printf("FunCall1 add  one to num %d\n", Number0);
        unlock_Mutex();
        usleep(1000);
    }
}

static void *FunCall2(void *Arg)
{
    for (int Counter = 0; Counter < 10; )
    {   
        if(trylock_Mutex())
        {
            Number0++;
            printf("FunCall2 add  one to num %d\n", Number0);
            unlock_Mutex();
            Counter++;
        }
        else
        {
            printf("FunCall2 failed to lock.\n");
        }
        usleep(1000);
    }
}

// 锁测试用例
int fun_mutex(void)
{
    pthread_t Id1, Id2;
    init_Mutex();
    pthread_create(&Id1, NULL, FunCall1, NULL);
    pthread_create(&Id2, NULL, FunCall2, NULL);
    pthread_join(Id1, NULL);
    pthread_join(Id2, NULL);
    destroy_Mutex();
    printf("Mutex Test Success!");
    return 0;
}


/**
 * 3、信号
*/
sem_t Sem_A;
int init_Sem()
{
    sem_init(&Sem_A, 0, 1); // 初始化信号量，初始值为 1
    return 0;
}

int destroy_Sem()
{
    sem_destroy(&Sem_A); // 销毁信号量
    return 0;
}

int wait_Sem()
{
    sem_wait(&Sem_A); // 等待信号量
    return 0;
}

int pos_Sem()
{
    sem_post(&Sem_A); // 释放信号量
    return 0;
}

// 测试用例
void* PrintNumbers(void* arg) 
{
    int id = *((int*)arg);
    for (int i = 1; i <= 20; i++) 
    {
        wait_Sem();
        printf("Thread %d: %d\n", id, i);
        pos_Sem(); 
        usleep(1000);
    }
    return NULL;
}

// 信号同步测试
int fun_sem() 
{
    pthread_t tid1, tid2;
    int id1 = 1, id2 = 2;
    init_Sem(); 
    pthread_create(&tid1, NULL, PrintNumbers, &id1);
    pthread_create(&tid2, NULL, PrintNumbers, &id2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    destroy_Sem();
    return 0;
}

/**
 * 4、进程间线程锁测试
*/
int fun_process_mutex() 
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);

    // 设置互斥锁的共享属性为进程间共享
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    // 创建具有指定属性的互斥锁
    pthread_mutex_t *mutex = mmap(NULL, sizeof(pthread_mutex_t), 
        PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pthread_mutex_init(mutex, &attr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process attempting to lock the mutex...\n");
        pthread_mutex_lock(mutex);
        printf("Child process locked the mutex.\n");
        // Simulate some work
        sleep(2);
        pthread_mutex_unlock(mutex);
        printf("Child process unlocked the mutex.\n");
    } else {
        // Parent process
        printf("Parent process attempting to lock the mutex...\n");
        pthread_mutex_lock(mutex);
        printf("Parent process locked the mutex.\n");
        // Simulate some work
        sleep(2);
        pthread_mutex_unlock(mutex);
        printf("Parent process unlocked the mutex.\n");
        wait(NULL);
    }

    // 销毁互斥锁及其属性
    pthread_mutex_destroy(mutex);
    pthread_mutexattr_destroy(&attr);

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
    function_print("fun_mutex", fun_mutex);
    function_print("fun_sem",   fun_sem);
    function_print("fun_process_mutex", fun_process_mutex);
    return 0;
}

// 测试用例
int main(int argc, char* argv[]) 
{
    main_test(argc, argv);
    return 0;
}
