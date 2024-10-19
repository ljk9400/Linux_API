#ifndef  _LINUX_SIGNAL_H__
#define  _LINUX_SIGNAL_H__
#ifdef __cplusplus
extern "C" {
#endif

// 信号测试
int fun_signal();
// 信号测试
int fun_alarm();
// 信号测试
int fun_kill();
// 信号测试
int fun_sigaction();
// 信号设置
int fun_sigset();
// 信号设置
int fun_sighandle();

#ifdef __cplusplus
}
#endif
#endif