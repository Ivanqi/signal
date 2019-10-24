#include <stdio.h>
#include <signal.h>

/**
 * 1. 结构体
    struct sigaction {
        void (*sa_handler)(int);
        void (*sa_sigaction)(int,siginfo_t*,void*);
        sigset_t sa_mask;
        int sa_flags;
        void (*sa_restorer)(void);
    }
 * 2. 参数含义
 *  1. sa_handler
 *      1. 一个函数指针，主要是表示接收到信号时所要采取的行动
 *      2. 此字段的值可以是 SIG_DFL, SIG_IGN.分别代表默认操作与内核将忽略进程信号
 *      3. 这个函数只传递一个参数，那就是信号代码
 *      4. 当SA_SIGINFO被设定在sa_flags中，那么则会使用sa_sigaction来指示信号处理函数，而非sa_handler
 *  2. sa_mask 设置了掩码集合，在程序执行期间会阻挡掩码集中的信号
 *  3. sa_flags
 *      1. 设置了一些标志，SA_RESETHAND当该函数处理完成之后，设定为系统默认的处理模式
 *      2. SA_NODEFER 在处理函数中，如果再次到达此信号时，将不会阻塞
 *      3. 默认情况下，同一信号两次达到时，如果此时处于信号处理程序中，那么此信号将会阻塞
 *      4. SA_SIGINFO 表示用sa_sigaction指示的函数
 *  4. sa_restorer 已经被废弃
 * 
 * 3. siginfo_t 结构体
    typedef struct siginfo_t{
        int si_signo;       //  信号编号
        int si_errno;       //  如果为非零值则错误代码与之关联
        int si_code;        //  说明进程如何接收信号以及从何处收到
        pid_t si_pid;       //  适用于SIGCHLD，代表被终止进程的PID
        pid_t si_uid;       //  适用于SIGCHLD,代表被终止进程所拥有进程的UID
        int si_status;      //  适用于SIGCHLD，代表被终止进程的状态
        clock_t si_utime;   //  适用于SIGCHLD，代表被终止进程所消耗的用户时间
        clock_t si_stime;   //  适用于SIGCHLD，代表被终止进程所消耗系统的时间
        sigval_t si_value;
        int si_int;
        void * si_ptr;
        void* si_addr;
        int si_band;
        int si_fd;
    };
 */

void signal_set1(int);  // 信号处理函数，只传递一个参数代码
void signal_set(struct sigaction *act)
{
    switch(act->sa_flags) {
        case (int)SIG_DFL: 
            printf("using default hander \n");
            break;
        case (int)SIG_IGN:
            printf("ignore the signal \n");
            break;
        default:
            printf("%0x\n", act->sa_handler);
    }
}

void signal_set1(int x) // 信号处理函数
{
    printf("xxxx\n");
    while (1) {
        printf("signal_set1\n");
        sleep(1);
    }
}

int main (int argc, char **argv)
{
    int i;
    struct sigaction act, oldact;
    act.sa_handler = signal_set1;
    act.sa_flags = SA_RESETHAND;

    /**
     * 1. SA_RESETHANDD 在处理完信号之后，将信号恢复成默认处理
     * 2. SA_NODEFER 在信号处理程序执行期间仍然可以接收信号
     */
    sigaction(SIGINT, &act, &oldact);   // 改变信号的处理模式

    for (i = 1; i < 12; i++) {
        printf("signal %d handler is :", i);
        sigaction(i, NULL, &oldact);
        /**
         * 1. 如果act为NULL， oldact 会存储信号当前的行为
         * 2. act不为空，oldact不为空，则oldact会存储信号以前的处理模式
         */
        signal_set(&oldact); 
    }

    while(1) {
        // 等待信号的到来
        sleep(1);
    }

    return 0;
}