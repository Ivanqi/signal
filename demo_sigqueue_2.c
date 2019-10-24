#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void myhandler(int signo, siginfo_t *si, void *ucontext);

int main () {

    struct sigaction oldact, act;
    act.sa_sigaction = myhandler;
    act.sa_flags = SA_SIGINFO | SA_NODEFER;

    // 表示执行后恢复，用sa_sigaction 指示的处理函数，在执行期间仍然可以接收信号
    sigaction(SIGUSR1, &act, &oldact);

    while(1) {
        sleep(2);
        printf("等待信号的到来\n");
    }
}

void myhandler(int signo, siginfo_t *si, void *ucontext)
{
    printf("the values is %d\n", si->si_int);
}