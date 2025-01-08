nasm -f bin ./src/boot/boot.asm -o ./bin.bin
# ndisasm ./boot.bin 
qemu-system-x86_64 -hda ./bin/boot.bin