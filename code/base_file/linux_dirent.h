#ifndef  _LINUX_DIR66535_H__
#define  _LINUX_DIR66535_H__
#ifdef __cplusplus
extern "C" {
#endif

// 遍历文件
int api_finddir(char* dir, int depth);
// 查找文件
int api_seekdir(char* target, char* path_dir);
// 创建目录
int api_mkdir(const char *path, unsigned int mode);
// 删除目录
int api_rmdir(const char *path);
// 获取当前工作目录
int api_getcwd();
// 更改当前工作目录
int api_chdir(const char *path);

#ifdef __cplusplus
}
#endif
#endif