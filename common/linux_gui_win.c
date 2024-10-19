#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>


// 获取窗口
void get_win() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0) {
        perror("ioctl");
        return ;
    }
    printf("终端窗口大小：宽 %d, 高 %d\n", ws.ws_col, ws.ws_row);
}

int main() 
{
    get_win();
    return 0;
}