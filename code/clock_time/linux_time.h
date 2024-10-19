#ifndef  _LINUX_TIME_H__
#define  _LINUX_TIME_H__
#ifdef __cplusplus
extern "C" {
#endif

//  1970年1月1日至今的秒数
int api_time();
//  时间差
int api_difftime(int cnt);
//  现在时间
int api_gmtime();
//  现在时间
int api_localtime();
//  时间戳
int api_mktime();
//  现在时间
int api_astime();
//  时间格式化
int api_strtime();


#ifdef __cplusplus
}
#endif
#endif