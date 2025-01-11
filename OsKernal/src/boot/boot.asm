ORG 0x7c00 ; the origin/PC to start the code from.
BITS 16 ; All real mode code is 16 bits

CODE_SEG equ gdtCode - gdtStart
DATA_SEG equ gdtData - gdtStart

_start:
    jmp short start 
    nop

times 33 db 0 ; 33 bytes of padding after shortjmp for bios parameter block for safety

; all this does it ouput the letter 'A' to the screen
start:
    jmp 0:step2 ; Jump to the start label/ Change of Code segment to 0x7c0. 

; Writing our own interupt to handle div by zero. 
handleZero:
    mov ah, 0eh
    mov al, 'A'
    mov bx, 0x00
    int 0x10
    iret ; intr return

step2:
    cli ; clear interupt flags/Disable interupts, because we want to change some segment registers and we don't want to be interrupted while doing this operation.

    ; Explicit setting of the segment registers. 
    mov ax, 0 ; Set the data segment register to 0x0000
    mov ds, ax ; Set the data segment register to 0x07C0
    mov es, ax ; Set the extra segment register to 0x07C0

    ; set the stack segment. 
    mov ax, 0x00 ; Set the stack segment register to 0x0000
    mov ss, ax ; Set the stack segment register to 0x0000
    mov sp, 0x7C00 ; Set the stack pointer to 0x7C00

    sti ; Set/Enable interupt flags

.loadProtected:
    cli ; clear interupt flags/Disable interupts, because we want to change some segment registers and we don't want to be interrupted while doing this operation.
    lgdt[gdtDiscriptor] ; Load the GDT table
    mov eax, cr0 ; Move the value of cr0 to eax
    or eax, 0x1 
    mov cr0, eax ; Set the first bit of cr0 to 1 to enable protected mode
    jmp CODE_SEG:load32


; GDT for jumping to protected mode. 
gdtStart:
gdtNull:
    dd 0x0
    dd 0x0 

; offset 0x8 
gdtCode:       ; CS should point to this. 
    dw 0xffff ; segment limit first 0-15 bits
    dw 0x0 ; base address first 0-15 bits
    db 0x0 ; base address 16-23 bits
    db 0x9a ; access byte
    db 11001111b ; high and low flags. 
    db 0 ; base address 24-31 bits

; offset 0x10
gdtData:       ; DS, ES, SS should point to this. 
    dw 0xffff ; segment limit first 0-15 bits
    dw 0x0 ; base address first 0-15 bits
    db 0x0 ; base address 16-23 bits
    db 0x92 ; access byte
    db 11001111b ; high and low flags. 
    db 0 ; base address 24-31 bits

gdtEnd:

gdtDiscriptor:
    dw gdtEnd - gdtStart - 1 ; size of the GDT
    dd gdtStart ; address of the GDT

[BITS 32]
load32:
    mov eax, 1
    mov ecx,100 
    mov edi, 0x100000
    call ataLbaRead
    jmp CODE_SEG:0x0100000


ataLbaRead:
    mov ebx,eax ; LBA backup. 
    ; send highest 8 bits of lba to the disk controller. 
    shr eax,24
    or eax,0xE0 ; Selcting the master drive
    mov dx, 0x1F6 ; Port number for sending the highest 8 bits of LBA
    out dx, al ; send the highest 8 bits of LBA to the disk controller.
    ; Done sending the highest 8 bits of LBA to the disk controller.

    ; Send the total sectors to read.
    mov eax, ecx 
    mov dx, 0x1F2 ; Port number for sending the number of sectors to read/write
    out dx, al ; send the number of sectors to read/write to the disk controller.

    mov eax,ebx ; LBA restore.
    mov dx, 0x1F3 ; Port number for sending the LBA to the disk controller
    out dx, al ; send the LBA to the disk controller.

    ; Send some more bits of LBA to the disk controller.
    mov dx, 0x1F4 ; Port number for sending the LBA to the disk controller
    mov eax, ebx ; LBA restore.
    shr eax,8
    out dx, al ; send the LBA to the disk controller.

    ; Send upper 16 bits of LBA to the disk controller.
    mov dx, 0x1F5 ; Port number for sending the LBA to the disk controller
    mov eax, ebx ; LBA restore.
    shr eax,16  
    out dx, al ; send the LBA to the disk controller.
    ; Finished sending the LBA to the disk controller.

    mov dx, 0x1F7 ; Port number for sending the command to the disk controller
    mov al, 0x20 ; Read command
    out dx, al ; send the read command to the disk controller.

.nextSector:
    push ecx ; backup the ecx register

; checking if we need read. 
.tryAgain:
    mov dx, 0x1F7 ; Port number for sending the command to the disk controller
    in al, dx ; read the status of the disk controller
    test al, 0x80 ; check if the disk is busy
    jnz .tryAgain ; if the disk is busy then try again

    mov ecx, 256 ; number of bytes to read
    mov dx, 0x1F0 ; Port number for reading the data from the disk controller
    rep insw ; read the data from the port 0x1F0 to the memory location pointed by edi which is 1M, rep means do this ecx times which is 256 times
    pop ecx ; restore the ecx register
    loop .nextSector ; loop through the sectors to read
    ret 


; Rest of the sector is filled with 0s because we need to make sure that the byte 510 and 511 are 0xAA55 which is the boot signature

times 510 - ($ - $$) db 0  ; fill the rest of the sector with 0s basically padding the 510 bytes with start code and rest is all 0s
dw 0xAA55 ; boot signature 2 bytes that completes 512 bytes of sector on disk. Putting this boot signature tells the bios that this is a bootable disk
