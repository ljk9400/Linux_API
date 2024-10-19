#ifndef  _LINUX_PWD_H__
#define  _LINUX_PWD_H__
#ifdef __cplusplus
extern "C" {
#endif

// getuid获取用户id
int api_getuid();
// getlogin获取登录用户
int api_getlogin();
// passwd获取密码
int api_passwd(int uid, char* user);
// getpwent遍历用户
int api_getpwent();
// 获取主机信息
int api_gethostname();
// 获取系统信息
int api_uname();
// 主机标识id
int api_gethostid();


#ifdef __cplusplus
}
#endif
#endif