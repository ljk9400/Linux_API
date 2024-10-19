#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>        // For uint64_t
#include <sys/timerfd.h>
#include <poll.h>
#include <string.h>        // For strerror()
#include <errno.h>



int timer_poll(int cnt) {
    // 创建定时器
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    if (timerfd == -1) {
        perror("timerfd_create");
        return -1;
    }

    // 设置定时器时间为5秒后超时
    struct itimerspec timerValue;
    memset(&timerValue, 0, sizeof(struct itimerspec));
    timerValue.it_value.tv_sec = cnt;     // 定时器5秒后到期

    if (timerfd_settime(timerfd, 0, &timerValue, NULL) == -1) {
        perror("timerfd_settime");
        close(timerfd);
        return -1;
    }

    // 初始化poll结构体
    struct pollfd fds;
    fds.fd = timerfd;
    fds.events = POLLIN;

    // 等待定时器超时
    printf("开始等待%d秒...\n", cnt);
    int ret = poll(&fds, 1, -1); // 无限等待，直到定时器超时
    if (ret == -1) {
        perror("poll");
        close(timerfd);
        return -1;
    }

    if (fds.revents & POLLIN) {
        printf("定时器超时！\n");
        // 读取定时器超时次数来重置定时器
        uint64_t expirations;
        ssize_t s = read(timerfd, &expirations, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
            perror("read");
            close(timerfd);
            return -1;
        }
        // 处理定时器超时事件
        printf("定时器超时次数：%llu\n", (unsigned long long) expirations);
    }

    // 关闭文件描述符
    close(timerfd);
    return 0;
}

// 测试dmeo
int main(int argc, char* argv[]) {
    timer_poll(3);
    return 0;
}