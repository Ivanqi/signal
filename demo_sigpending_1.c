#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

/**
 * 1. sigpending(sigset_t *set)将未决信号放到指定的set信号集中去，未决信号包括被阻塞的信号和信号到达时但还没来得及处理的信号
 * 2. 此函数功能
 *  1. 先将 SIGRTMIN + 10 阻塞，然后取得未决信号集，判断此信号是否在其中，再解除阻塞
 */

void myhandler(int signo, siginfo_t *si, void *ucontext);

int main () {

    struct sigaction oldact, act;
    sigset_t oldmask, newmask, pendingmask;

    act.sa_sigaction = myhandler;
    act.sa_flags = SA_SIGINFO;
    // 首先将阻塞集合设置为空，即不阻塞任何信号
    sigemptyset(&act.sa_mask);
    // 注册信号处理函数
    sigaction(SIGRTMIN + 10, &act, &oldact);

    // 开始阻塞
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGRTMIN + 10);
    printf("SIGRTMIN + 10 blocked \n");
    
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    sleep(20);  // 为了发出信号
    printf("now begin to get pending mask \n");

    if (sigpending(&pendingmask) < 0) {
        perror("pendingmask error");
    }

    if (sigismember(&pendingmask, SIGRTMIN + 10)) {
        printf("SIGRTMIN + 10 is in the pending mask \n");
    }

    sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);

    printf("SIGRTMIN + 10 unbloked \n");

    return 0;
}

//信号处理函数
void myhandler(int signo, siginfo_t *si, void *ucontext) {
    printf("receive signal %d \n", si->si_signo);
}