section .asm

global idtLoad

idtLoad:
    push ebp
    mov ebp, esp

    mov ebx, [ebp + 8] ; idt
    lidt [ebx]

    pop ebp
    ret 