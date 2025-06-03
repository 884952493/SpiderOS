      |              ~~~~~~^~~~~~~~
ld -m elf_i386 -Ttext 0xc0001500 -e main -Map ./build/kernel.map build/main.o build/init.o build/interrupt.o build/timer.o build/kernel.o build/print.o build/switch.o build/debug.o build/string.o build/memory.o build/bitmap.o build/thread.o build/list.o build/sync.o build/console.o build/keyboard.o build/ioqueue.o build/tss.o build/process.o build/syscall-init.o build/syscall.o build/stdio.o build/stdio-kernel.o build/ide.o build/fs.o build/inode.o build/file.o build/dir.o build/fork.o build/shell.o build/buildin_cmd.o build/exec.o -o build/kernel.bin
ld: warning: build/switch.o: missing .note.GNU-stack section implies executable stack
ld: NOTE: This behaviour is deprecated and will be removed in a future version of the linker
ld: build/main.o: in function `main':
main.c:(.text+0x32): undefined reference to `getchar'
ld: build/main.o: in function `test_thread_scheduling':
main.c:(.text+0x3ef): undefined reference to `getchar'
ld: build/main.o: in function `test_io_devices':
main.c:(.text+0x63a): undefined reference to `getchar'
ld: build/main.o: in function `test_interrupt':
main.c:(.text+0x6f3): undefined reference to `getchar'
make: *** [Makefile:168：build/kernel.bin] 错误 1