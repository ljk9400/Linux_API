#ifndef  _LINUX_TIME_OS_H__
#define  _LINUX_TIME_OS_H__
#ifdef __cplusplus
extern "C" {
#endif

// gettimeofday获取当前时间
int api_gettimeofday();
// 获取当前系统允许时间
int api_uptime();
// 延时秒
int api_delay_s(int cnt);
// 延时微秒
int api_delay_us(int cnt);
// 延时纳秒
int api_delay_ns(int cnt);

#ifdef __cplusplus
}
#endif
#endif