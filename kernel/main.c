#include "print.h"
#include "init.h"
#include "debug.h"
#include "string.h"
#include "memory.h"
#include "../thread/thread.h"
void test_thread(void* arg);

int main(void) {
   put_str("I am kernel\n");
   init_all();
   thread_start("kernel_thread_a",31,test_thread,"argA ");
   while(1);
   return 0;
}

void test_thread(void* arg)
{
    while(1)
    	put_str((char*)arg);
}
