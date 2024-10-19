/*
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信信号量例程
 */
 #include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define MAX_BUF_SIZE 1024

// 根据进程名获取进程PID
int get_pid_by_name(const char *process_name) 
{
    DIR *dir;
    struct dirent *entry;
    char path[MAX_BUF_SIZE];
    
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
            FILE *fp = fopen(path, "r");
            if (fp) {
                char cmdline[MAX_BUF_SIZE];
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                
                if (strstr(cmdline, process_name) != NULL) {
                    closedir(dir);
                    return atoi(entry->d_name);
                }
            }
        }
    }
    
    closedir(dir);
    return -1; // 进程未找到
}

int main(int argc, char* argv[]) 
{
    pid_t receiver_pid = get_pid_by_name("linux_ipc_sig_read");  // 替换为接收方进程的实际PID
    int signal_number = SIGUSR1; // 使用SIGUSR1信号作为示例

    if(receiver_pid)
    {
        printf("Sender: Sending signal to process %d\n", receiver_pid);
        if (kill(receiver_pid, signal_number) == 0) {
            printf("Sender: Signal sent successfully!\n");
        } else {
            perror("Sender: Error sending signal");
            return 1;
        }
    }
    else{
        printf("未找到进程\n");
    }
    
    return 0;
}