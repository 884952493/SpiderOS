#include "print.h"
#include "init.h"
#include "debug.h"
#include "string.h"
#include "memory.h"
#include "../thread/thread.h"
#include "interrupt.h"

void test_thread1(void* arg);
void test_thread2(void* arg);

int main(void) {
   put_str("I am kernel\n");
   init_all();
   thread_start("kernel_thread_a",31,test_thread1,"Arga ");
   thread_start("kernel_thread_b",31,test_thread2,"Argb ");
   intr_enable();
   
   while(1)
   {
   	intr_disable();
   	put_str("Main ");
   	intr_enable();
   }
   return 0;
}

void test_thread1(void* arg)
{
    while(1)
    {
    	intr_disable();
    	put_str((char*)arg);
    	intr_enable();
    }
}

void test_thread2(void* arg)
{
    while(1)
    {
    	intr_disable();
    	put_str((char*)arg);
    	intr_enable();
    }
}
