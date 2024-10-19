/*
 * 文件名: client.c
 * 作者: ljk
 * 创建时间: 2023-07-22
 * 文件描述: TCP通信客户端
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "common_socket.h"



// 客户端 运行状态 0：准备 1：运行 2：连接成功
static int client_run_flag = 0;
// 全局客户端通信描述符
static int fd = 0;

// TCP client 连接
static int tcpClient(char* ip, int port) 
{
    int client_fd = -1;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // 创建套接字
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        goto EXIT;
    }

    // 设置服务器地址和端口号
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    bzero(&(serv_addr.sin_zero),sizeof(serv_addr.sin_zero));
   
    // 连接服务器
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0) {
        printf("\nConnection Failed \n");
        goto EXIT;
    }

    fd = client_fd;
    client_run_flag = 2;
    // 发送数据
    while (client_run_flag)
    {
        bzero(buffer, MAX_BUFFER_SIZE); 
        ssize_t numbytes = event_recv(client_fd, buffer, MAX_BUFFER_SIZE);
        
        if (numbytes <= 0)
        {  
            fprintf(stderr,"TCP server close!\n");
            //goto EXIT;
            exit(EXIT_FAILURE);
        } 

        buffer[numbytes] = '\0'; 
        //printf("fd:(%d) Received message: %s\n", client_fd, buffer);
    }

EXIT:
    fprintf(stdout, "TCP client exit\n");
    close(client_fd);
    fd = -1; 
    client_run_flag = 0;
    return 0;
}

// client 创建接收线程
void* thread_handle_Client(void* arg)
{
    pthread_detach(pthread_self());
    if(client_run_flag == 0)
    {
        client_run_flag = 1;
        tcpClient(SERVER_ADDRESS, SERVER_PORT);
    }
}

// 输入发送字符串
static int input_send()
{
    sleep(1);
    while(client_run_flag == 2)
    {
        char msg[100];
        int len = 0;
        scanf("%s",msg); 
        len = strlen(msg);
        //sent to the server
        event_send(fd, (const void *)msg, len);
    }
    return 0;
}

// 测试程序
int main(int argc, char* argv[])
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_handle_Client, NULL);
    input_send();
    return 0;
}