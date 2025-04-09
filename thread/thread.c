#include "thread.h"   //函数声明 各种结构体
#include "stdint.h"   //前缀
#include "string.h"   //memset
#include "global.h"   //不清楚
#include "memory.h"   //分配页需要

#define PG_SIZE 4096

void kernel_thread(thread_func* function,void* func_arg)
{
    function(func_arg);
}

void thread_create(struct task_struct* pthread,thread_func function,void* func_arg)
{
    pthread->self_kstack -= sizeof(struct intr_struct);  //减去中断栈的空间
    pthread->self_kstack -= sizeof(struct thread_stack);
    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack; 
    kthread_stack->eip = kernel_thread;                 //地址为kernel_thread 由kernel_thread 执行function
    kthread_stack->function = function;
    kthread_stack->func_arg = func_arg;
    kthread_stack->ebp = kthread_stack->ebx = kthread_stack->ebx = kthread_stack->esi = 0; //初始化一下
    return;
}

void init_thread(struct task_struct* pthread,char* name,int prio)
{
    memset(pthread,0,sizeof(*pthread)); //pcb位置清0
    strcpy(pthread->name,name);
    pthread->status = TASK_RUNNING;
    pthread->priority = prio;
    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE); //刚开始的位置是最低位置 栈顶位置+一页
    pthread->stack_magic = 0x23333333;                               //设置的魔数 检测是否越界限
}

struct task_struct* thread_start(char* name,int prio,thread_func function,void* func_arg)
{
    struct task_struct* thread = get_kernel_pages(1);
    init_thread(thread,name,prio);
    thread_create(thread,function,func_arg);
    
    asm volatile("movl %0,%%esp; pop %%ebp; pop %%ebx; pop %%edi; pop %%esi; ret" : : "g"(thread->self_kstack) :"memory"); //栈顶的位置为 thread->self_kstack 
    return thread;
}
