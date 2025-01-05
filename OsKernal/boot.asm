ORG 0 ; Set the origin/PC to 0
BITS 16 ; All real mode code is 16 bits

_start:
    jmp short start 
    nop

times 33 db 0 ; 33 bytes of padding after shortjmp for bios parameter block for safety

; all this does it ouput the letter 'A' to the screen
start:
    jmp 0x7c0:step2 ; Jump to the start label/ Change of Code segment to 0x7c0. 

step2:
    cli ; clear interupt flags/Disable interupts, because we want to change some segment registers and we don't want to be interrupted while doing this operation.

    ; Explicit setting of the segment registers. 
    mov ax, 0x07C0 ; Set the data segment register to 0x07C0 which is the segment where the bootloader is loaded. 
    mov ds, ax ; Set the data segment register to 0x07C0
    mov es, ax ; Set the extra segment register to 0x07C0

    ; set the stack segment. 
    mov ax, 0x00 ; Set the stack segment register to 0x0000
    mov ss, ax ; Set the stack segment register to 0x0000
    mov sp, 0x7C00 ; Set the stack pointer to 0x7C00

    sti ; Set/Enable interupt flags
    mov si, message ; load the address of the message into si
    call print ; call the print function
    jmp $ ; infinite loop

; Rest of the sector is filled with 0s because we need to make sure that the byte 510 and 511 are 0xAA55 which is the boot signature

print:
    mov bx, 0 ; initalize the display terminal
.loop:
    lodsb ; load the byte from si into al and increment si
    cmp al, 0
    je .done ; if al is 0 then we are done
    call printChar ; call the printChar function
    jmp .loop ; jump back to the loop
.done: ; Sublabel of print
    ret

printChar:
    mov ah,0eh ; output to terminal function
    int 0x10 ; interrupt to call bios
    ret 


message: db "Hello, World!", 0   ; a label that defines a string Hello world

times 510 - ($ - $$) db 0  ; fill the rest of the sector with 0s basically padding the 510 bytes with start code and rest is all 0s
dw 0xAA55 ; boot signature 2 bytes that completes 512 bytes of sector on disk. Putting this boot signature tells the bios that this is a bootable disk
