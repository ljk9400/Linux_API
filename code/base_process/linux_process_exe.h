#ifndef  _LINUX_PROCESS_EX_H__
#define  _LINUX_PROCESS_EX_H__
#ifdef __cplusplus
extern "C" {
#endif

// 在一个shell新进程执行命令
int fun_system(char* cmd);
// 在一个新的shell进程执行命令，
int fun_popen(char* cmd, char* mode);
// 代替原先的进程，exec后代码不会执行
int fun_execl();

#ifdef __cplusplus
}
#endif
#endif