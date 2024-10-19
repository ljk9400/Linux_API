/*
 * 文件名: server.c
 * 作者: ljk
 * 创建时间: 2023-07-22
 * 文件描述: UDP通信服务端
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "common_socket.h"

 

// 运行状态
static int run_flag = 1;

// 建立UDP 服务端
static int udpServer(int port) 
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定地址和端口号
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is listening...\n");
    // 等待连接
    while (run_flag) 
    {
        char buffer[MAX_BUFFER_SIZE]={0};
        ssize_t recv_len = udp_event_recv(server_fd, buffer, MAX_BUFFER_SIZE, &client_addr, &addrlen);
        if (recv_len == -1) {
            perror("recvfrom");
            continue;
        }

        // Send a response back to the client
        if (udp_event_send(server_fd, buffer, strlen(buffer), &client_addr, sizeof(client_addr)) == -1) {
            perror("sendto");
        }
    }
    
    close(server_fd);
    run_flag = 0;
    return 0;
}

// 客户端断掉，服务端没有任何消息
// 服务端断掉，客户端没有任何信息
int main(int argc, char* argv[]) 
{
    udpServer(SERVER_PORT);
    return 0;
}
