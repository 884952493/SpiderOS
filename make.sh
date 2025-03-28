dd if=/dev/zero of=hd60M.img bs=1M count=60
nasm -I include/ -o boot/mbr.bin boot/mbr.S
dd if=boot/mbr.bin of=hd60M.img conv=notrunc
nasm -I include/ -o boot/loader.bin boot/loader.S
dd if=boot/loader.bin of=hd60M.img bs=512 count=3 seek=2 conv=notrunc
nasm -f elf -o lib/kernel/print.o lib/kernel/print.S
gcc -m32 -I lib/kernel/ -I lib/ -c -o kernel/main.o kernel/main.c
ld -m elf_i386 -Ttext 0xc0001500 -e main -o kernel/kernel.bin \
kernel/main.o lib/kernel/print.o
dd if=kernel/kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc
