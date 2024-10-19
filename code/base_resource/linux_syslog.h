#ifndef  _LINUX_SYSLOG_H__
#define  _LINUX_SYSLOG_H__
#ifdef __cplusplus
extern "C" {
#endif

// 写入到 /var/log/syslog日志中
int api_syslog();
// 设置写入过滤
int api_setlogmask();

#ifdef __cplusplus
}
#endif
#endif