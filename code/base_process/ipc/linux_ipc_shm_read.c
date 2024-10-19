/**
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信共享内存例程
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <unistd.h>
#include "linux_ipc_shm.h"





static IPC_Shared_Str* pSharedMemory = NULL;

// 往tsp发送数据
int write_IPC(char* data, int len, char type)
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        if(pIPCWR->slaveTR == 0 && pIPCWR->runFlag == 1)
        {
            pIPCWR->readType = type;
            pIPCWR->readLen  = len;
            memset(pIPCWR->read, 0, SHM_SIZE);
            strncpy(pIPCWR->read, data, SHM_SIZE);
            pIPCWR->slaveTR = 1;
            return true;
        }
    }
    return false;
}

// 从tsp接收数据
int read_IPC(char* data, int *len, char *type)
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        if(pIPCWR->masterTR && pIPCWR->runFlag == 1)
        {
            *type = pIPCWR->dataType;
            *len  = pIPCWR->dataLen;
            strncpy(data, pIPCWR->data, SHM_SIZE);
            pIPCWR->masterTR = 0;
            return true;
        }
    }
    return false;
}

// 防止接收tsp遗留数据
void clear_Reply()
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        pIPCWR->masterTR = 0;
    }
}

// 创建共享内存
int create_SharedMemory()
{
    static int init = 0;
    if(init == 0)
    {
        init = 1;
        key_t key = 36001;

        // 打开创建共享内存
        int shmid = shmget(key, MEM_SIZE, 0666);
        if (shmid == -1) {
            log_debug("[SE] shmget error\n");
            init = 0;
            return init;
        }

        // 附加共享内存地址
        char* shmaddr = shmat(shmid, NULL, 0);
        if (shmaddr == (char*)-1){
            log_debug("[SE] shmat error\n");
            init = 0;
            return init;
        }
        pSharedMemory = (IPC_Shared_Str*)shmaddr;
        pSharedMemory->masterTR = 0; 
        pSharedMemory->slaveTR  = 0;
    }
    return init;
}

// 数据获取, cnt重复次数, time等待毫秒
int read_repeat(char* data, int *len, char *type, int cnt, int time)
{
    int repeat_cnt = 0;
    while(read_IPC(data, len, type) == false)
    {
        printf("[S] read fault\n");
        usleep(time*1000);
        if(repeat_cnt++ > cnt)
        {
            return false;
        }  
    }
    return true;
}

// 防止多开多开,进程如果退出也会自己释放文件锁。
#include <fcntl.h>
#include <sys/file.h>
int Lock_Process()
{
    const char *lockFilePath = "/tmp/my_process_lock";

    int lockFile = open(lockFilePath, O_RDWR | O_CREAT, 0666);
    if (lockFile == -1) {
        log_debug("[E] Failed to open lock file\n");
        //exit(EXIT_FAILURE);
        return false;
    }

    int lockResult = flock(lockFile, LOCK_EX | LOCK_NB);
    if (lockResult == -1) {
        log_debug("[E] Another instance of the process is already running\n");
        close(lockFile);
        exit(EXIT_FAILURE);
    }

    // 进程如果退出也会自己释放文件锁，这里就不用解锁了
    //flock(lockFile, LOCK_UN);
    //close(lockFile);
    return true;
}

// 数据制造
int RW_Test()
{
    char *regist_str = "regist:1234567890";
    char *list_str   = "list:asdfghjklqwertyuiopzxcvbnm";
    char *event_str  = "!@#$/%^&*()";

    if(pSharedMemory <= 0)
        return false;

    while(pSharedMemory->runFlag != 2)
    {
        int len = -1;
        char data[SHM_SIZE]={0}, type = -1;
    
        write_IPC(regist_str, strlen(regist_str), 0);
        log_debug("[SI] Write: %x, %ld, %s\n", 0, strlen(regist_str), regist_str);
        if(read_repeat(data, &len, &type, 100, 5))
        {
            log_debug("[SI] Read: %x, %d, %s\n", type, len, data);
        }

        write_IPC(list_str, strlen(list_str), 1);
        log_debug("[SI] Write: %x, %ld, %s\n", 1, strlen(list_str), list_str);
        if(read_repeat(data, &len, &type, 100, 5))
        {
            log_debug("[SI] Read: %x, %d, %s\n", type, len, data);
        }

        write_IPC(event_str, strlen(event_str), 2);
        log_debug("[SI] Write: %x, %ld, %s\n", 2, strlen(event_str), event_str);

        sleep(1);
    }
    pSharedMemory->runFlag = 0;
    return true;
}



// 测试例程
int main(int argc, char** argv[]) 
{
    if(Lock_Process())
    {
        if(create_SharedMemory())
        {
            RW_Test();
        }
    }
    log_debug("[SI] IPC module main exit\n");
    return 0;
}
