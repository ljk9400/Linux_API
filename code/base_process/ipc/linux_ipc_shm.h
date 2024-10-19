#ifndef  _LINUX_IPC_SHM_H__
#define  _LINUX_IPC_SHM_H__
#ifdef __cplusplus
extern "C" {
#endif

#define log_debug  printf
// 读写数据大小
#define SHM_SIZE  (1024)
#define MEM_SIZE  (2*SHM_SIZE+12+2)
// 启动读程序目录
#define IDPS_PROCESS_PATH   "../bin"
#define IDPS_PROCESS_NAME   "linux_ipc_shm_read"
#define IDPS_PROCESS_EXE    IDPS_PROCESS_PATH "/" IDPS_PROCESS_NAME
// log打印方式
#define log_debug  printf

// 进程间通信结构体
typedef struct _IPC_Shared_Str
{
    char runFlag;     // 0:停止状态/从退出后设置0, 1:主要求从运行, 2(非0,1):主要求从停止, 优先级最高
    char masterTR;    // 0:主写数据，非0:从读数据
    char slaveTR;     // 0:从写数据，非0:主读数据
    char dataType;    // 0:注册协商, 1:拉取配置, 2(非0,1)通信数据
    int  dataLen;
    char data[SHM_SIZE];
    char readType;
    int  readLen;
    char read[SHM_SIZE];
} IPC_Shared_Str;

#ifdef __cplusplus
}
#endif
#endif