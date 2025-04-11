#include "stdio-kernel.h"
#include "stdint.h"
#include "stdio.h"
#include "console.h"

#define va_start(ap,v) ap = (va_list)&v          //这里把第一个char*地址赋给ap 强制转换一下
#define va_arg(ap,t)   *((t*)(ap +=4))	   //强制类型转换 得到栈中参数
#define va_end(ap)	ap = NULL   

void printk(const char* format, ...)
{
    void* args;
    va_start(args,format);
    char buf[1024] = {0};
    vsprintf(buf,format,args);
    va_end(args);
    console_put_str(buf);
}
