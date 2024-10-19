/**
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信共享内存例程
 * 共享内存建立后一直存在，释放需要进程分离后（进程退出后自动分离），最后一个进程去释放
 * 查看共享内存  ipcs -m
 * 删除共享内存	 ipcrm -m <shmid>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "linux_ipc_shm.h"





static IPC_Shared_Str* pSharedMemory = NULL;

// 通知IPC启动
int start_IPC()
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        pIPCWR->runFlag = 1;
        return true;
    }
    return false;
}

// 通知IPC退出
int stop_IPC()
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        pIPCWR->runFlag = 2;
        return true;
    }
    return false;
}

// 往IPC发送数据
int write_IPC(char* data, int len, char type)
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        if(pIPCWR->masterTR==0  && pIPCWR->runFlag==1)
        {
            pIPCWR->dataType = type;
            pIPCWR->dataLen  = len;
            memset(pIPCWR->data, 0, SHM_SIZE);
            strncpy(pIPCWR->data, data, SHM_SIZE);
            pIPCWR->masterTR = 1;
            return true;
        }
    }
    return false;
}

// 从IPC接收数据
int read_IPC(char* data, int *len, char *type)
{
    if(pSharedMemory > 0)
    {
        IPC_Shared_Str* pIPCWR = pSharedMemory;
        if(pIPCWR->slaveTR && pIPCWR->runFlag==1)
        {
            *type = pIPCWR->readType;
            *len  = pIPCWR->readLen;
            strncpy(data, pIPCWR->read, SHM_SIZE);
            pIPCWR->slaveTR = 0;
            return true;
        }
    }
    return false;
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
        int shmid = shmget(key, MEM_SIZE, IPC_CREAT | 0666);
        if (shmid == -1) 
        {
            perror("s");
            log_debug("[ME] shmget error\n");
            init = 0;
            return init;
        }

        // 附加共享内存地址
        char* shmaddr = shmat(shmid, NULL, 0);
        if (shmaddr == (char*)-1) 
        {
            log_debug("[ME] shmat error\n");
            init = 0;
            return init;
        }
        pSharedMemory = (IPC_Shared_Str*)shmaddr;
    }
    if(pSharedMemory > 0)
    {
        pSharedMemory->runFlag = 0;
    }
    return init;
}

// 查询进程是否存在
bool process_IsRunning(const char *processName) 
{
    FILE *fp;
    char cmd[100];
    sprintf(cmd, "pgrep -x %s", processName);
    fp = popen(cmd, "r");
    if (fp == NULL) {
        log_debug(" M [E] popen failed\n");
        return true;
    }
    
    int pid;
    if (fscanf(fp, "%d", &pid) != 1) {
        pclose(fp);
        return true;
    }
    
    pclose(fp);
    return false;
}

// 检查IPC进程是否存在
int check_IPCRun()
{
    //int result = system("ps -e | grep IDPS_R > /dev/null");
    int result = process_IsRunning(IDPS_PROCESS_NAME);
    if (result == 0) {
        log_debug("[MI] IPC process is running.\n");
    } 
    else {
        log_debug("[MI] IPC process is not running.\n");
    }
    return result;
}

// 启动IPC子进程
void* startIPC_SubProcess(void* arg)
{
    *(int*)arg = 1;
    pid_t pid = fork();
    if (pid == 0) // 子进程，启动IPC进程
    {
        char cwd_path[1024];
        // 获取当前工作目录
        getcwd(cwd_path, 1024);
        log_debug("Current working directory: %s\n", cwd_path);
        execl(IDPS_PROCESS_EXE, IDPS_PROCESS_NAME, NULL);
        log_debug("[ME] IPC SubProcess execl failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);  // 等待子进程终止
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            log_debug("[MI] IPC SubProcess exited with status: %d\n", exit_status);
        }
    }
    *(int*)arg = 0;
    return NULL; 
}

// IPC子进程的守护线程创建
int InitIPC_Thread()
{
	// 守护进程状态
	static int IPCFalg = 0;
	pthread_t threads;
	// 创建线程
	if(IPCFalg == 0){
		pthread_create(&threads, NULL, startIPC_SubProcess, (void*)&IPCFalg);
	}
}

// 守护调度线程
void* thread_IPC_ReadWrite(void* arg) 
{
	*(int*)arg = 1;
	int* runFalg = (int*)arg;
    pthread_detach(pthread_self());

	log_debug("[MI] IPC read & write thread start\n");
    while(*runFalg)
	{
        int len = -1;
        char data[SHM_SIZE], type = -1;
        char str[20];
        static int cnt = 0;
        
		if(read_IPC(data, &len, &type))
        {
            log_debug("[MI] Read: %x, %d, %s\n", type, len, data);
            sprintf(str, "Master send %d", cnt++);
            if(type == 0)
            {
                write_IPC(str, strlen(str), 0);
                log_debug("[MI] Write: %x, %ld, %s\n", 0, strlen(str), str);
            }
            else if(type == 1)
            {
                write_IPC(str, strlen(str), 1);
                log_debug("[MI] Write: %x, %ld, %s\n", 1, strlen(str), str);
            }
        }
	} 
	log_debug("[MI] IPC read & write thread exit\n");
	*runFalg = 0;
}

// 读写线程创建
int InitRW_Thread()
{
	// 守护进程状态
	static int IPCWRFalg = 0;
	pthread_t threads;
	
	// 创建线程
	if(IPCWRFalg == 0)
    {
		pthread_create(&threads, NULL, thread_IPC_ReadWrite, (void*)&IPCWRFalg);
	}
}

// 初始共享内存和启动IPC
int Init_IPC()
{
    log_debug("[MI] IPC module init\n");
    create_SharedMemory();
    if(check_IPCRun()){
        InitIPC_Thread();
    }
    check_IPCRun();
    return 0;
}


// 删除共享内存
int delect_SharedMemory(int shmid, char* shmaddr)
{
    // 分离共享内存
    shmdt(shmaddr);

    // 最后一个进程分离后，删除共享内存段
    struct shmid_ds shmid_ds_info;
    if (shmctl(shmid, IPC_STAT, &shmid_ds_info) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    printf("共享内存信息:\n");
    printf("Size: %lu\n",             shmid_ds_info.shm_segsz);
    printf("Last Attach Time: %ld\n", shmid_ds_info.shm_atime);
    printf("Last Detach Time: %ld\n", shmid_ds_info.shm_dtime);
    printf("Last Change Time: %ld\n", shmid_ds_info.shm_ctime);
    printf("PID of Creator: %d\n",    shmid_ds_info.shm_cpid);
    printf("PID of Last Shmop: %d\n", shmid_ds_info.shm_lpid);
    printf("附加到共享内存的进程个数: %ld\n", shmid_ds_info.shm_nattch);

    if (shmid_ds_info.shm_nattch == 0) 
    {
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
        printf("共享内存被删除.\n");
    }

    return shmid;
}

// 创建键值，特定的IPC资源标识
key_t create_Key(const char* path, int proj_id)
{
    // path任意目录, proj_id一个非负整数
    if(path && proj_id>0){
        key_t key = ftok(path, proj_id);
        return key;
    }
    return -1;    
}





// 测试例程
int main(int argc, char* argv[])
{
    Init_IPC();
    InitRW_Thread();
  
    printf("-------start-------\n");
    start_IPC();
    sleep(20);
    printf("-------stop-------\n");
    stop_IPC();
    sleep(10);

    return 0;
}