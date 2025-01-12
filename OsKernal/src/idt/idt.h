#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idtDesc{
    uint16_t offset1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t zero; // unused, set to 0 does nothing. 
    uint8_t type_attr; // descriptor type and attributes 
    uint16_t offset2; // offset bits 16..31
} __attribute__((packed)); // packed attribute is used to tell the compiler to not add any padding to the struct or change the alignment of the struct members and not to optimize the struct in any way.


struct idtrDesc{
    uint16_t limit; // Size of the descriptor table - 1
    uint32_t base; // Base address of the first idtDesc
} __attribute__((packed));

void idtInit();
#endif // IDT_H