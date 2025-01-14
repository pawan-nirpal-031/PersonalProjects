[BITS 32]
global _start
extern kernelMain

CODE_SEG equ 0x08 
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Remap the master PIC
    mov al, 00010001b
    out 0x20, al

    mov al, 0x20 ; Interrupt 0x20 is where the master PIC starts
    out 0x21, al

    mov al, 00000001b
    out 0x21, al

    ; Enable interrupts
    sti
    call kernelMain
    jmp $

times 512- ($ - $$) db 0  ; fill the rest of the sector with 0s basically padding the 510 bytes with start code and rest is all 0s
