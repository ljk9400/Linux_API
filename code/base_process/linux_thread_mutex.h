#ifndef  _LINUX_MUTEX_H__
#define  _LINUX_MUTEX_H__
#ifdef __cplusplus
extern "C" {
#endif

int fun_filemutex(const char* path);
// 锁测试用例
int fun_MutexTest(void);
// 信号同步测试
int fun_sem();
// 进程间锁
int fun_process_mutex();

#ifdef __cplusplus
}
#endif
#endif