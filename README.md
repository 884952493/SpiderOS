# SpiderOS

## 模块说明：

### boot/ 引导模块
mbr.S / mbr.bin

功能：主引导记录（MBR）程序，加载操作系统的第二阶段（loader）。

特点：512 字节大小，末尾有引导标志 0x55AA，包含简单的分区引导逻辑。

loader.S / loader.bin

功能：内核加载器，负责将内核从磁盘加载到内存，并跳转至内核入口。

特点：设置保护模式、初始化 GDT 和分页、加载内核映像。


### device/ 设备驱动模块
console.c/h

功能：提供控制台输出接口，支持互斥打印。

ide.c/h

功能：IDE 硬盘驱动，实现磁盘识别、读写、分区扫描等功能。

keyboard.c/h

功能：键盘中断处理及按键缓冲，向 I/O 队列传递扫描码。

ioqueue.c/h

功能：环形 I/O 队列，实现键盘输入缓存。

timer.c/h

功能：初始化 8253 可编程定时器，周期性时钟中断及线程时间片管理。

### fs/ 文件系统模块
你已说明不需要函数逐个解释，我已认真阅读所有文件。以下仅为模块总结。

实现了 FAT 类似的文件系统，支持：

分区管理与加载（super_block.h）

i节点与目录结构（inode.*, dir.*）

文件读写、打开、关闭、创建（file.*）

文件系统初始化、格式化、路径解析等（fs.*）

### kernel/ 内核核心模块
interrupt.c/h

功能：中断描述符表（IDT）初始化与中断服务处理框架。

memory.c/h

功能：内存管理，包括物理页分配、虚拟内存管理、内核堆分配（sys_malloc）等。

init.c/h

功能：初始化内核的各个子系统。

main.c

功能：内核主程序入口，测试并调用核心功能。

debug.c/h

功能：断言与调试信息输出。

kernel.S

功能：内核入口汇编代码（C语言入口之前）。

### thread/ 线程调度模块
thread.c/h

功能：线程控制块管理、线程创建、上下文切换、调度机制实现。

sync.c/h

功能：实现信号量和锁，支持线程同步与互斥。

switch.S

功能：实现线程上下文切换的底层汇编代码。

### userprog/ 用户进程管理模块
process.c/h

功能：用户进程创建与页表构造，切换用户模式。

tss.c/h

功能：任务状态段 TSS 管理，支持从用户态切换到内核态。

syscall-init.c/h

功能：系统调用初始化，映射用户调用到内核服务。

syscall.c

功能：系统调用服务例程，如 malloc, write, getpid, fork 等。

exec.c/h, fork.c/h

功能：程序替换与复制实现，支持用户态 exec, fork 语义。

### shell/ 命令行终端模块
shell.c/h

功能：基本 shell 循环，实现提示符与命令解析、执行。

buildin_cmd.c/h

功能：实现如 ls, cd, pwd, ps 等内置命令。


### lib/ 通用工具函数库
string.*, stdio.*, stdint.h

功能：实现 C 语言常用的字符串处理、格式化输出等工具函数。

user/ 与 kernel/ 目录下

syscall.h/c 用户接口调用封装；stdio-kernel.c 为内核态的 printk, console_put_str 等接口。

kernel/print.S

功能：字符输出与格式化显示底层实现，控制显存和光标。

### 根目录配置
Makefile

功能：自动化编译项目，构建操作系统二进制映像。

bochsrc.disk

功能：Bochs 配置文件，定义硬盘映像和启动方式。

脚本：make.sh, run.sh, xxd.sh

功能：编译、运行、查看磁盘二进制信息的辅助脚本。