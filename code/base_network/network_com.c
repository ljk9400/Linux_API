#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include "network_com.h"



// 获取固定网卡mac地址
void get_local_mac(char *if_name) {
    unsigned char local_net_mac_hex[6];

    struct ifreq m_ifreq;
    int sock = 0;
    sock = socket(AF_INET,SOCK_STREAM, 0);
    strcpy(m_ifreq.ifr_name,if_name);
    
    ioctl(sock, SIOCGIFHWADDR, &m_ifreq);
    int i = 0;
    for(i = 0; i < 6; i++){
        local_net_mac_hex[i] = m_ifreq.ifr_hwaddr.sa_data[i];
    }
    printf("mac addr = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n\n", 
        local_net_mac_hex[0], local_net_mac_hex[1], local_net_mac_hex[2], 
        local_net_mac_hex[3], local_net_mac_hex[4], local_net_mac_hex[5]);

    close(sock);
}

// 获取固定地址ip地址
void get_local_ip(char *if_name) {
    char local_net_ip[32];

    int ret = 0;
    struct ifaddrs *addr = NULL;
    struct ifaddrs *temp_addr = NULL;
    ret = getifaddrs(&addr);
    if (ret == 0) {
        temp_addr = addr;
        while(temp_addr != NULL) 
        {   
            if(temp_addr->ifa_addr == 0)
            {
                temp_addr = temp_addr->ifa_next;
                continue;
            }
            if(temp_addr->ifa_addr->sa_family == AF_INET) 
            {
                if(strcmp(temp_addr->ifa_name, if_name)  == 0) 
                {
                    char *tmp = inet_ntoa((struct in_addr){.s_addr=((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr.s_addr});
                    memcpy(local_net_ip, tmp, strnlen(tmp,sizeof(local_net_ip)));
                    printf("\nlocal ip = %s\n\n",local_net_ip);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(addr);
}

// 测试程序 
int main(int argc, char* argv[])
{
    // 测试网卡名
    char* netcard = "ens33";
    get_local_mac(netcard);
    get_local_ip(netcard);
    return 0;
}