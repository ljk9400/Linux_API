#ifndef  _NET_COMMON_66736_H__
#define  _NET_COMMON_66736_H__
#ifdef __cplusplus
extern "C" {
#endif

// 获取固定网卡mac地址
void get_local_mac(char *if_name);
// 获取固定地址ip地址
void get_local_ip(char *if_name);

#ifdef __cplusplus
}
#endif
#endif