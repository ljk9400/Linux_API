#ifndef  _LINUX_MEASURER_H__
#define  _LINUX_MEASURER_H__
#ifdef __cplusplus
extern "C" {
#endif

// 测量函数执行时间
void measure_time(int (*func)());
// 获取当前进程 CPU 使用率
double get_cpu_usage();
// 获取当前进程内存使用率
long get_memory_usage();
// 获取当前 GPU 使用率（todo）
double get_gpu_usage();

#ifdef __cplusplus
}
#endif
#endif