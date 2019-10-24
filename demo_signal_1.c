#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

/**
 * 信号操作函数
 *  1. int sigemptyset(sigset_t *set);  // 初始化信号，集合set并将set设置为空
 *  2. int sigfillset(sigset_t *set);   // 也是初始化信号集合，不过将信号集合设置为所有信号的集合
 *  3. int sigaddset(sigset_t *set, int signo); // 将信号signo加入到信号集合set中去
 *  4. int sigdelset(sigset_t *set, int signo); // 将指定的信号signo从信号集合set中移去
 *  5. int sigsmember(sigset_t *set, int signo);    // 判断此信号是否在信号集合中
 *  6. int sigprocmask(int how, const sigset_t *set, sigset_t *oset);
 *      1. 第一个参数
 *          1. how 有三个取值，SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK
 *          2. SIG_BLOCK: 增加一个信号集合到进程指定的阻塞集合中去
 *          3. SIG_UNBLOCK：从阻塞集合中删除指定的信号集合
 *          4. SIG_SETMASK: 将当前集合设置为进程阻塞的集合
 *      2. 第二个参数
 *          1. 如果oset不空，那么当前进程信号阻塞集合将会保存到oset里面
 */

int main () {

    sigset_t initset;
    int i;
    sigemptyset(&initset);          // 初始化信号集合为空集合
    sigaddset(&initset, SIGINT);    // 将SIGINT信号加入到此集合中去

    while(1) {
        sigprocmask(SIG_BLOCK, &initset, NULL); // 将信号集合加入到进程的阻塞集合中去
        fprintf(stdout, "SIGINT singal blocked/n");

        for (i = 0; i <10; i++) {
            sleep(1);   // 每1秒输出
            fprintf(stdout, "block %d\n", i);
        }

        // 在这时按一下ctrl + c不会终止
        sigprocmask(SIG_UNBLOCK, &initset, NULL);   // 从进程的阻塞集合中去删除信号集合
        fprintf(stdout, "SIGINT SINGAL unblocked\n");

        for (i = 0; i < 10; i++) {
            sleep(1);
            fprintf(stdout, "unblock %d \n", i);
        }
    }

    exit(0);
}