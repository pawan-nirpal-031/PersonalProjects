section .asm

global idtLoad
extern int21hHandler
global int21h
global noInterrupts
extern noInterruptsHandler

idtLoad:
    push ebp
    mov ebp, esp

    mov ebx, [ebp + 8] ; idt
    lidt [ebx]

    pop ebp
    ret 

int21h:
    cli ; clear interrupt flag
    pushad ; push all general purpose registers to stack
    call int21hHandler
    popad
    sti ; set interrupt flag
    iret

noInterrupts:
    cli ; clear interrupt flag
    pushad ; push all general purpose registers to stack
    call noInterruptsHandler
    popad
    sti ; set interrupt flag
    iret