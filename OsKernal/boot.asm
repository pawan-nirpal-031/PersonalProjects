ORG 0x7c00
BITS 16

; all this does it ouput the letter 'A' to the screen
start:
    mov si, message ; load the address of the message into si
    call print ; call the print function
    jmp $ ; infinite loop

; Rest of the sector is filled with 0s because we need to make sure that the byte 510 and 511 are 0xAA55 which is the boot signature

print:
.loop:
    lodsb ; load the byte from si into al and increment si
    cmp al, 0
    je .done ; if al is 0 then we are done
    call printChar ; call the printChar function
    jmp .loop ; jump back to the loop
.done: ; Sublabel of print
    ret

printChar:
    mov ah,0eh
    int 0x10 ; interrupt to call bios
    ret 


message: db "Hello, World!", 0   ; a label that defines a string Hello world

times 510 - ($ - $$) db 0  ; fill the rest of the sector with 0s basically padding the 510 bytes with start code and rest is all 0s
dw 0xAA55 ; boot signature 2 bytes that completes 512 bytes of sector on disk. Putting this boot signature tells the bios that this is a bootable disk
