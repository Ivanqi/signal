#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main () {

    union sigval value;
    value.sival_int = 10;

    if (sigqueue(48445, SIGUSR1, value) == -1) {    // 48445 是目标进程pid
        perror("信号发送成功\n");
    }

    sleep(2);
}