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
#include "../userprog/process.h"
#include "../lib/user/syscall.h"
#include "../userprog/syscall-init.h"
#include "../lib/stdio.h"

int a=0,b=0;
void test_thread1(void* arg);
void test_thread2(void* arg);
void u_prog_a(void);
void u_prog_b(void);

int main(void) {
   put_str("I am kernel\n");
   init_all();
   process_execute(u_prog_a,"user_prog_a");
   process_execute(u_prog_b,"user_prog_b");
   intr_enable();
   console_put_str(" i am main:0x");
   printf("%x",getpid());
   console_put_char('\n');
   thread_start("kernel_thread_a",31,test_thread1," i am thread_A:0x");
   thread_start("kernel_thread_b",31,test_thread2," i am thread_B:0x");
   
   while(1);
   return 0;
}

void test_thread1(void* arg)
{
    console_put_str((char*)arg);
    printf("%x",getpid());
    console_put_char('\n');
    while(1);
}

void test_thread2(void* arg)
{
    console_put_str((char*)arg);
    printf("%x",getpid());
    console_put_char('\n');
    while(1);
}

void u_prog_a(void)
{
    printf(" i am %s prog_a_pid:%d%c","prog_a",getpid(),'\n');
    while(1);
}

void u_prog_b(void)
{
    printf(" i am %s prog_b_pid:%d%c","prog_b",getpid(),'\n');
    while(1);
}
