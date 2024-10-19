#ifndef  _LINUX_PROCESS_H__
#define  _LINUX_PROCESS_H__
#ifdef __cplusplus
extern "C" {
#endif

// 创建子进程
int fun_fork();
// 获取线程、进程ID
int fun_getID();

#ifdef __cplusplus
}
#endif
#endif