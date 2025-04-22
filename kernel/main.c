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
int main(void) {
    put_str("I am kernel\n");
    init_all();
    intr_enable();
    
    console_put_str("[SpiderOS@hutaotao /]:~$ ");
    
    while(1);
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