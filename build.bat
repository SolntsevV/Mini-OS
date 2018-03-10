gcc -O0 -nostdlib -masm=intel main.c -o kernel.bin
objcopy -O binary kernel.bin
nasm -f bin loader.asm -o loader.bin
copy /b loader.bin+kernel.bin start.bin
copy_prog.exe start.bin start1.bin 1.bmp text.bmp text0.bmp
C:\programs\qemu\qemu-system-i386.exe start1.bin
pause