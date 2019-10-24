#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void myhandler(int signo, siginfo_t *si, void *ucontext);

// sigqueue 向本进程发送数据信息
int main () {

    union sigval val;   // 定义一个携带数据的共用体
    struct sigaction oldact, act;

    act.sa_sigaction = myhandler;
    // 表示使用sa_sigaction 指示的函数，处理完恢复默认，不阻塞处理过到达的被处理的信号
    act.sa_flags = SA_SIGINFO;  

    // 注册信号处理函数
    sigaction(SIGUSR1, &act, &oldact);
    char data[100];
    int num = 0;
    while (num < 10) {
        sleep(2);
        printf("等待SIGUSR1 信号的到来\n");
        sprintf(data, "%d", num++);
        val.sival_ptr = data;
        sigqueue(getpid(), SIGUSR1, val);   // 向本进程发送一个信号
    }
}

void myhandler(int signo, siginfo_t *si, void *ucontext)
{
    printf("已经接收SIGUSR1信号\n");
    printf("%s\n", (char *)(si->si_ptr));
}