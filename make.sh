dd if=/dev/zero of=hd60M.img bs=1M count=60
nasm -I include/ -o boot/mbr.bin boot/mbr.S
dd if=boot/mbr.bin of=hd60M.img conv=notrunc
nasm -I include/ -o boot/loader.bin boot/loader.S
dd if=boot/loader.bin of=hd60M.img bs=512 count=3 seek=2 conv=notrunc
nasm -f elf -o lib/kernel/print.o lib/kernel/print.S
gcc -m32 -I lib/kernel -I lib/ -c -o build/timer.o device/timer.c
gcc -m32 -I lib/kernel/ -m32 -I lib/ -m32 -I kernel/ -c -fno-builtin -o build/main.o kernel/main.c
gcc -m32 -I lib/kernel/ -m32 -I lib/ -m32 -I kernel/ -c -fno-builtin -o build/interrupt.o kernel/interrupt.c
gcc -m32 -I lib/kernel/ -m32 -I lib/ -m32 -I kernel/ -c -fno-builtin -o build/init.o kernel/init.c
nasm -f elf -o build/print.o lib/kernel/print.S
nasm -f elf -o build/kernel.o kernel/kernel.S
ld -m elf_i386 -Ttext 0xc0001500 -e main -o build/kernel.bin build/main.o build/init.o build/interrupt.o  \
build/print.o build/kernel.o build/timer.o
dd if=build/kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc