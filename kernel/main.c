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

void test_thread1(void* arg);
void test_thread2(void* arg);

int main(void) {
   put_str("I am kernel\n");
   init_all();
   // thread_start("kernel_thread_a",31,test_thread1," A_");
   // thread_start("kernel_thread_b",31,test_thread2," B_");
   // intr_enable();
   
   while(1);
   return 0;
}

void test_thread1(void* arg)
{
    while(1)
    {
        enum intr_status old_status = intr_disable();
        while(!ioq_empty(&ioqueue))
        {
   	    console_put_str((char*)arg);
    	    char chr = ioq_getchar(&ioqueue);
   	    console_put_char(chr);
	}
   	intr_set_status(old_status);
    }
}

void test_thread2(void* arg)
{
    while(1)
    {
        enum intr_status old_status = intr_disable();
        while(!ioq_empty(&ioqueue))
        {
   	    console_put_str((char*)arg);
    	    char chr = ioq_getchar(&ioqueue);
   	    console_put_char(chr);
	}
   	intr_set_status(old_status);
    }
}
