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

// 测试函数声明
void test_memory_management(void);
void test_thread_scheduling(void);
void test_process_management(void);
void test_file_system(void);
void test_io_devices(void);
void test_interrupt(void);

// 检查是否按下Ctrl+C
bool check_ctrl_c(void) {
    enum intr_status old_status = intr_disable();
    char ch = ioq_getchar(&ioqueue);
    intr_set_status(old_status);
    return (ch == 3); // Ctrl+C的ASCII码是3
}

// 启动日志
void print_boot_log(void) {
    put_str("\n\n");
    put_str("==========================================\n");
    put_str("           SpiderOS Boot Sequence         \n");
    put_str("==========================================\n");
    put_str("[+] Initializing system...\n");
    put_str("[+] Loading kernel...\n");
    put_str("[+] Setting up memory management...\n");
    put_str("[+] Initializing interrupt handlers...\n");
    put_str("[+] Setting up device drivers...\n");
    put_str("[+] Mounting file system...\n");
    put_str("[+] Starting process scheduler...\n");
    put_str("[+] System initialization complete!\n");
    put_str("==========================================\n\n");
}

// 显示菜单
void show_menu(void) {
    put_str("\n=== SpiderOS Test Menu ===\n");
    put_str("1. Memory Management Test\n");
    put_str("2. Thread Scheduling Test\n");
    put_str("3. Process Management Test\n");
    put_str("4. File System Test\n");
    put_str("5. I/O Devices Test\n");
    put_str("6. Interrupt Test\n");
    put_str("0. Exit\n");
    put_str("Please select (0-6): ");
}

// 内存管理测试
void test_memory_management(void) {
    put_str("\n=== Memory Management Test ===\n");
    
    // 测试内核内存分配
    void* kernel_addr = get_kernel_pages(3);
    put_str("[+] Kernel memory allocated at: ");
    put_int((uint32_t)kernel_addr);
    put_char('\n');
    
    // 测试用户内存分配
    void* user_addr = get_user_pages(1);
    put_str("[+] User memory allocated at: ");
    put_int((uint32_t)user_addr);
    put_char('\n');
    
    // 释放内存
    put_str("[+] Memory test completed\n");
}

// 线程调度测试
void thread_a(void* arg) {
    while(1) {
        put_str("Thread A running...\n");
    }
}

void thread_b(void* arg) {
    while(1) {
        put_str("Thread B running...\n");
    }
}

void test_thread_scheduling(void) {
    put_str("\n=== Thread Scheduling Test ===\n");
    put_str("Press Ctrl+C to return to menu\n");
    thread_start("thread_a", 31, thread_a, NULL);
    thread_start("thread_b", 31, thread_b, NULL);
    put_str("[+] Threads started\n");
    while(1) {
        if(check_ctrl_c()) {
            put_str("\nReturning to menu...\n");
            return;
        }
        if(ioq_getchar(&ioqueue) != -1) break;
    }
}

// 进程管理测试
void test_process_management(void) {
    put_str("\n=== Process Management Test ===\n");
    put_str("Press Ctrl+C to return to menu\n");
    uint32_t pid = fork();
    if(pid) {
        put_str("[+] Parent process running, pid: ");
        put_int(pid);
        put_char('\n');
    } else {
        put_str("[+] Child process running\n");
    }
    while(1) {
        if(check_ctrl_c()) {
            put_str("\nReturning to menu...\n");
            return;
        }
    }
}

// 文件系统测试
void test_file_system(void) {
    put_str("\n=== File System Test ===\n");
    put_str("Press Ctrl+C to return to menu\n");
    
    // 创建目录
    if(sys_mkdir("/testdir") == 0) {
        put_str("[+] Directory /testdir created successfully\n");
    }
    
    // 创建文件
    int fd = sys_open("/testdir/test.txt", O_CREAT | O_RDWR);
    if(fd != -1) {
        put_str("[+] File created successfully\n");
        
        // 写入数据
        char* test_data = "Hello SpiderOS!";
        sys_write(fd, test_data, strlen(test_data));
        
        // 读取数据
        char read_buf[32] = {0};
        sys_read(fd, read_buf, 31);
        put_str("[+] File content: ");
        put_str(read_buf);
        put_char('\n');
        
        sys_close(fd);
    }
    
    while(1) {
        if(check_ctrl_c()) {
            put_str("\nReturning to menu...\n");
            return;
        }
    }
}

// I/O设备测试
void test_io_devices(void) {
    put_str("\n=== I/O Devices Test ===\n");
    put_str("Press Ctrl+C to return to menu\n");
    put_str("[+] Testing keyboard input...\n");
    put_str("Please type something: ");
    
    char input[32] = {0};
    int i = 0;
    while(i < 31) {
        if(check_ctrl_c()) {
            put_str("\nReturning to menu...\n");
            return;
        }
        enum intr_status old_status = intr_disable();
        char ch = ioq_getchar(&ioqueue);
        intr_set_status(old_status);
        if(ch != -1) {
            input[i++] = ch;
            put_char(ch);
        }
    }
    put_str("\n[+] Input received: ");
    put_str(input);
    put_char('\n');
    
    while(1) {
        if(check_ctrl_c()) {
            put_str("\nReturning to menu...\n");
            return;
        }
    }
}

// 中断测试
void test_interrupt(void) {
    put_str("\n=== Interrupt Test ===\n");
    put_str("Press Ctrl+C to return to menu\n");
    put_str("[+] Testing timer interrupt...\n");
    put_str("[+] System will beep every second\n");
    
    // 启用定时器中断
    timer_init();
    while(1) {
        if(check_ctrl_c()) {
            put_str("\nReturning to menu...\n");
            return;
        }
        enum intr_status old_status = intr_disable();
        if(ioq_getchar(&ioqueue) != -1) {
            intr_set_status(old_status);
            break;
        }
        intr_set_status(old_status);
    }
}

int main(void) {
    // 初始化所有模块
    init_all();
    intr_enable();
    
    // 显示启动日志
    print_boot_log();
    
    // 主菜单循环
    while(1) {
        show_menu();
        char choice = ioq_getchar(&ioqueue);
        put_char('\n');
        
        switch(choice) {
            case '1':
                test_memory_management();
                break;
            case '2':
                test_thread_scheduling();
                break;
            case '3':
                test_process_management();
                break;
            case '4':
                test_file_system();
                break;
            case '5':
                test_io_devices();
                break;
            case '6':
                test_interrupt();
                break;
            case '0':
                put_str("Exiting...\n");
                return 0;
            default:
                put_str("Invalid choice, please try again\n");
        }
    }
    
    return 0;
}

void init(void)
{
    uint32_t ret_pid = fork();
    if(ret_pid)
        while(1);
    else
        my_shell();
    PANIC("init: should not be here");
}