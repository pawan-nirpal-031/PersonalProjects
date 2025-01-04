ORG 0x7c00
BITS 16

# all this does it ouput the letter 'A' to the screen
start:
    mov ah,0eh
    mov al, 'A'
    int 0x10 # interrupt to call bios

