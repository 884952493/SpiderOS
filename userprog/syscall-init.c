#include "syscall-init.h"
#include "../lib/user/syscall.h"
#include "stdint.h"
#include "print.h"
#include "debug.h"
#include "interrupt.h"
#include "../thread/thread.h"
#include "console.h"
#include "syscall.h"

#define syscall_nr 32
typedef void* syscall;
syscall syscall_table[syscall_nr];

uint32_t sys_getpid(void)
{
    return running_thread()->pid;
}
 

void sys_write(char* str)
{
    console_put_str(str);
}

void syscall_init(void)
{
    put_str("syscall_init start\n");
    syscall_table[SYS_GETPID] = sys_getpid;
    syscall_table[SYS_WRITE]  = sys_write;
    put_str("syscall_init done\n");
}
