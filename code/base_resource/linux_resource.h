#ifndef  _LINUX_RESOURCE_H__
#define  _LINUX_RESOURCE_H__
#ifdef __cplusplus
extern "C" {
#endif

// 获取进程或进程组的调度优先级
int api_getpriority();
// 获取进程的资源限制
int api_getrlimit();
// 获取进程资源使用情况
int api_getrusage();

#ifdef __cplusplus
}
#endif
#endif