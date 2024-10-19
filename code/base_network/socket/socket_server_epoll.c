/*
 * 文件名: epoll.c
 * 作者: ljk
 * 创建时间: 2023-07-22
 * 文件描述: TCP通信服务端 epoll
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include "common_socket.h"



static int server_run_flag = 1;

// Get local address ip/fd
static int printAddress(int sockfd, int port, int localfd, struct sockaddr_in client_sockaddr)
{
    int ret = -1;
    struct sockaddr_in local_address;
    socklen_t address_length = sizeof(local_address);
    
    if (getsockname(sockfd, (struct sockaddr*)&local_address, &address_length) == 0)
    {
        char* address_p = inet_ntoa(local_address.sin_addr);
        fprintf(stdout, "fd(%d), LocalIP:%s, LocalPort:%hu\n", localfd, address_p, SERVER_PORT);
    }
    fprintf(stdout, "fd(%d), RemoteIP:%s, RemotePort:%hu\n", sockfd, inet_ntoa(client_sockaddr.sin_addr), ntohs(client_sockaddr.sin_port));
    event_send(sockfd, CONNECT_SUCCESS, strlen(CONNECT_SUCCESS)); 

    return ret;
}

// Initialize TCP, blocking wait for connection
static int tcpServer(int port) 
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 绑定地址和端口号
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听端口
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "TCP server bind listening ...\n");

    // 创建 epoll 实例
    int epoll_fd, event_count;
    struct epoll_event event, events[MAX_CONNECT_CNT];
    if ((epoll_fd = epoll_create1(0)) < 0) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    // 添加监听 socket 到 epoll 实例中
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0) {
        perror("epoll_ctl failed");
        exit(EXIT_FAILURE);
    }

    // 等待连接
    while (server_run_flag) 
    {
        // 等待事件发生
        event_count = epoll_wait(epoll_fd, events, MAX_CONNECT_CNT, -1);
        if (event_count < 0) {
            perror("epoll_wait failed");
            exit(EXIT_FAILURE);
        }

        // 处理所有事件
        for (int i = 0; i < event_count; i++) 
        {
            int fd = events[i].data.fd;
            
            // 如果是监听 socket 上有新连接请求
            if (fd == server_fd) {
                // 接受新连接
                //if ((new_socket = accept(server_fd, NULL, NULL)) < 0)
                if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }
                
                // 将新连接的 socket 添加到 epoll 实例中
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) {
                    perror("epoll_ctl failed");
                    exit(EXIT_FAILURE);
                }
                
                // 获取连接地址和本地地址
                printAddress(client_fd, port, server_fd, address);
            }
            // 如果是已连接 socket 上有数据可读
            else {
                // 读取数据
                char buffer[MAX_BUFFER_SIZE]={0};
                ssize_t numbytes = event_recv(fd, buffer, MAX_BUFFER_SIZE);
                if (numbytes <= 0) {
                    // 发生错误或连接关闭，从 epoll 实例中移除该 socket
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    printf("TCP server fd:(%d) client exit!\n", fd);
                } else {
                    // 发送数据
                    event_send(fd, buffer, strlen(buffer));
                }
            }
        }
    }

EXIT:
    fprintf(stdout, "TCP server exit\n");
    close(epoll_fd);
    close(server_fd);
    server_run_flag = 0;
    return 0;
}

#include <signal.h>
// 在文件开头添加此声明
void error_log(const char *message)
{
    printf("Error message: %s\n", message);
}

// 通过忽略这两个信号，可以在终端关闭或控制终端断开时继续运行进程，以及在向已关闭的管道或套接字写入时不会导致进程退出。
void ignore_signal()
{
    //ignore SIGHUP and SIGPIPE
    if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
    {
        error_log("signal ignore SIGHUP");
    }
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        error_log("signal ignore SIGPIPE");
    }
}

// 客户端断掉，服务端收到长度为0的消息
// 服务端断掉，客户端收到长度为0的消息
int main(int argc, char* argv[]) 
{
    tcpServer(SERVER_PORT);
    return 0;
}
