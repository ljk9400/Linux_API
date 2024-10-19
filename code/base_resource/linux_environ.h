#ifndef  _LINUX_ENV_H__
#define  _LINUX_ENV_H__
#ifdef __cplusplus
extern "C" {
#endif

// getenv获取环境变量
int api_getenv(const char* path);
// putenv设置环境变量
int api_putenv(const char* path, const char* value);
// setenv设置环境变量 over 非0：覆盖已经存在的环境变量值 0：不覆盖已经存在环境变量值
int api_setenv(const char* path, const char* value, int over);
// environ 展示环境变量
int api_showenv();

#ifdef __cplusplus
}
#endif
#endif