#include "print.h"
#include "init.h"
#include "debug.h"
#include "string.h"
#include "memory.h"
#include "../thread/thread.h"
#include "interrupt.h"
#include "../device/console.h"
#include "../device/ioqueue.h"
#include "../device/keyboard.h"
#include "../device/timer.h"
#include "../userprog/process.h"
#include "../lib/user/syscall.h"
#include "../userprog/syscall-init.h"
#include "../lib/stdio.h"
#include "../lib/kernel/stdio-kernel.h"
#include "../fs/fs.h"
#include "../fs/file.h"
#include "../shell/shell.h"
#include "global.h"
// int main(void) {
//     put_str("I am kernel\n");
//     init_all();
//     intr_enable();
    
//     console_put_str("[SpiderOS@hutaotao /]:~$ ");
    
//     while(1);
//     return 0;
// }

//测试用例1：内存管理功能验证
// int main(void) 
// {
// init_all();
// intr_enable();
// void* addr1 = get_kernel_pages(3);
// put_str("kernel malloc addr1: ");
// put_int((uint32_t)addr1);
// put_char('\n');
// user_mem_test();
// void* addr2 = get_user_pages(1);
// put_str("user malloc addr2: ");
// put_int((uint32_t)addr2);
// put_char('\n');

// while(1);
// return 0;
// }
// void user_mem_test() {
// struct task_struct* cur = running_thread();
// cur->userprog_vaddr.vaddr_start = USER_VADDR_START;
// uint32_t bitmap_pg_cnt = DIV_ROUND_UP((0xc0000000 - USER_VADDR_START) / PG_SIZE / 8, PG_SIZE);
// cur->userprog_vaddr.vaddr_bitmap.bits = get_kernel_pages(bitmap_pg_cnt);
// cur->userprog_vaddr.vaddr_bitmap.btmp_bytes_len = (0xc0000000 - USER_VADDR_START) / PG_SIZE / 8;
// bitmap_init(&cur->userprog_vaddr.vaddr_bitmap);
// put_str("[+] userprog_vaddr bitmap init success!\n");
// }

//测试用例2：线程与进程调度机制验证
void thread_a(void* arg);
void thread_b(void* arg);
int main(void) 
{
init_all();
intr_enable();
thread_start("thread_a", 31, thread_a, NULL);
thread_start("thread_b", 31, thread_b, NULL);
while (1) {
put_str("Main thread is running\n");
}
return 0;
}

void thread_a(void* arg) {
    while (1) {
    put_str("Thread A is running\n");
    }
    }
// 测试用线程B
void thread_b(void* arg) {
while (1) {
put_str("Thread B is running\n");
}
}


// void file_system_test() {
//     put_str("\n[+] begin test filesystem...\n");
    
    // if (sys_mkdir("/testdir") == 0) {
    // put_str("[OK] directory /testdir create success!\n");
    // } else {
    // put_str("[ERR] directory /testdir create fail!\n");
    // }
    // int fd = sys_open("/testdir/hello.txt", O_CREAT | O_RDWR);
    // if (fd != -1) {
    // put_str("[OK] file /testdir/hello.txt create success,fd=");
    // put_int(fd);
    // put_char('\n');
    // char write_buf[] = "SpiderOS FileSystem Test!";
    // if (sys_write(fd, write_buf, strlen(write_buf)) == (int32_t)strlen(write_buf)) {
    // put_str("[OK] file input success。\n");
    // } else {
    // put_str("[ERR] file input fail!\n");
    // }
    // sys_close(fd);
    // } else {
    // put_str("[ERR] fail /testdir/hello.txt create fail！\n");
    // }
    // fd = sys_open("/testdir/hello.txt", O_RDWR);
    // if (fd != -1) {
    // char read_buf[32] = {0};
    // sys_read(fd, read_buf, 31);
    // put_str("[OK] file read success:");
    // put_str(read_buf);
    // put_char('\n');
    // sys_close(fd);
    // } else {
    // put_str("[ERR] file read fail,can't open file。\n");
    // }
    // int wrong_fd = sys_open("/noexist/file", O_RDWR);
    // if (wrong_fd == -1) {
    // put_str("[OK] Illegal path processed correctly, failed to open non-existent file.\n");
    // } else {
    // put_str("[ERR] Illegal path error, file handle should be -1\n");
    // sys_close(wrong_fd);
    // }
    // put_str("[+] File system functionality testing completed\n");
    // }
void init(void)
{
    uint32_t ret_pid = fork();
    if(ret_pid)
        while(1);
    else
        my_shell();
    PANIC("init: should not be here");
}