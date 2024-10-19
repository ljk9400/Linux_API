#ifndef  _LINUX_LINK66538_H__
#define  _LINUX_LINK66538_H__
#ifdef __cplusplus
extern "C" {
#endif

// 更改权限
int api_chmod(const char* path, mode_t mode);
// 更改用户和组
int api_chown(const char* path, uid_t owner, gid_t group);
// 创建硬链接
int api_hardlink(const char *src, const char *dest);
// 创建软链接
int api_symlink(const char *src, const char *dest);
// 删除链接
int api_unlink(const char *path);

#ifdef __cplusplus
}
#endif
#endif