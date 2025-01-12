section .asm 

global insb
global insw
global outb
global outw

insb:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp + 8] ; port, 1st func arg
    in al, dx

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp + 8] ; port, 1st func arg
    in ax, dx

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 12] ; port. 1st func arg
    mov edx, [ebp + 8] ; data, 2nd func arg
    out dx, al  ; output 1Byte data to port

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 12] ; port. 1st func arg
    mov edx, [ebp + 8] ; data, 2nd func arg
    out dx, ax  ; output 2Byte data to port

    pop ebp
    ret