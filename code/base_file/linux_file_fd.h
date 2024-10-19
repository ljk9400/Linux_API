#ifndef  _LINUX_FILEIO66537_H__
#define  _LINUX_FILEIO66537_H__
#ifdef __cplusplus
extern "C" {
#endif

// 文件权限确认
int api_access(const char* path);
// 文件IO写
int api_write(const char* path, const char* data) ;
// 文件IO读
int api_read(const char* path);
// 文件lseek
int api_seek(const char* path);
// 文件stat
int api_stat(const char* path);
// 文件dup
int api_dup2(const char* path);

#ifdef __cplusplus
}
#endif
#endif