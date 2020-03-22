#### 信号
- 信号是由用户，系统或者进程发送给目标进程的信息，以通知目标进程某个状态的改变或系统异常
- 简而言之，信号是一种软件中断，提供了一种处理异步的方法，信号的发生是随机的
#### 信号的来源
- Linux 信号可由如下条件产生
  - 对于前台进程，用户可以通过输入特殊的终端字符来给它发送信号。比如输入 Ctrl + C 通常会给进程发送一个中断信号
  - 系统异常。比如浮点异常和非法内存段访问
  - 系统状态变化。比如alram定时器到期将引起SIGALRM信号
  - 运行kill命令或调用kill函数

#### 信号种类
- 信号分为可靠信号与不可靠信号。可靠信号又称为实时信号，非可靠信号又称为非实时信号
- 信号代码从 1 - 32是不可靠信号，不可靠信号主要有以下问题
  - 每次信号处理完之后，就会恢复成默认处理，这可能是调用者不希望看到的
  - 存在信号丢失问题
    - 现在的Linux 对信号机制进行了改进，因此，不可靠信号主要是指信号丢失
- 信号代码从 SIGRTMIN 到 SIGRTMAX之间的信号是可靠信号
  - 可靠信号不存在丢失，由sigueue发送，可靠信号支持排队
- 可靠信号注册机制
  - 内核每收到一个可靠信号都会去注册这个信号，在信号的未决信号链中分配sigqueue 结构，因此，不会存在信号丢失的问题
- 不可靠信号注册机制
  - 而对于不可靠的信号，如果内核已经注册了这个信号，那么便不会再去注册，对于进程来讲，便不会知道本次信号的产生
- 可靠信号与不可靠信号与发送函数没有关系，而是取决于信号代码，前面的32种信号就是不可靠信号，而后面的32种信号就是可靠信号

#### 信号处理函数过程
- 注册信号处理函数
  - 信号的处理是有内核代理的，首先程序通过sigal 或 sigaction函数为每个信号注册处理函数
  - 而内核中维护一张信号向量表，对应信号处理机制
  - 这样，信号在进程中注销完毕之后，会调用相应处理函数进行处理
- 信号的检测和响应时机
  - 在系统调用或中断返回用户态的前夕，内核会检查未决信号集，进行相应的信号处理
- 处理过程
  - 过程
    - 程序运行在用户态时
    -  -> 进程由于系统调用或中断进入内核
    -  -> 转向用户态执行信号处理函数
    -  -> 信号处理函数完毕后进入内核
    -  -> 返回用户态继续执行程序
  - 首先程序执行在用户态，在进程陷入并从内核返回的前夕，会去检查有没有信号没有被处理
  - 如果有且没有被阻塞就会调用相应的信号处理函数程序去处理
  - 首先，内核在用户栈上创建一个层，该层中将返回地址设置成信号处理函数的地址
  - 这样，从内核返回用户态时，就会执行这个信号处理函数
  - 当信号处理函数执行完，会再次进入内核，主要是检测有没有信号没有处理，以及恢复原先程序中断执行点，恢复内核等工作，这样，当从内核返回后到原先程序执行的地方


#### 信号表
#### 信号默认动作
|动作|解释|
| ----- | ---- |
|Term|终止执行|
|Ign|忽略|
|Code|终止执行并产生coredump|
|Stop|停止运行|
|Cont|继续运行|

##### 完整信号表
|序号 | 信号名 | 信号解释 | 默认处理|
| ----- | ---- | ---- |---- |
|1 | SIGHUP | 终端的挂断或进程死亡 | Term |
|2 | SIGINT| 来自键盘的中断信号, Ctrl+c | Term |
|3 | SIGQUIT| 来自键盘的离开信号,Ctrl+\ | Core |
|4 | SIGILL| 非法指令| Core|
|6 | SIGABRT| 来自 abort 的异常信号| Core |
|8| SIGFPE| 浮点例外| Core |
|9 | SIGKILL| 不可以被捕获或忽略的终止信号 | Term |
|11 | SIGSEGV| 段非法错误 (内存引用无效)=>Segmentation error | Core |
|13 | SIGPIPE| 管道损坏：向一个没有读进程的管道写数据 | Term |
|14 | SIGALRM| 来自 alarm 的计时器到时信号生 | Term |
|15 | SIGTERM| 可以被捕获或忽略的终止信号 | Term |
|10, 30, 16 | SIGUSR1| 用户定义信号1 | Term |
|12, 17, 31 | SIGUSR2| 用户定义信号1 | Term) |
|17, 18, 20 | SIGCHLD| 子进程停止或终止 | Ign|
|18, 19, 25| SIGCONT| 如果停止，继续执行| Cont |
|17, 19, 23 | SIGSTOP | 非来自终端的停止信号 | Stop |
|18, 20, 24| SIGTSTP | 来自终端的停止信号| Stop |
|21, 21, 26| SIGTTIN | 后台进程读终端 | Stop |
|22, 22, 27	| SIGTTOU | 后台进程写终端 | Stop |
|7, 10, 10| SIGBUS | 总线错误（内存访问错误） | Core |
|| SIGPOLL | Pollable 事件发生 (Sys V)，与 SIGIO 同义| Term |
|27, 27, 29| SIGPROF | 统计分布图用计时器到时 | Term |
|12, -, 12| SIGSYS | Core | 非法系统调用 (SVr4) |
|5 | SIGSYS | 非法系统调用 (SVr4) | Core |
|16, 21, 23 | SIGURG | socket 紧急信号 (4.2BSD) | Ign |
|26, 26, 28| SIGVTALRM | 虚拟计时器到时 (4.2BSD) | Term |
|24 ,24, 30| SIGXCPU | 超过文件长度限制 (4.2BSD)| Core |
|25, 25, 31| SIGXFSZ | 超过文件长度限制 (4.2BSD) | Core |
|6| SIGIOT | IOT 自陷，与 SIGABRT 同义 | Core |
|7, -, 7| SIGEMT | Term ||
|-, 16, -|SIGSTKFLT| 协处理器堆栈错误 (不使用)| Term|
|22, 23, 29|SIGIO|描述符上可以进行 I/O 操作|Term|
|-, -, 18|SIGCLD|与 SIGCHLD 同义|Ign|
|19, 29, 30|SIGPWR|电力故障 (System V)|Term|
|29, -, -|SIGINFO|与 SIGPWR 同义||
|-, -, -|SIGLOST|文件锁丢失|Term|
|20, 28, 28|SIGWINCH|窗口大小改变 (4.3BSD, Sun)|Ign|
|-, 31, -|SIGUNUSED|未使用信号 (will be SIGSYS)|Term|

##### 非可靠信号

|名称|说明|
| ----- | ---- |
|SIGHUP|	连接断开|
|SIGINT|	终端中断符|
|SIGQUIT|	终端退出符|
|SIGILL|	非法硬件指令|
|SIGTRAP|	硬件故障|
|SIGABRT|	异常终止 (abort)|
|SIGBUS|	硬件故障|
|SIGFPE|	算术异常|
|SIGKILL|	终止|
|SIGUSR1|	用户定义信号|
|SIGUSR2|	用户定义信号|
|SIGSEGV|	无效内存引用|
|SIGPIPE|	写至无读进程的管道|
|SIGALRM|	定时器超时 (alarm)|
|SIGTERM|	终止|
|SIGCHLD|	子进程状态改变|
|SIGCONT|	使暂停进程继续|
|SIGSTOP|	停止|
|SIGTSTP|	终端停止符|
|SIGTTIN|	后台读控制 tty|
|SIGTTOU|	后台写向控制 tty|
|SIGURG|	紧急情况 (套接字)|
|SIGXCPU|	超过 CPU 限制 (setrlimit)|
|SIGXFSZ|	超过文件长度限制 (setrlimit)|
|SIGVTALRM|	虚拟时间闹钟 (setitimer)|
|SIGPROF|	梗概时间超时 (setitimer)|
|SIGWINCH|	终端窗口大小改变|
|SIGIO|	异步 I/O|
|SIGPWR|	电源失效 / 重启动|
|SIGSYS|	无效系统调用|

##### 可靠信号
|名称|用户自定义|
| ----- | ---- |
|SIGRTMIN	||
|SIGRTMIN+1	||
|SIGRTMIN+2	||
|SIGRTMIN+3	||
|SIGRTMIN+4	||
|SIGRTMIN+5	||
|SIGRTMIN+6	||
|SIGRTMIN+7	||
|SIGRTMIN+8	||
|SIGRTMIN+9	||
|SIGRTMIN+10|	|
|SIGRTMIN+11||
|SIGRTMIN+12|	|
|SIGRTMIN+13||
|SIGRTMIN+14|	|
|SIGRTMIN+15||	
|SIGRTMAX-14||	
|SIGRTMAX-13||
|SIGRTMAX-12||	
|SIGRTMAX-11||	
|SIGRTMAX-10||	
|SIGRTMAX-9	||
|SIGRTMAX-8	||
|SIGRTMAX-7	||
|SIGRTMAX-6	||
|SIGRTMAX-5	||
|SIGRTMAX-4	||
|SIGRTMAX-3	||
|SIGRTMAX-2	||
|SIGRTMAX-1	||
|SIGRTMAX||