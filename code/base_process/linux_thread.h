#ifndef  _LINUX_THREAD_H__
#define  _LINUX_THREAD_H__
#ifdef __cplusplus
extern "C" {
#endif

// 等待式线程创建
int fun_thread();
// 分离式线程创建
int fun_thread_self();
// 固定点退出线程创建
int fun_thread_exit();
// 线程属性设置线程创建
int fun_thread_set();

#ifdef __cplusplus
}
#endif
#endif