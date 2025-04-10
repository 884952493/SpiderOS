#include "thread.h"
#include "stdint.h"
#include "string.h"
#include "global.h"
#include "memory.h"
#include "debug.h"
#include "interrupt.h"
#include "print.h"

#define PG_SIZE 4096

struct task_struct* main_thread;
struct list thread_ready_list;
struct list thread_all_list;

extern void switch_to(struct task_struct* cur, struct task_struct* next);

// 获取当前运行线程的 PCB
struct task_struct* running_thread(void) {
    uint32_t esp;
    asm ("mov %%esp, %0" : "=g"(esp));
    return (struct task_struct*)(esp & 0xfffff000);
}

void kernel_thread(thread_func* function, void* func_arg) {
    intr_enable();  // [+] 开中断，允许后续时间中断触发调度
    if (function == NULL) {
        put_str("[-] kernel_thread: function is NULL\n");
        return;
    }
    function(func_arg);  // [+] 执行线程主函数
}

void thread_create(struct task_struct* pthread, thread_func function, void* func_arg) {
    if (pthread == NULL) {
        put_str("[-] thread_create: pthread is NULL\n");
        return;
    }
    pthread->self_kstack -= sizeof(struct intr_struct);
    pthread->self_kstack -= sizeof(struct thread_stack);

    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
    kthread_stack->eip = kernel_thread;
    kthread_stack->function = function;
    kthread_stack->func_arg = func_arg;
    kthread_stack->ebp = kthread_stack->ebx = kthread_stack->edi = kthread_stack->esi = 0;

    put_str("[+] thread_create: kernel stack initialized\n");
}

void init_thread(struct task_struct* pthread, char* name, int prio) {
    if (pthread == NULL || name == NULL) {
        put_str("[-] init_thread: invalid parameters\n");
        return;
    }

    memset(pthread, 0, sizeof(*pthread));
    strcpy(pthread->name, name);

    pthread->status = (pthread == main_thread) ? TASK_RUNNING : TASK_READY;
    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);

    pthread->priority = prio;
    pthread->ticks = prio;
    pthread->elapsed_ticks = 0;
    pthread->pgdir = NULL;
    pthread->stack_magic = 0x23333333;

    put_str("[+] init_thread: initialized thread ");
    put_str(name);
    put_char('\n');
}

struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg) {
    put_str("[+] thread_start: creating thread...\n");

    if (function == NULL) {
        put_str("[-] thread_start error: function is NULL\n");
        return NULL;
    }

    if (name == NULL || name[0] == '\0') {
        put_str("[-] thread_start error: name is NULL or empty\n");
        return NULL;
    }

    struct task_struct* thread = get_kernel_pages(1);
    if (thread == NULL) {
        put_str("[-] thread_start error: get_kernel_pages failed!\n");
        return NULL;
    }

    init_thread(thread, name, prio);
    thread_create(thread, function, func_arg);

    if (elem_find(&thread_ready_list, &thread->general_tag)) {
        put_str("[-] thread_start error: thread already in ready list!\n");
        return NULL;
    }
    list_append(&thread_ready_list, &thread->general_tag);

    if (elem_find(&thread_all_list, &thread->all_list_tag)) {
        put_str("[-] thread_start error: thread already in all list!\n");
        return NULL;
    }
    list_append(&thread_all_list, &thread->all_list_tag);

    put_str("[+] thread_start: thread created successfully\n");
    return thread;
}

void make_main_thread(void) {
    main_thread = running_thread();
    if (main_thread == NULL) {
        put_str("[-] make_main_thread: running_thread returned NULL\n");
        return;
    }

    init_thread(main_thread, "main", 31);

    ASSERT(!elem_find(&thread_all_list,&main_thread->all_list_tag));
    list_append(&thread_all_list,&main_thread->all_list_tag);
    put_str("[+] make_main_thread: main thread initialized\n");
}

void schedule(void) {
    ASSERT(intr_get_status() == INTR_OFF);

    struct task_struct* cur = running_thread();
    if(cur->status == TASK_RUNNING)
    {
    	ASSERT(!elem_find(&thread_ready_list,&cur->general_tag));     //目前在运行的肯定ready_list是不在的
    	list_append(&thread_ready_list,&cur->general_tag);            //加入尾部
    	
    	cur->status = TASK_READY;
    	cur->ticks = cur->priority;
    }
    else
    {}
    ASSERT(!list_empty(&thread_ready_list));

    struct task_struct* thread_tag = list_pop(&thread_ready_list);
    struct task_struct* next = (struct task_struct*)((uint32_t)thread_tag & 0xfffff000);
    next->status = TASK_RUNNING;

    // put_str("[+] schedule: switching to next thread\n");
    switch_to(cur, next);
}

void thread_block(enum task_status stat)
{
    //设置block状态的参数必须是下面三个以下的
    ASSERT(((stat == TASK_BLOCKED) || (stat == TASK_WAITING) || stat == TASK_HANGING));
    
    enum intr_status old_status = intr_disable();			 //关中断
    struct task_struct* cur_thread = running_thread();		 
    cur_thread->status = stat;					 //把状态重新设置
    
    //调度器切换其他进程了 而且由于status不是running 不会再被放到就绪队列中
    schedule();	
    				
    //被切换回来之后再进行的指令了
    intr_set_status(old_status);
}

//由锁拥有者来执行的 善良者把原来自我阻塞的线程重新放到队列中
void thread_unblock(struct task_struct* pthread)
{
    enum intr_status old_status = intr_disable();
    ASSERT(((pthread->status == TASK_BLOCKED) || (pthread->status == TASK_WAITING) || (pthread->status == TASK_HANGING)));
    if(pthread->status != TASK_READY)
    {
    	//被阻塞线程 不应该存在于就绪队列中）
    	ASSERT(!elem_find(&thread_ready_list,&pthread->general_tag));
    	if(elem_find(&thread_ready_list,&pthread->general_tag))
    	    PANIC("thread_unblock: blocked thread in ready_list\n"); //debug.h中定义过
    	
    	//让阻塞了很久的任务放在就绪队列最前面
    	list_push(&thread_ready_list,&pthread->general_tag);
    	
    	//状态改为就绪态
    	pthread->status = TASK_READY;
    }
    intr_set_status(old_status);
}


void thread_init(void) {
    put_str("[+] thread_init start\n");
    list_init(&thread_ready_list);
    list_init(&thread_all_list);
    make_main_thread();
    put_str("[+] thread_init done\n");
}
